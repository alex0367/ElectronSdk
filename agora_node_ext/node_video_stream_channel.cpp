/*
* Copyright (c) 2017 Agora.io
* All rights reserved.
* Proprietry and Confidential -- Agora.io
*/

/*
*  Created by Wang Yongli, 2017
*/
#include "node_video_stream_channel.h"
#include <util.h>
#include "node_async_queue.h"
#include "node.h"
#include "node_buffer.h"
#include "v8.h"
namespace agora {
    namespace rtc {
        using namespace agora::media;
        using namespace agora::commons;

#if defined(_WIN32)
#if defined(_MSC_VER) || defined(__BORLANDC__)
#define EPOCHFILETIME (116444736000000000i64)
#else
#define EPOCHFILETIME (116444736000000000LL)
#endif

        struct timezone {
            int tz_minuteswest;
            int tz_dsttime;
        };

        static int node_gettimeofday(struct timeval *tv, struct timezone *tz)
        {
            FILETIME        ft;
            LARGE_INTEGER   li;
            __int64         t;
            static int      tzflag;

            if (tv)
            {
                GetSystemTimeAsFileTime(&ft);
                li.LowPart = ft.dwLowDateTime;
                li.HighPart = ft.dwHighDateTime;
                t = li.QuadPart;       /* In 100-nanosecond intervals */
                t -= EPOCHFILETIME;     /* Offset to the Epoch time */
                t /= 10;                /* In microseconds */
                tv->tv_sec = (long)(t / 1000000);
                tv->tv_usec = (long)(t % 1000000);
            }

            if (tz)
            {
                if (!tzflag)
                {
                    _tzset();
                    tzflag++;
                }
                tz->tz_minuteswest = _timezone / 60;
                tz->tz_dsttime = _daylight;
            }

            return 0;
        }
        static uint64_t get_timestamp()
        {
            struct timeval tv;
            node_gettimeofday(&tv, NULL);
            return ((uint64_t)tv.tv_sec) * 1000 + tv.tv_usec / 1000;
        }
#else
        static uint64_t get_timestamp()
        {
            struct timeval tv;
            gettimeofday(&tv, NULL);
            return ((uint64_t)tv.tv_sec) * 1000 + tv.tv_usec / 1000;
        }
#endif

        NodeVideoStreamChannel::NodeVideoStreamChannel(NodeRtcEngine& rtcEngine, channel_context& channel)
            : m_engine(&rtcEngine)
            , m_channelContext(channel)
            , m_tsStats(0)
            , m_sentFrames(0)
            , m_skippedFrames(0)
        {
            m_muted = false;
        }

        NodeVideoStreamChannel::~NodeVideoStreamChannel()
        {
        }

        void NodeVideoStreamChannel::setViewProperties(const agora::media::ExternalVideoRenerContext& context)
        {
            m_properties.zOrder = context.zOrder;
            m_properties.renderMode = context.renderMode;
            m_properties.top = context.top;
            m_properties.left = context.left;
            m_properties.bottom = context.bottom;
            m_properties.right = context.right;
        }

        void NodeVideoStreamChannel::setChannelMuteState(bool muted)
        {
            m_muted = muted;
        }

        static uv_work_t work;
        static bool stop = false;

        int NodeVideoStreamChannel::deliverFrame(const agora::media::IVideoFrame& videoFrame, int rotation, bool mirrored)
        {
            if (m_muted)
                return 0;
            if (stop)
                return 0;
            std::shared_ptr<NodeVideoStreamChannel> ptr = this->m_wp.lock();
            if (this->m_wp.expired()) return 0;
            std::lock_guard<std::mutex> _lock(m_lock);
            int r = deliverFrame_I420(videoFrame, rotation, mirrored, buffers);
            if (!r && m_engine) {
                int skips = buffers.size();
                int *pskips = &skips;
                node_async_call::asyncCall([&, ptr]() {
                    std::lock_guard<std::mutex> _lock(ptr->m_lock);
                    Isolate *isolate = Isolate::GetCurrent();
                    HandleScope scope(isolate);

                    if (!NodeVideoStreamChannel::s_nodeInfo.init)
                        return;
                    Local<Value> args[5];
                    args[0] = napi_create_uid_(isolate, m_channelContext.uid);
                    int index = 1;
                    for (auto & it = buffers.begin(); it != buffers.end(); ++it, ++index) {
                        Local<v8::ArrayBuffer> buff = v8::ArrayBuffer::New(isolate, it->buffer, it->length);
                        if (it != buffers.begin()) {
                            Local<v8::Uint8Array> array = v8::Uint8Array::New(buff, 0, it->length);
                            args[index] = array;
                        }
                        else {
                            args[index] = buff;
                        }
                    }
                    NodeVideoStreamChannel::s_nodeInfo.callback.Get(isolate)->Call(NodeVideoStreamChannel::s_nodeInfo.js_this.Get(isolate), 5, args);
                });
            }
            return r;
        }

