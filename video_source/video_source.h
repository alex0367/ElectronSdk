/*
* Copyright (c) 2018 Agora.io
* All rights reserved.
* Proprietary and Confidential -- Agora.io
*/

/*
*  Created by Wang Yongli, 2018
*/

#ifndef AGORA_VIDEO_SOURCE_H
#define AGORA_VIDEO_SOURCE_H
#include "IAgoraRtcEngine.h"
#include <memory>
#include "video_source_param_parser.h"
#include "video_source_ipc.h"
#include <thread>

using agora::rtc::IRtcEngine;
using agora::util::AutoPtr;
using agora::rtc::uid_t;

class AgoraVideoSourceEventHandler;
class AgoraVideoSourceRenderFactory;
class VideoSourceParamParser;
class AgoraVideoSource;

/**
 * AgoraVideoSource is used to wrap RTC engine and provide IPC functionality.
 */
class AgoraVideoSource : public AgoraIpcListener
{
    /**
     * AgoraVideoSourceEventHandle need to access IPC related functions.
     */
    friend class AgoraVideoSourceEventHandler;
public:
    /**
     * To construct AgoraVideoSource
     * @param : the parameters to construct AgoraVideoSource. It's like 'id:***** pid:****'.Currently id and pid parameters is needed.
     */
    AgoraVideoSource(const std::string& param);
    ~AgoraVideoSource();

    /**
     * To run video source, including following two steps:
     * 1) To start monitor thread, when the sink process exit, video source stopped.
     * 2) To run IPC to monitor cmds from sink
     */
    void run();

    /**
     * Initialize the video source. Must be called before run.
     */
    bool initialize();

    /**
     * After release, the video source object could not be accessed any more.
     */
    void release();

    /**
     * Each video source has one specific Id.
     */
    const std::string* getId();

    /**
     * To process IPC messages.
     */
    virtual void onMessage(unsigned int msg, char* payload, unsigned int len) override;

    /**
     * To send data via IPC
     */
    virtual void sendData(char* payload, int len);
protected:
    bool joinChannel(const char* key, const char* name, const char* chanInfo, agora::rtc::uid_t uid);
    void notifyJoinedChannel(uid_t uid);
    void notifyLeaveChannel();
    void notifyRequestNewToken();
    void notifyRenderReady();
private:
    void monitor();
    void exit(bool notifySink);
private:
    /** Used to process RTC events. */
    std::unique_ptr<AgoraVideoSourceEventHandler> m_eventHandler;
    /** Self video render interface implementation */
    std::unique_ptr<AgoraVideoSourceRenderFactory> m_renderFactory;
    /** Used to parse parameters from sink */
    std::unique_ptr<VideoSourceParamParser> m_paramParser;
    /** Used to provide IPC functionality. */
    std::unique_ptr<IAgoraIpc> m_ipc;
    /** Used to transfer video data */
    std::unique_ptr<AgoraIpcDataSender> m_ipcSender;
    /** RTC engine. */
    AutoPtr<IRtcEngine> m_rtcEngine;
    bool m_initialized;
    std::string m_params;
    /** Used to monitor the sink process */
    std::thread m_monitorThread;
};

#endif