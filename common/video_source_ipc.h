/*
* Copyright (c) 2018 Agora.io
* All rights reserved.
* Proprietary and Confidential -- Agora.io
*/

/*
*  Created by Wang Yongli, 2018
*/

/**
 * the file define classes used to deliver command and event between node ADDON and video source process.
 */

#ifndef AGORA_VIDEO_SOURCE_IPC_H
#define AGORA_VIDEO_SOURCE_IPC_H
#include <string>
#include <vector>
#include "IAgoraRtcEngine.h"
#include "ipc_shm.h"
#include <functional>

/**
 * AgoraIpcMsg define the message type transferred between node ADDON and vidoe source process
 */
enum AgoraIpcMsg
{
    /**  obsolete*/
    AGORA_IPC_CONNECT = 1,
    /** obsolete  */
    AGORA_IPC_CONNECT_CONFIRM,
    /** obsolete  */
    AGORA_IPC_DISCONNECT,
    /** To notify video source to join channel */
    AGORA_IPC_JOIN,
    /** Video source ==> node ADDON join success event */
    AGORA_IPC_JOIN_SUCCESS,
    /** Node ADDON ==> video source to begin capture screen */
    AGORA_IPC_CAPTURE_SCREEN,
    /** Node ADDON ==> video source to update screen sharing area*/
    AGORA_IPC_UPDATE_CAPTURE_SCREEN,
    /** Node ADDON ==> video source to stop capture screen */
    AGORA_IPC_STOP_CAPTURE_SCREEN,
    /** Node ADDON ==> video source to start video */
    AGORA_IPC_START_CAMERA,
    /** Node ADDON ==> video source to update token */
    AOGRA_IPC_RENEW_TOKEN,
    /** Node ADDON ==> video source to set channel profile */
    AGORA_IPC_SET_CHANNEL_PROFILE,
    /** Node ADDON ==> video source to set video profile */
    AGORA_IPC_SET_VIDEO_RPOFILE,
    /** Node ADDON ==> video source to leave channel */
    AGORA_IPC_LEAVE_CHANNEL,
    /** video source render is ready */
    AGORA_IPC_RENDER_READY
};

/**
 * Screen capture parameters when ADDON ask video source to start screen sharing.
 */
struct CaptureScreenCmd
{
    agora::rtc::IRtcEngine::WindowIDType windowid;
    int captureFreq;
    agora::rtc::Rect rect;
    int bitrate;
};

#define MAX_TOKEN_LEN 128
#define MAX_CNAME_LEN 256
#define MAX_CHAN_INFO 512
/**
 * Join channel parameters when ADDON ask video source to join channel
 */
struct JoinChannelCmd
{
    char token[128];
    char cname[256];
    char chan_info[512];
    agora::rtc::uid_t uid;
};

/**
 * video profile parameters when ADDON ask video source to set video profile
 */
struct VideoProfileCmd
{
    agora::rtc::VIDEO_PROFILE_TYPE profile;
    bool swapWidthAndHeight;
};

/**
 * AgoraIpcListener is used to monitor IPC message
 */
class AgoraIpcListener
{
public:
    virtual ~AgoraIpcListener(){}
    virtual void onMessage(unsigned int msg, char* payload, unsigned int len)
    {
        (void)payload;
        (void)len;
    }
};

/**
 * IAgoraIpc is used to facilitate communications between processes. This is one virtual class, may have different implementation on different platforms.
 */
class IAgoraIpc
{
public:
    IAgoraIpc(AgoraIpcListener* listener)
        : m_listener(listener)
    {}
    virtual ~IAgoraIpc(){}
    virtual const std::string& getId(){ return m_id; }
    /**
     * To initialize IPC.
     * @param id : the id used to identify the IPC endpoint.
     */
    virtual bool initialize(const std::string& id) = 0;
    /**
     * To put IPC endpoint in listen state, then other endpoint can connect the endpoint.
     */
    virtual bool listen() = 0;
    /**
     * To connect to other IPC endpoint.
     */
    virtual bool connect() = 0;
    /**
     * To disconnect the IPC
     */
    virtual bool disconnect() = 0;
    /**
     * To start IPC.
     */
    virtual void run() = 0;
    /**
     * To send message.
     * @param msg : the message id.
     * @param payload : the pointer to the transferred data.
     * @param len : the length of the data payload.
     */
    virtual bool sendMessage(AgoraIpcMsg msg, char* payload, unsigned int len) = 0;
protected:
    std::string m_id;
    AgoraIpcListener *m_listener;
};

/**
 * The class is used for IPC with large throughput. 
 * AgoraIpcDataSender provide send-only facilities.
 */
#define DATA_DELIVER_BLOCK_SIZE 3110450
class AgoraIpcDataSender
{
public:
    AgoraIpcDataSender();
    ~AgoraIpcDataSender();

    /**
     * To initialize the AgoraIpcDataSender.
     * @param id : sender identifier.
     */
    bool initialize(const std::string& id);
    /**
     * To send data.
     * @param payload : the pointer to data to be sent.
     * @param len : the length of the data.
     */
    void sendData(char* payload, unsigned int len);
    /**
     * To send multiple data in one time.
     * @payloads : vector of data to be sent.
     */
    void sendMultiData(const std::vector<std::pair<char*, int32_t>>& payloads);

    /** Disconnect the sender and IPC */
    void Disconnect();
private:
    shm_ipc<1, DATA_DELIVER_BLOCK_SIZE> m_ipcData;
    bool m_initialized;
    std::string m_id;
};

/**
 * The class is used to IPC with large throughput.
 * The AgoraIpcDataReceiver provide receive-only facilities.
 */
class AgoraIpcDataReceiver
{
public:
    AgoraIpcDataReceiver();
    ~AgoraIpcDataReceiver();

    /**
     * To initialize AgoraIpcDataReciever
     * @param id : IPC identifier
     * @param handler : the receiver event handler.
     */
    bool initialize(const std::string& id, const std::function<void(const char*, int)>& handler);
    /**
     * To start the IPC.
     */
    void run();
private:
    std::function<void(const char*, int)> m_handler;
    std::string m_id;
    bool m_initialized;
    shm_ipc<1, DATA_DELIVER_BLOCK_SIZE> m_ipcData;
    char *m_buf;
};

/**
 * To create IAgroaIpc instance.
 */
IAgoraIpc* createAgoraIpc(AgoraIpcListener *listner);

#endif