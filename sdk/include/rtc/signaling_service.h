//
//  SignalingService.hpp
//  mediasdk
//
//  Created by junhao wang on 04/08/2017.
//  Copyright © 2017 Agora. All rights reserved.
//

#ifndef SignalingService_hpp
#define SignalingService_hpp

#include <stdio.h>
#include <io_engine_base.h>
#include <event_dispatcher.h>
#include <sigslot.h>
#include "agora_api.h"
#include "rtc/signaling_protocol.h"

namespace agora {
namespace rtc {
    
namespace protocol {
    struct CmdTranscoding;
    struct transcoding_info;
}

class RtcContext;

class SignalingService : public sigslot::has_slots<>, public agora_sdk::ICallBack
{
    using task_type = std::function<void(void)>;
    typedef agora::commons::async_queue_base<task_type, any_document_t> async_queue_type;

public:
    SignalingService(RtcContext &ctx);
    virtual ~SignalingService();
    int join();
    int leave();
    
    void onNetworkChanged(bool ipLayerChanged, int oldNetworkType, int newNetworkType);

    bool joined() { return m_joined; }
    
private:
    void sendPeerMessage(uid_t peerUid, const std::string &payload);
    void onReceivePeerMessage(const std::string &payload);
    
    void resetState();
public:
    // login
    virtual void onLoginSuccess(uint32_t uid, int fd);
    virtual void onLogout(int ecode);
    virtual void onLoginFailed(int ecode);
    virtual void onLog(std::string txt);
    virtual void onError(std::string name, int ecode, std::string desc);

    //message
    virtual void onMessageSendError(std::string messageID, int ecode);
    virtual void onMessageInstantReceive(std::string account, uint32_t uid, std::string msg);

    virtual void onBCCall_result(std::string reason, std::string json_ret, std::string callID);

    void bc_call(const std::string &func, const std::string &json, const std::string &callId);
    int sendInstantMessage(const std::string &account, const std::string &payload, const std::string &msgId);
    std::string getAccount(std::string channelName, uid_t uid);
private:
    RtcContext& m_context;
    agora_sdk::IAgoraAPI* m_signalingLink;
    std::unique_ptr<async_queue_type> m_sigQueue;
    
    bool m_joined;
};
}
}




#endif /* SignalingService_hpp */
