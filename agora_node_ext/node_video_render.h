/*
* Copyright (c) 2017 Agora.io
* All rights reserved.
* Proprietry and Confidential -- Agora.io
*/

/*
*  Created by Wang Yongli, 2017
*/

#ifndef AGORA_NODE_VIDEO_RENDER_H
#define AGORA_NODE_VIDEO_RENDER_H

#include "internal/rtc_engine_i.h"
#include "IAgoraMediaEngine.h"
#include "IAgoraRtcEngine.h"
#include "agora_rtc_engine.h"
#include "external/chat_engine/chat_engine_i.h"
#include "node_video_stream_channel.h"

namespace agora {
    namespace rtc {
        using media::IExternalVideoRenderFactory;
        using media::IExternalVideoRender;
        using media::ExternalVideoRenerContext;
        using media::IVideoFrame;
        using media::IExternalVideoRenderCallback;

        /**
         * IExternalVideoRenderFactory implementation used to create video render.
         */
        class NodeVideoRenderFactory : public IExternalVideoRenderFactory
        {
        public:
            NodeVideoRenderFactory(NodeRtcEngine& engine);
            ~NodeVideoRenderFactory();

        public:
            virtual IExternalVideoRender* createRenderInstance(const ExternalVideoRenerContext& context) override;
        protected:
            NodeRtcEngine& m_engine;
        };

        /**
         * IExtenral Video Render implementation used to render video data.
         */
        class NodeVideoRender : public IExternalVideoRender
        {
        public:
            NodeVideoRender(channel_handle_type channel, const ExternalVideoRenerContext& context);
            ~NodeVideoRender();

        public:
            virtual void release() override;
            virtual int initialize() override;
            virtual int deliverFrame(const IVideoFrame& videoFrame, int rotation, bool mirrored) override;
        private:
            IExternalVideoRenderCallback *m_renderCallback;
            std::mutex m_mutex;
            channel_handle_type m_channel;
        };
    }
}

#endif