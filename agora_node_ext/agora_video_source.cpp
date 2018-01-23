/*
* Copyright (c) 2018 Agora.io
* All rights reserved.
* Proprietary and Confidential -- Agora.io
*/

/*
*  Created by Wang Yongli, 2018
*/

#include "agora_video_source.h"
#include "node_log.h"
#include "node_async_queue.h"
#include "node_video_stream_channel.h"
#include "video_source_ipc.h"
#include <Rpc.h>
#include <RpcDce.h>
#include <thread>
#include <functional>
#include <string>
#include <sstream>

#define DATA_IPC_NAME "agora_video_source_ipc"
#define PROCESS_RUN_EVENT_NAME "agora_video_source_process_ready_event_name"
namespace agora{
    namespace rtc{
        /**
         * AgoraVideoSource implementation
         * agoraVideoSource starts two thread, one for message transform, and one for video data transform.
         */
        class AgoraVideoSourceSink : public AgoraVideoSource, public AgoraIpcListener
        {
        public:
            AgoraVideoSourceSink();
            ~AgoraVideoSourceSink();


            virtual bool initialize(IAgoraVideoSourceEventHandler *eventHandler) override;
            virtual void join(const char* token, const char* cname,
                const char* chan_info, uid_t uid) override;
            virtual void release() override;
            virtual void renewVideoSourceToken(const char* token) override;
            virtual void setVideoSourceChannelProfile(agora::rtc::CHANNEL_PROFILE_TYPE profile) override;
            virtual void setVideoSourceVideoProfile(agora::rtc::VIDEO_PROFILE_TYPE profile, bool swapWidthAndHeight) override;
            virtual void onMessage(unsigned int msg, char* payload, unsigned int len) override;
            virtual void captureScreen(agora::rtc::IRtcEngine::WindowIDType id, int captureFreq, agora::rtc::Rect* rect, int bitrate) override;
            virtual void updateScreenCapture(agora::rtc::Rect* rect) override;
            virtual void stopCaptureScreen() override;
        private:
            void renderThread();
            void msgThread();
            void deliverFrame(const char* payload, int len);
            void clear();

        private:
            std::thread m_hThread;
            std::thread m_msgThread;
            char* m_backBuf;
            mutable std::mutex m_lock;
            std::unique_ptr<IAgoraIpc> m_ipcMsg;
            std::unique_ptr<AgoraIpcDataReceiver> m_ipcReceiver;
            std::string m_peerId;
            bool m_peerJoined;
            bool m_initialized;
            IAgoraVideoSourceEventHandler *m_eventHandler;
            uid_t m_peerUid;
        private:
            static const unsigned int s_bufLen;
        };
        const unsigned int AgoraVideoSourceSink::s_bufLen = 1920 * 1080 * 1.5 + 50;

        AgoraVideoSource* createVideoSource()
        {
            return new AgoraVideoSourceSink();
        }

        AgoraVideoSourceSink::AgoraVideoSourceSink()
            : m_hThread()
            , m_msgThread()
            , m_backBuf(nullptr)
            , m_initialized(false)
        {}

        AgoraVideoSourceSink::~AgoraVideoSourceSink()
        {
            clear();
        }

        void AgoraVideoSourceSink::clear()
        {
            m_initialized = false;
            m_eventHandler = nullptr;
            m_ipcReceiver.reset();
            if (m_backBuf) {
                free(m_backBuf);
                m_backBuf = NULL;
            }
        }

        void AgoraVideoSourceSink::release()
        {
            if (m_initialized){
                m_ipcMsg->disconnect();
                clear();
            }
        }

        bool getCurrentModuleFileName(std::string& targetPath)
        {
            HMODULE module = NULL;
            if (!GetModuleHandleExA(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS, (LPCSTR)&getCurrentModuleFileName, &module)) {
                return false;
            }

            char path[MAX_PATH] = { 0 };
            if (!GetModuleFileNameA(module, path, MAX_PATH)){
                return false;
            }
            targetPath.assign(path);
            return true;
        }

