/*
* Copyright (c) 2017 Agora.io
* All rights reserved.
* Proprietary and Confidential -- Agora.io
*/

/*
*  Created by Wang Yongli, 2017
*/

#include "node_video_render.h"

namespace agora {
    namespace rtc {
        using namespace std::placeholders;

        NodeVideoRenderFactory::NodeVideoRenderFactory(NodeRtcEngine& engine)
            : m_engine(engine)
        {}

        NodeVideoRenderFactory::~NodeVideoRenderFactory() {}

        IExternalVideoRender* NodeVideoRenderFactory::createRenderInstance(const ExternalVideoRenerContext& context)
        {
            if (!context.view)
                return nullptr;
            
            return new NodeVideoRender((channel_handle_type)context.view, context);
        }

        NodeVideoRender::NodeVideoRender(channel_handle_type channel, const ExternalVideoRenerContext& context)
            :m_renderCallback(context.renderCallback)
        {
            m_channel = channel;
            auto p = m_channel;
            if (p) {
                p->setViewProperties(context);
            }
        }

        NodeVideoRender::~NodeVideoRender() 
        {
        }

        int NodeVideoRender::initialize()
        {
            return 0;
        }

        void NodeVideoRender::release()
        {

        }

        int NodeVideoRender::deliverFrame(const IVideoFrame& videoFrame, int rotation, bool mirrored)
        {
            auto p = m_channel;
            if (p)
                return p->deliverFrame(videoFrame, rotation, mirrored);
            return -1;
        }
    }
}