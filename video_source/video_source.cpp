/*
* Copyright (c) 2018 Agora.io
* All rights reserved.
* Proprietary and Confidential -- Agora.io
*/

/*
*  Created by Wang Yongli, 2018
*/

#include "video_source.h"
#include "video_source_log.h"
#include "video_source_event_handler.h"
#include "video_source_render.h"
#include "internal\IAgoraRtcEngine2.h"
#include "video_source_param_parser.h"
#include "video_source_ipc.h"
static const char* s_appid = "0c0b4b61adf94de1befd7cdd78a50444";

#define PROCESS_RUN_EVENT_NAME "agora_video_source_process_ready_event_name"
#define DATA_IPC_NAME "agora_video_source_ipc"

using agora::rtc::RtcEngineContext;
using agora::rtc::uid_t;

AgoraVideoSource::AgoraVideoSource(const std::string& param)
    : m_initialized(false)
    , m_params(param)
    , m_monitorThread()
{
}

AgoraVideoSource::~AgoraVideoSource()
{
    if (m_monitorThread.joinable())
        m_monitorThread.detach();
    m_rtcEngine.reset();
    m_eventHandler.reset();
    m_renderFactory.reset();
    m_ipc.reset();
    m_paramParser.reset();
}

const std::string* AgoraVideoSource::getId()
{
    return m_paramParser->getParameter("id");
}

bool AgoraVideoSource::initialize()
{
    m_paramParser.reset(new VideoSourceParamParser());
    m_paramParser->initialize(m_params);

    const std::string* id = m_paramParser->getParameter("id");
    if (!id || id->empty())
        return false;

    m_ipc.reset(createAgoraIpc(this));
    if (!m_ipc->initialize(*id)){
        return false;
    }
    if (!m_ipc->listen()){
        return false;
    }

    m_ipcSender.reset(new AgoraIpcDataSender());
    if (!m_ipcSender->initialize(*id + DATA_IPC_NAME)){
        return false;
    }

    m_rtcEngine.reset(createAgoraRtcEngine());
    if (!m_rtcEngine.get())
        return false;
    m_eventHandler.reset(new AgoraVideoSourceEventHandler(*this));
    m_renderFactory.reset(new AgoraVideoSourceRenderFactory(*this));
    RtcEngineContext context;
    context.eventHandler = m_eventHandler.get();
    context.appId = s_appid;
    if (m_rtcEngine->initialize(context) != 0){
        log("AgoraVideoSource initialize failed.");
        return false;
    }

    agora::util::AutoPtr<agora::media::IMediaEngine> pMediaEngine;
    pMediaEngine.queryInterface(m_rtcEngine.get(), agora::AGORA_IID_MEDIA_ENGINE);

    if (pMediaEngine.get()){
        pMediaEngine->registerVideoRenderFactory(m_renderFactory.get());
    }
    else{
        log("Get media engine failed.");
        return false;
    }

    m_rtcEngine->disableAudio();
    m_rtcEngine->enableVideo();
    agora::rtc::RtcEngineParameters rep(m_rtcEngine.get());
    rep.enableLocalVideo(false);
    rep.muteAllRemoteVideoStreams(true);
    agora::rtc::VideoCanvas canvas;
    canvas.uid = 0;
    canvas.renderMode = agora::rtc::RENDER_MODE_HIDDEN;
    canvas.view = m_renderFactory.get();
    m_rtcEngine->setupLocalVideo(canvas);
    HANDLE hEvent = OpenEventA(EVENT_MODIFY_STATE, FALSE, (*id + PROCESS_RUN_EVENT_NAME).c_str());
    if (hEvent){
        SetEvent(hEvent);
        CloseHandle(hEvent);
    }
    m_initialized = true;
    return true;
}

void AgoraVideoSource::notifyJoinedChannel(uid_t uid)
{
    m_ipc->sendMessage(AGORA_IPC_JOIN_SUCCESS, (char*)&uid, sizeof(uid));
}