        bool AgoraVideoSourceSink::initialize(IAgoraVideoSourceEventHandler *eventHandler)
        {
            if (m_initialized)
                return true;
            clear();
            m_eventHandler = eventHandler;
            m_ipcReceiver.reset(new AgoraIpcDataReceiver());
            UUID uuid = { 0 };
            RPC_CSTR struuid;

            if (UuidCreate(&uuid) != RPC_S_OK)
                return false;
            if (UuidToStringA(&uuid, &struuid) != RPC_S_OK)
                return false;
            m_peerId = (LPSTR)struuid;
            RpcStringFreeA(&struuid);

            m_backBuf = (char*)malloc(AgoraVideoSourceSink::s_bufLen);
            if (!m_backBuf){
                clear();
                return false;
            }

            HANDLE hEvent = CreateEventA(NULL, FALSE, FALSE, (m_peerId + PROCESS_RUN_EVENT_NAME).c_str());
            if (!hEvent)
                return false;

            m_ipcMsg.reset(createAgoraIpc(this));
            m_ipcMsg->initialize(m_peerId);

            std::string targetPath;
            if (!getCurrentModuleFileName(targetPath)){
                clear();
                return false;
            }

            size_t pos = targetPath.find_last_of("\\/");
            if (pos == targetPath.npos){
                clear();
                return false;
            }
            std::stringstream ss;
            ss << GetCurrentProcessId();
            std::string cmdline = targetPath.substr(0, pos + 1) + "VideoSource.exe id:" + m_peerId + " pid:" + ss.str();
            STARTUPINFOA info = { 0 };
            info.cb = sizeof(info);
            PROCESS_INFORMATION pi = { 0 };
            DWORD err;
            char pram[150] = { 0 };
            memcpy(pram, cmdline.c_str(), cmdline.length());
            if (!CreateProcessA(NULL, pram, NULL, NULL, FALSE, CREATE_NO_WINDOW | CREATE_SUSPENDED, NULL, NULL, &info, &pi))
            {
                err = GetLastError();
                clear();
                return false;
            }
            ResumeThread(pi.hThread);
            DWORD result = WaitForSingleObject(hEvent, 5000);
            if (result != WAIT_OBJECT_0){
                TerminateProcess(pi.hProcess, 0);
                clear();
                return false;
            }
            m_ipcMsg->connect();
            m_msgThread = std::thread(&AgoraVideoSourceSink::msgThread, this);

            if (!m_ipcReceiver->initialize(m_peerId + DATA_IPC_NAME, std::bind(&AgoraVideoSourceSink::deliverFrame, this, std::placeholders::_1, std::placeholders::_2))){
                m_ipcMsg->sendMessage(AGORA_IPC_DISCONNECT, nullptr, 0);
                clear();
                return false;
            }
            m_hThread = std::thread(&AgoraVideoSourceSink::renderThread, this);
            m_initialized = true;
            return true;
        }

        void AgoraVideoSourceSink::msgThread()
        {
            m_ipcMsg->run();
        }

        void AgoraVideoSourceSink::join(const char* token, const char* cname, const char* chan_info, uid_t uid)
        {
            if (m_initialized){
                m_peerUid = uid;
                JoinChannelCmd *cmd = new JoinChannelCmd();
                memset(cmd, 0, sizeof(*cmd));
                if (token)
                    strcpy(cmd->token, token);
                if (cname) {
                    strcpy(cmd->cname, cname);
                }
                if (chan_info)
                    strcpy(cmd->chan_info, chan_info);
                cmd->uid = uid;
                m_ipcMsg->sendMessage(AGORA_IPC_JOIN, (char*)cmd, sizeof(*cmd));
                delete cmd;
            }
        }

        void AgoraVideoSourceSink::renewVideoSourceToken(const char* token)
        {
            if (m_initialized){
                m_ipcMsg->sendMessage(AOGRA_IPC_RENEW_TOKEN, (char*)token, strlen(token));
            }
        }
        
        void AgoraVideoSourceSink::setVideoSourceChannelProfile(agora::rtc::CHANNEL_PROFILE_TYPE profile)
        {
            if (m_initialized){
                m_ipcMsg->sendMessage(AGORA_IPC_SET_CHANNEL_PROFILE, (char*)&profile, sizeof(profile));
            }
        }
        
        void AgoraVideoSourceSink::setVideoSourceVideoProfile(agora::rtc::VIDEO_PROFILE_TYPE profile, bool swapWidthAndHeight)
        {
            if (m_initialized){
                VideoProfileCmd cmd;
                cmd.profile = profile;
                cmd.swapWidthAndHeight = swapWidthAndHeight;
                m_ipcMsg->sendMessage(AGORA_IPC_SET_VIDEO_RPOFILE, (char*)&cmd, sizeof(cmd));
            }
        }

