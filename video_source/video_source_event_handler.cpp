/*
* Copyright (c) 2018 Agora.io
* All rights reserved.
* Proprietry and Confidential -- Agora.io
*/

/*
*  Created by Wang Yongli, 2018
*/

#include "video_source_event_handler.h"
#include "video_source_log.h"

AgoraVideoSourceEventHandler::AgoraVideoSourceEventHandler(AgoraVideoSource& videoSource)
    : m_videoSource(videoSource)
{

}

AgoraVideoSourceEventHandler::~AgoraVideoSourceEventHandler()
{}

void AgoraVideoSourceEventHandler::onJoinChannelSuccess(const char* channel, uid_t uid, int elapsed)
{
    log(("%s, channel :%s, uid : %d, elapsed :%d", __FUNCTION__, channel, uid, elapsed));
    m_videoSource.notifyJoinedChannel(uid);
}

void AgoraVideoSourceEventHandler::onRejoinChannelSuccess(const char* channel, uid_t uid, int elapsed)
{
    log(("%s, channel :%s, uid: %d, elapsed :%d", __FUNCTION__, channel, uid, elapsed));
    m_videoSource.notifyJoinedChannel(uid);
}

void AgoraVideoSourceEventHandler::onWarning(int warn, const char* msg)
{
    log(("%s, warn :%d, msg :%s", __FUNCTION__, warn, msg));
}

void AgoraVideoSourceEventHandler::onError(int err, const char* msg)
{
    log(("%s, err : %d, msg :%s", __FUNCTION__, err, msg));
}

void AgoraVideoSourceEventHandler::onLeaveChannel(const RtcStats& stats)
{
    log("%s", __FUNCTION__);
    m_videoSource.notifyLeaveChannel();
}

void AgoraVideoSourceEventHandler::onRtcStats(const RtcStats& stats)
{
    log("%s", __FUNCTION__);
}

void AgoraVideoSourceEventHandler::onVideoDeviceStateChanged(const char* deviceId, int deviceType, int deviceState)
{
    log(("%s, deviceId :%s, deviceType :%d, deviceStatus :%d", __FUNCTION__, deviceId, deviceType, deviceState));
}

void AgoraVideoSourceEventHandler::onNetworkQuality(uid_t uid, int txQuality, int rxQuality)
{
    log(("%s, uid :%d, txQuality :%d, rxQuality:%d", __FUNCTION__, uid, txQuality, rxQuality));
}

void AgoraVideoSourceEventHandler::onLastmileQuality(int quality)
{
    log(("%s, quality :%d", __FUNCTION__, quality));
}

void AgoraVideoSourceEventHandler::onFirstLocalVideoFrame(int width, int height, int elapsed)
{
    log(("%s, width :%d, height :%d, elapsed: %d", __FUNCTION__, width, height, elapsed));
}

void AgoraVideoSourceEventHandler::onVideoSizeChanged(uid_t uid, int width, int height, int rotation)
{
    log(("%s, uid :%d, width :%d, height:%d, rotation :%d", __FUNCTION__, uid, width, height, rotation));
}

void AgoraVideoSourceEventHandler::onApiCallExecuted(int err, const char* api, const char* result)
{
    log(("%s, err :%d, api :%s, result :%s", __FUNCTION__, err, api, result));
}

void AgoraVideoSourceEventHandler::onLocalVideoStats(const LocalVideoStats& stats)
{
    log(("%s", __FUNCTION__));
}

void AgoraVideoSourceEventHandler::onCameraReady()
{
    log(("%s", __FUNCTION__));
}

void AgoraVideoSourceEventHandler::onCameraFocusAreaChanged(int x, int y, int width, int height)
{
    log(("%s, x :%d, y:%d, width:%d, heigh:%d", __FUNCTION__, x, y, width, height));
}

void AgoraVideoSourceEventHandler::onVideoStopped()
{
    log(("%s", __FUNCTION__));
}

void AgoraVideoSourceEventHandler::onConnectionLost()
{
    log(("%s", __FUNCTION__));
}

void AgoraVideoSourceEventHandler::onConnectionInterrupted()
{
    log(("%s", __FUNCTION__));
}

void AgoraVideoSourceEventHandler::onConnectionBanned()
{
    log(("%s", __FUNCTION__));
}

void AgoraVideoSourceEventHandler::onRefreshRecordingServiceStatus(int status)
{
    log(("%s, status :%d", __FUNCTION__, status));
}

void AgoraVideoSourceEventHandler::onRequestToken()
{
    log(("%s", __FUNCTION__));
    m_videoSource.notifyRequestNewToken();
}

void AgoraVideoSourceEventHandler::onStreamPublished(const char *url, int error)
{
    log(("%s, url :%s, error :%d", __FUNCTION__, url, error));
}

void AgoraVideoSourceEventHandler::onStreamUnpublished(const char* url)
{
    log(("%s, url :%s", __FUNCTION__, url));
}

void AgoraVideoSourceEventHandler::onTranscodingUpdated()
{
    log(("%s", __FUNCTION));
}

void AgoraVideoSourceEventHandler::onPublishingRequestAnswered(uid_t owner, int response, int error)
{
    log(("%s, uid :%d, response :%d, error :%d", __FUNCTION__, owner, resposne, error));
}

void AgoraVideoSourceEventHandler::onPublishingRequestReceived(uid_t uid)
{
    log(("%s, uid :%d", __FUNCTION__, uid));
}

void AgoraVideoSourceEventHandler::onUnpublishingRequestReceived(uid_t owner)
{
    log(("%s, uid :%d", __FUNCTION__, owner));
}

void AgoraVideoSourceEventHandler::onStreamInjectedStatus(const char* url, uid_t uid, int status)
{
    log(("%s, url :%s, uid :%d, status :%d", __FUNCTION__, url, uid, status));
}