        int NodeVideoStreamChannel::deliverFrame_RGBA(const agora::media::IVideoFrame& videoFrame, int rotation)
        {
            int height = videoFrame.height();
            int stride = videoFrame.stride(IVideoFrame::Y_PLANE);
            size_t imageSize = sizeof(image_header_type) + stride * height * 4;

            auto s = m_buffer.size();
            if (s < imageSize || s >= imageSize * 2)
                m_buffer.resize(imageSize);
            image_header_type *hdr = reinterpret_cast<image_header_type*>(&m_buffer[0]);
            hdr->format = 0;
            hdr->width = htons(stride);
            hdr->height = htons(height);

            rotation = rotation < 0 ? rotation + 360 : rotation;
            hdr->rotation = htons(rotation);
            int r = videoFrame.convertFrame(IVideoFrame::VIDEO_TYPE_ABGR, 0, (unsigned char*)&hdr[1]);
            return r;
        }

        int NodeVideoStreamChannel::deliverFrame_I420(const agora::media::IVideoFrame& videoFrame, int rotation, bool mirrored, buffer_list& buffers)
        {
            int stride, stride0 = videoFrame.stride(IVideoFrame::Y_PLANE);
            stride = stride0;
            if (stride & 0xf) {
                stride = (((stride + 15) >> 4) << 4);
            }
            int width = videoFrame.width();
            int height = videoFrame.height();

            rotation = rotation < 0 ? rotation + 360 : rotation;
            size_t imageSize = sizeof(image_header_type) + stride * height * 3 / 2;

            auto s = m_buffer.size();
            if (s < imageSize || s >= imageSize * 2)
                m_buffer.resize(imageSize);
            image_header_type* hdr = reinterpret_cast<image_header_type*>(&m_buffer[0]);
            hdr->mirrored = mirrored ? 1 : 0;
            hdr->rotation = htons(rotation);
            setupFrameHeader(hdr, stride, width, height);
            copyFrame(videoFrame, (unsigned char*)&hdr[1], stride, stride0, width, height, buffers);
            return 0;
        }

        void NodeVideoStreamChannel::setupFrameHeader(image_header_type*header, int stride, int width, int height)
        {
            int left = 0;
            int top = 0;
            header->format = 0;
            header->width = htons(width);
            header->height = htons(height);
            header->left = htons((uint16_t)left);
            header->top = htons((uint16_t)top);
            header->right = htons((uint16_t)(stride - width - left));
            header->bottom = htons((uint16_t)0);
            header->timestamp = (uint32_t)htonl((uint32_t)get_timestamp());
        }

        void NodeVideoStreamChannel::copyFrame(const agora::media::IVideoFrame& videoFrame, unsigned char *buffer, int dest_stride, int src_stride, int width, int height, buffer_list& buffers)
        {
            int width2 = width / 2, heigh2 = height / 2;
            int strideY = videoFrame.stride(IVideoFrame::Y_PLANE);
            int strideU = videoFrame.stride(IVideoFrame::U_PLANE);
            int strideV = videoFrame.stride(IVideoFrame::V_PLANE);

            unsigned char* y = buffer;
            unsigned char* u = y + dest_stride*height;
            unsigned char* v = u + dest_stride / 2 * heigh2;
            const unsigned char* planeY = videoFrame.buffer(IVideoFrame::Y_PLANE);
            const unsigned char* planeU = videoFrame.buffer(IVideoFrame::U_PLANE);
            const unsigned char* planeV = videoFrame.buffer(IVideoFrame::V_PLANE);

            {
                int i;
                for (i = 0; i < heigh2; i++) {
                    memcpy(y + i * dest_stride, planeY + i * strideY, width);
                    memcpy(u + i *dest_stride / 2, planeU + i * strideU, width2);
                    memcpy(v + i * dest_stride / 2, planeV + i * strideV, width2);
                }
                for (; i < height; i++)
                    memcpy(y + i * dest_stride, planeY + i * strideY, width);
            }

            buffers[0].buffer = &m_buffer[0];
            buffers[0].length = sizeof(image_header_type);

            buffers[1].buffer = y;
            buffers[1].length = dest_stride * height;

            buffers[2].buffer = u;
            buffers[2].length = dest_stride / 2 * heigh2;

            buffers[3].buffer = v;
            buffers[3].length = dest_stride / 2 * heigh2;
        }

        NodeVideoStreamChannel::node_info NodeVideoStreamChannel::s_nodeInfo;
    }
}