        void AgoraVideoSourceSink::onMessage(unsigned int msg, char* payload, unsigned int len)
        {
            if (!m_initialized)
                return;
            if (msg == AGORA_IPC_JOIN_SUCCESS){
                m_peerJoined = true;
                if (m_eventHandler){
                    m_eventHandler->onVideoSourceJoinedChannel(*((agora::rtc::uid_t*)payload));
                }
            }
            else if (msg == AGORA_IPC_LEAVE_CHANNEL){
                if (m_eventHandler){
                    m_eventHandler->onVideoSourceLeaveChannel();
                }
            }
            else if (msg == AOGRA_IPC_RENEW_TOKEN){
                if (m_eventHandler){
                    m_eventHandler->onVideoSourceRequestNewToken();
                }
            }
            else if (msg == AGORA_IPC_RENDER_READY){
            }
        }

        void AgoraVideoSourceSink::captureScreen(agora::rtc::IRtcEngine::WindowIDType id, int captureFreq, agora::rtc::Rect* rect, int bitrate)
        {
            if (m_initialized && m_peerJoined){
                CaptureScreenCmd cmd = { 0 };
                cmd.windowid = id;
                cmd.captureFreq = captureFreq;
                if (rect){
                    cmd.rect.top = rect->top;
                    cmd.rect.left = rect->left;
                    cmd.rect.right = rect->right;
                    cmd.rect.bottom = rect->bottom;
                }
                cmd.bitrate = bitrate;
                m_ipcMsg->sendMessage(AGORA_IPC_CAPTURE_SCREEN, (char*)&cmd, sizeof(cmd));
            }
        }

        void AgoraVideoSourceSink::updateScreenCapture(agora::rtc::Rect* rect)
        {
            if (m_initialized && m_peerJoined){
                m_ipcMsg->sendMessage(AGORA_IPC_UPDATE_CAPTURE_SCREEN, (char*)rect, sizeof(*rect));
            }
        }

        void AgoraVideoSourceSink::stopCaptureScreen()
        {
            if (m_initialized && m_peerJoined){
                m_ipcMsg->sendMessage(AGORA_IPC_STOP_CAPTURE_SCREEN, nullptr, 0);
            }
        }

        void AgoraVideoSourceSink::renderThread()
        {
            m_ipcReceiver->run();
        }

        struct image_frame_info {
            int stride;
            int stride0;
            int width;
            int height;
            int strideU;
            int strideV;
        };

        struct image_header_type {
            unsigned char format;
            unsigned char mirrored;
            unsigned short width;
            unsigned short height;
            unsigned short left;
            unsigned short top;
            unsigned short right;
            unsigned short bottom;
            unsigned short rotation;
            unsigned int timestamp;
        };

        void AgoraVideoSourceSink::deliverFrame(const char* payload , int len)
        {
            std::lock_guard<std::mutex> _lock(m_lock);
            memcpy(m_backBuf, payload, len);
            node_async_call::asyncCall([this]() {
                std::lock_guard<std::mutex> _lock(this->m_lock);
                Isolate *isolate = Isolate::GetCurrent();
                HandleScope scope(isolate);

                if (!NodeVideoStreamChannel::s_nodeInfo.init)
                    return;
                Local<Value> args[5];
                args[0] = napi_create_uid_(isolate, m_peerUid);

                image_frame_info *info = (image_frame_info*)this->m_backBuf;
                image_header_type *hdr = (image_header_type*)(this->m_backBuf + sizeof(image_frame_info));
                int hdr_offset = (unsigned char*)hdr - (unsigned char*)info;
                unsigned int len = (info->stride / 2) * (info->height / 2);
                char* y = (char*)(hdr + 1);
                int y_offset = y - this->m_backBuf;
                char* u = y + len * 4;
                int u_offset = u - this->m_backBuf;
                char* v = u + len;
                int v_offset = v - this->m_backBuf;
                args[1] = v8::ArrayBuffer::New(isolate, hdr, sizeof(*hdr));
                Local<v8::ArrayBuffer> buff = v8::ArrayBuffer::New(isolate, y, len * 4);
                args[2] = v8::Uint8Array::New(buff, 0, len * 4);

                Local<v8::ArrayBuffer> uBuff = v8::ArrayBuffer::New(isolate, u, len);
                args[3] = v8::Uint8Array::New(uBuff, 0, len);

                Local<v8::ArrayBuffer> vBuff = v8::ArrayBuffer::New(isolate, v, len);
                args[4] = v8::Uint8Array::New(vBuff, 0, len);

                NodeVideoStreamChannel::s_nodeInfo.callback.Get(isolate)->Call(NodeVideoStreamChannel::s_nodeInfo.js_this.Get(isolate), 5, args);
            });
        }
    }
}