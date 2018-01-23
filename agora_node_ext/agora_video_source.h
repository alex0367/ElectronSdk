/*
* Copyright (c) 2018 Agora.io
* All rights reserved.
* Proprietary and Confidential -- Agora.io
*/

/*
*  Created by Wang Yongli, 2018
*/

#ifndef AGORA_VIDEO_SOURCE_SINK_H
#define AGORA_VIDEO_SOURCE_SINK_H

#include "IAgoraRtcEngine.h"
#include <string>

namespace agora{
    namespace rtc{
        /**
         * Video source need join the same channel, this class used to monitor video source's event.
         * More event maybe needed in future.
         */
        class IAgoraVideoSourceEventHandler
        {
        public:
            virtual ~IAgoraVideoSourceEventHandler(){}
            /**
             * Video source joined channel success event.
             * @param uid : video source's uid.
             */
            virtual void onVideoSourceJoinedChannel(agora::rtc::uid_t uid) = 0;

            /**
             * Video source request new token event.
             */
            virtual void onVideoSourceRequestNewToken() = 0;

            /**
             * Video source leaved channel event.
             */
            virtual void onVideoSourceLeaveChannel() = 0;
        };

        /**
         * This is video source stub interface.
         */
        class AgoraVideoSource
        {
        public:
            virtual ~AgoraVideoSource(){}

            /**
             * To initialize Video source.
             * @param eventHandler : video source event handler.
             */
            virtual bool initialize(IAgoraVideoSourceEventHandler *eventHandler) = 0;

            /**
             * To ask video source to join channel with specified parameters.
             * @param token : token if it is enabled.
             * @param cname : channel name.
             * @param chan_info : channel information
             * @param uid : uid of video source.
             */
            virtual void join(const char* token, const char* cname, const char* chan_info, uid_t uid) = 0;

            /**
             * Release video source.
             */
            virtual void release() = 0;

            /**
             * To ask video source begin to share screen.
             * @param id : window id whose window will be shared. if the value is 0, then desktop is shared.
             * @param captureFreq : video frequency, 0-15.
             * @param rect : the shared area
             * @param bitrate : bitrate of video
             */
            virtual void captureScreen(agora::rtc::IRtcEngine::WindowIDType id, int captureFreq, agora::rtc::Rect* rect, int bitrate) = 0;

            /**
             * To update shared window area
             * @param rect : updated area
             */
            virtual void updateScreenCapture(agora::rtc::Rect* rect) = 0;

            /**
             * To stop screen share
             */
            virtual void stopCaptureScreen() = 0;

            /**
             * To renew video source's token.
             * @param token : new token
             */
            virtual void renewVideoSourceToken(const char* token) = 0;

            /**
             * To set video source channel profile
             * @param profile : video source's channel profile
             */
            virtual void setVideoSourceChannelProfile(agora::rtc::CHANNEL_PROFILE_TYPE profile) = 0;

            /**
             * To set video source's video profile
             * @param profile : the video source's video profile
             * @param swapWidthAndHeight : whether adjust width and height
             */
            virtual void setVideoSourceVideoProfile(agora::rtc::VIDEO_PROFILE_TYPE profile, bool swapWidthAndHeight) = 0;
        };

        /**
         * Video source may be has different implementation on different platforms. The API is used to generate video source.
         */
        AgoraVideoSource* createVideoSource();
    }
}

#endif