void AgoraVideoSource::notifyRenderReady()
{
    m_ipc->sendMessage(AGORA_IPC_RENDER_READY, nullptr, 0);
}

void AgoraVideoSource::notifyLeaveChannel()
{
    m_ipc->sendMessage(AGORA_IPC_LEAVE_CHANNEL, nullptr, 0);
}

void AgoraVideoSource::notifyRequestNewToken()
{
    m_ipc->sendMessage(AOGRA_IPC_RENEW_TOKEN, nullptr, 0);
}

void AgoraVideoSource::release()
{
    delete this;
}

void AgoraVideoSource::onMessage(unsigned int msg, char* payload, unsigned int len)
{
    if (!m_initialized)
        return;

    if (msg == AGORA_IPC_JOIN){
        if (payload){
            JoinChannelCmd *cmd = (JoinChannelCmd*)payload;
            joinChannel(cmd->token, cmd->cname, cmd->chan_info, cmd->uid);
        }
    }
    else if (msg == AGORA_IPC_CAPTURE_SCREEN){
        if (len != sizeof(CaptureScreenCmd))
            return;
        agora::rtc::RtcEngineParameters rep(m_rtcEngine.get());
        rep.enableLocalVideo(true);
        CaptureScreenCmd *cmd = (CaptureScreenCmd*)payload;
        m_rtcEngine->startScreenCapture(cmd->windowid, cmd->captureFreq, &cmd->rect, cmd->bitrate);
    }
    else if (msg == AGORA_IPC_STOP_CAPTURE_SCREEN){
        m_rtcEngine->stopScreenCapture();
        agora::rtc::RtcEngineParameters rep(m_rtcEngine.get());
        rep.enableLocalVideo(false);
    }
    else if (msg == AOGRA_IPC_RENEW_TOKEN){
        m_rtcEngine->renewToken(payload);
    }
    else if (msg == AGORA_IPC_SET_CHANNEL_PROFILE){
        m_rtcEngine->setChannelProfile((agora::rtc::CHANNEL_PROFILE_TYPE)*payload);
    }
    else if (msg = AGORA_IPC_SET_VIDEO_RPOFILE){
        if (len != sizeof(VideoProfileCmd))
            return;
        VideoProfileCmd *cmd = (VideoProfileCmd*)payload;
        m_rtcEngine->setVideoProfile(cmd->profile, cmd->swapWidthAndHeight);
    }
    else if (msg == AGORA_IPC_DISCONNECT){
        this->exit(false);
    }
}

bool AgoraVideoSource::joinChannel(const char* key, const char* name, const char* chanInfo, agora::rtc::uid_t uid)
{
    return m_rtcEngine->joinChannel(key ? key : "", name ? name : "", chanInfo ? chanInfo : "", uid);
}

void AgoraVideoSource::exit(bool notifySink)
{
    m_ipcSender->Disconnect();
    m_ipc->disconnect();
}

void AgoraVideoSource::monitor()
{
    const std::string* pidstr = m_paramParser->getParameter("pid");
    if (!pidstr || pidstr->empty())
        return;
    HANDLE hProcess = OpenProcess(SYNCHRONIZE, FALSE, std::atoi(pidstr->c_str()));
    if (hProcess){
        if (WaitForSingleObject(hProcess, INFINITE) == WAIT_OBJECT_0) {
            this->exit(false);
        }
    }
}

void AgoraVideoSource::run()
{
    m_monitorThread = std::thread(&AgoraVideoSource::monitor, this);
    m_ipc->run();
}

void AgoraVideoSource::sendData(char* payload, int len)
{
    m_ipcSender->sendData(payload, len);
}

int main(int argc, char* argv[])
{
    if (argc < 3)
        return 0;
    std::string param;
    for (int i = 1; i < argc; i++) {
        param.append(argv[i]);
        param.append(" ");
    }
    auto videoSource = new AgoraVideoSource(param);
    videoSource->initialize();
    videoSource->run();
    videoSource->release();
}