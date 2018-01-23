/*
* Copyright (c) 2017 Agora.io
* All rights reserved.
* Proprietary and Confidential -- Agora.io
*/

/*
*  Created by Wang Yongli, 2017
*/

#ifndef NODE_VIDEO_STREAM_CHANNEL_H
#define NODE_VIDEO_STREAM_CHANNEL_H

#include <memory>
#include <mutex>
#include <array>
#include <atomic>
#include "agora_rtc_engine.h"
#pragma once

namespace agora {
    namespace rtc {
        struct channel_context {
            uid_t uid;
            channel_context()
                : uid(0)
            {}
        };

        /**
         * NodeVideoStreamChannel is used to transfer video data from SDK to JS layer.
         */
        class NodeVideoStreamChannel
        {
            struct image_header_type {
                uint8_t format;
                uint8_t mirrored;
                uint16_t width;
                uint16_t height;
                uint16_t left;
                uint16_t top;
                uint16_t right;
                uint16_t bottom;
                uint16_t rotation;
                uint32_t timestamp;
            };

            struct view_properties {
                int renderMode;
                int zOrder;
                float left;
                float top;
                float right;
                float bottom;
                view_properties()
                    : renderMode(0)
                    , zOrder(0)
                    , left(0)
                    , top(0)
                    , right(0)
                    , bottom(0)
                {}
            };

            struct buffer_info {
                unsigned char* buffer;
                uint32_t length;
            };

        public:

            struct node_info {
                bool init;
                Isolate* env;
                Persistent<Function> callback;
                Persistent<Object> js_this;
            };
            using buffer_list = std::array<buffer_info, 4>;

        public:
            NodeVideoStreamChannel(NodeRtcEngine& rtcEngine, channel_context& channel);
            ~NodeVideoStreamChannel();

            void setViewProperties(const agora::media::ExternalVideoRenerContext& context);
            int deliverFrame(const agora::media::IVideoFrame& videoFrame, int rotation, bool mirrored);
            void setChannelMuteState(bool muted);
            uid_t getUid() { return m_channelContext.uid; }
            void setWeakToSelf(std::shared_ptr<NodeVideoStreamChannel>& sp){ m_wp = sp; }

        private:
            int deliverFrame_RGBA(const agora::media::IVideoFrame& videoFrame, int rotation);
            int deliverFrame_I420(const agora::media::IVideoFrame& videoFrame, int rotation, bool mirrored, buffer_list& buffers);
            void setupFrameHeader(image_header_type *header, int stride, int width, int height);
            void copyFrame(const agora::media::IVideoFrame& videoFrame, unsigned char *buffer, int dst_stride, int src_stride, int width, int height, buffer_list& buffers);
        private:
            typedef std::vector<unsigned char> stream_buffer_type;
            std::weak_ptr<NodeVideoStreamChannel> m_wp;
            NodeRtcEngine *m_engine;
            stream_buffer_type m_buffer;
            view_properties m_properties;
            int m_sentFrames;
            int m_skippedFrames;
            std::atomic_bool m_muted;
            uint64_t m_tsStats;
            channel_context m_channelContext;
            mutable std::mutex m_lock;
            buffer_list buffers;
        public:
            static NodeVideoStreamChannel::node_info s_nodeInfo;
        };

        using channel_handle_type = NodeVideoStreamChannel*;
    }
}

#endif