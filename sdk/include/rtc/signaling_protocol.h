//
//  signaling_protocol.h
//  mediasdk
//
//  Created by junhao wang on 04/08/2017.
//  Copyright © 2017 Agora. All rights reserved.
//

#pragma once 

#ifndef signaling_protocol_h
#define signaling_protocol_h

#include <vector>
#include <string>
#include <memory>
#include <map>
#include <base/base_type.h>
#include <packet.h>
#include <type.h>
#include <sstream>

// LBES: live broadcasting edge service

#define SIGNALING_GET_LBES_URI            1
#define SIGNALING_GET_LBES_RES_URI        2
#define SIGNALING_LBES_QUIT_URI				502
#define SIGNALING_LBES_QUIT_RES_URI         503
#define SIGNALING_LBES_PING_URI             504
#define SIGNALING_LBES_PONG_URI             505
#define SIGNALING_LBES_UNPUBLISH_URI        508
#define SIGNALING_LBES_UNPUBLISH_RES_URI    509
#define SIGNALING_LBES_PUBLISH_REQ_URI_v1 512
#define SIGNALING_LBES_PUBLISH_RES_URI_v1 513

#define SIGNALING_LBES_TRANSCODING_UPDATE_REQ_URI_v1 514
#define SIGNALING_LBES_TRANSCODING_UPDATE_RES_URI_v1 515
#define SIGNALING_LBES_ADD_INJECT_STREAM_URL_REQ_URI 516
#define SIGNALING_LBES_ADD_INJECT_STREAM_URL_RES_URI 517
#define SIGNALING_LBES_REMOVE_INJECT_STREAM_URL_REQ_URI 518
#define SIGNALING_LBES_REMOVE_INJECT_STREAM_URL_RES_URI 519
#define SIGNALING_LBES_STREAM_INJECTED_STATUS_RES_URI 520

#define SIGNALING_LBES_PUBLISH_REQ_URI_v2 521
#define SIGNALING_LBES_PUBLISH_RES_URI_v2 522
#define SIGNALING_LBES_TRANSCODING_UPDATE_REQ_URI_v2 523
#define SIGNALING_LBES_TRANSCODING_UPDATE_RES_URI_v2 524

#define SIGNALING_PEER_COMMAND_URI		    600

namespace agora {
    using namespace commons;
    namespace rtc {
        namespace protocol {
    #define name_to_str(name_31415926)  (#name_31415926)

            enum { SIGNALING_SERVER_TYPE = 20};
            
            DECLARE_PACKABLE_5(lbec_detail, std::string, vosIp, std::string, signalingSdkVersion, std::string, sdkVersion, uint32_t, clientRole, std::string, appId);

            DECLARE_PACKET_8(PGetLbesReq, SIGNALING_SERVER_TYPE, SIGNALING_GET_LBES_URI, std::string, sid, seq_t, seq, uint64_t, ts, std::string, cname, uid_t, uid, std::string, url, int32_t, lbecNumber, lbec_detail, detail);

            DECLARE_PACKET_10(PGetLbesRes, SIGNALING_SERVER_TYPE, SIGNALING_GET_LBES_RES_URI, std::string, sid, seq_t, seq, uint32_t, code, uint64_t, server_ts, std::string, cname, uid_t, uid, vid_t, vid, int32_t, lbecNumber, std::string, lbesToken, std::vector<std::string>, addresses);


            DECLARE_PACKET_6(PQuitLbesReq, SIGNALING_SERVER_TYPE, SIGNALING_LBES_QUIT_URI, std::string, sid, seq_t, seq, uint64_t, ts, std::string, cname, uid_t, uid, vid_t, vid);
            DECLARE_PACKET_7(PQuitLbesRes, SIGNALING_SERVER_TYPE, SIGNALING_LBES_QUIT_RES_URI, std::string, sid, seq_t, seq, uint32_t, code, uint64_t, server_ts, std::string, cname, uid_t, uid, uint32_t, vid);


            DECLARE_PACKET_6(PLbesPing, SIGNALING_SERVER_TYPE, SIGNALING_LBES_PING_URI, std::string, sid, seq_t, seq, uint64_t, ts, std::string, cname, uid_t, uid, vid_t, vid);
            DECLARE_PACKET_7(PLbesPong, SIGNALING_SERVER_TYPE, SIGNALING_LBES_PONG_URI, std::string, sid, seq_t, seq, uint32_t, code, uint64_t, server_ts, std::string, cname, uid_t, uid, uint32_t, vid);

            DECLARE_PACKABLE_7(transcoding_user, uid_t, uid, int32_t, x, int32_t, y, int32_t, width, int32_t, height, int32_t, zOrder, double, alpha);

            /*
            DECLARE_PACKABLE_11(transcoding_info, int32_t, width, int32_t, height, int32_t, gop, int32_t, sampleRate, int32_t, framerate, int32_t, bitrate, bool, lowLatency, int32_t, videoCodecProfile, uint32_t, backgroundColor, std::string, userConfigExtraInfo, std::vector<transcoding_user>, userConfigs);
            */
            
            DECLARE_PACKABLE_13(transcoding_info, int32_t, width, int32_t, height, int32_t, videoGop, int32_t, videoFramerate, int32_t, videoCodecProfile, int32_t, videoBitrate, bool, lowLatency, int32_t, audioSampleRate, int32_t, audioBitrate, int32_t, audioChannels, uint32_t, backgroundColor, std::string, userConfigExtraInfo, std::vector<transcoding_user>, userConfigs);

            DECLARE_PACKET_9(PLbesPublishReq_v1, SIGNALING_SERVER_TYPE, SIGNALING_LBES_PUBLISH_REQ_URI_v1, seq_t, seq, std::string, sid, vid_t, vid, std::string, cname, uid_t, uid, uint64_t, ts, std::string, lbesToken, std::string, url, std::unique_ptr<transcoding_info>, transcodingConfig);
            DECLARE_PACKET_9(PLbesPublishRes_v1, SIGNALING_SERVER_TYPE, SIGNALING_LBES_PUBLISH_RES_URI_v1, seq_t, seq, std::string, sid, uint32_t, vid, std::string, cname,uid_t, uid, uint64_t, server_ts, std::string, url, uint32_t, code, std::string, reason);

            DECLARE_PACKET_7(PLbesUnpublishReq, SIGNALING_SERVER_TYPE, SIGNALING_LBES_UNPUBLISH_URI, std::string, sid, seq_t, seq, uint64_t, ts, std::string, cname, uid_t, uid, vid_t, vid, std::string, url);
            DECLARE_PACKET_9(PLbesUnpublishRes, SIGNALING_SERVER_TYPE, SIGNALING_LBES_UNPUBLISH_RES_URI, std::string, sid, seq_t, seq, uint32_t, code, uint64_t, server_ts, std::string, cname, uid_t, uid, uint32_t, vid, std::string, url, std::string, reason);

            DECLARE_PACKET_7(PLbesTranscodingUpdateReq_v1, SIGNALING_SERVER_TYPE, SIGNALING_LBES_TRANSCODING_UPDATE_REQ_URI_v1, seq_t, seq, std::string, sid, vid_t, vid, std::string, cname, uid_t, uid, uint64_t, ts, std::unique_ptr<transcoding_info>, transcodingConfig);
            DECLARE_PACKET_8(PLbesTranscodingUpdateRes_v1, SIGNALING_SERVER_TYPE, SIGNALING_LBES_TRANSCODING_UPDATE_RES_URI_v1, seq_t, seq, std::string, sid, uint32_t, vid, std::string, cname, uid_t, uid, uint64_t, server_ts, uint32_t, code, std::string, reason);

            DECLARE_PACKET_5(PPeerCommandMessage, SIGNALING_SERVER_TYPE, SIGNALING_PEER_COMMAND_URI, int32_t, command, seq_t, seq, uint64_t, ts, uid_t, uid, uint32_t, response);

            DECLARE_PACKABLE_8(PInjectStreamConfig, int32_t, width, int32_t, height, int32_t, videoGop, int32_t, videoFramerate, int32_t, videoBitrate, int32_t, audioSampleRate, int32_t, audioBitrate, int32_t, audioChannels);
            DECLARE_PACKET_9(PLbesAddInjectStreamUrlReq, SIGNALING_SERVER_TYPE, SIGNALING_LBES_ADD_INJECT_STREAM_URL_REQ_URI, seq_t, seq, std::string, sid, vid_t, vid, std::string, cname, uid_t, uid, uint64_t, ts, std::string, lbesToken, std::string, url, PInjectStreamConfig, transcodingConfig);
            DECLARE_PACKET_10(PLbesAddInjectStreamUrlRes, SIGNALING_SERVER_TYPE, SIGNALING_LBES_ADD_INJECT_STREAM_URL_RES_URI, seq_t, seq, std::string, sid, uint32_t, vid, std::string, cname, uid_t, uid, uint64_t, server_ts, std::string, url, uint32_t, code, std::string, reason, uid_t, inject_uid);
            DECLARE_PACKET_7(PLbesRemoveInjectStreamUrlReq, SIGNALING_SERVER_TYPE, SIGNALING_LBES_REMOVE_INJECT_STREAM_URL_REQ_URI, seq_t, seq, std::string, sid, vid_t, vid, std::string, cname, uid_t, uid, uint64_t, ts, std::string, url);
            DECLARE_PACKET_10(PLbesRemoveInjectStreamUrlRes, SIGNALING_SERVER_TYPE, SIGNALING_LBES_REMOVE_INJECT_STREAM_URL_RES_URI, seq_t, seq, std::string, sid, uint32_t, vid, std::string, cname, uid_t, uid, uint64_t, server_ts, uint32_t, code, std::string, reason, std::string, url, uid_t, inject_uid);
            DECLARE_PACKET_10(PLbesStreamInjectedStatusRes, SIGNALING_SERVER_TYPE, SIGNALING_LBES_STREAM_INJECTED_STATUS_RES_URI, seq_t, seq, std::string, sid, uint32_t, vid, std::string, cname, uid_t, uid, uint64_t, server_ts, std::string, url, uint32_t, code, std::string, reason, uid_t, inject_uid);

            DECLARE_PACKABLE_8(PTranscodingUserConfig, uid_t, uid, int32_t, x, int32_t, y, int32_t, width, int32_t, height, int32_t, zOrder, double, alpha, int32_t, audioChannel);
            DECLARE_PACKABLE_13(PTranscodingConfig, int32_t, width, int32_t, height, int32_t, videoGop, int32_t, videoFramerate, int32_t, videoCodecProfile, int32_t, videoBitrate, bool, lowLatency, int32_t, audioSampleRate, int32_t, audioBitrate, int32_t, audioChannels, uint32_t, backgroundColor, std::string, userConfigExtraInfo, std::vector<PTranscodingUserConfig>, userConfigs);
            DECLARE_PACKET_9(PLbesPublishReq_v2, SIGNALING_SERVER_TYPE, SIGNALING_LBES_PUBLISH_REQ_URI_v2, seq_t, seq, std::string, sid, vid_t, vid, std::string, cname, uid_t, uid, uint64_t, ts, std::string, lbesToken, std::string, url, std::unique_ptr<PTranscodingConfig>, transcodingConfig);
            DECLARE_PACKET_9(PLbesPublishRes_v2, SIGNALING_SERVER_TYPE, SIGNALING_LBES_PUBLISH_RES_URI_v2, seq_t, seq, std::string, sid, uint32_t, vid, std::string, cname,uid_t, uid, uint64_t, server_ts, std::string, url, uint32_t, code, std::string, reason);
            DECLARE_PACKET_7(PLbesTranscodingUpdateReq_v2, SIGNALING_SERVER_TYPE, SIGNALING_LBES_TRANSCODING_UPDATE_REQ_URI_v2, seq_t, seq, std::string, sid, vid_t, vid, std::string, cname, uid_t, uid, uint64_t, ts, std::unique_ptr<PTranscodingConfig>, transcodingConfig);
            DECLARE_PACKET_8(PLbesTranscodingUpdateRes_v2, SIGNALING_SERVER_TYPE, SIGNALING_LBES_TRANSCODING_UPDATE_RES_URI_v2, seq_t, seq, std::string, sid, uint32_t, vid, std::string, cname, uid_t, uid, uint64_t, server_ts, uint32_t, code, std::string, reason);

            typedef PLbesPublishReq_v2 PLbesPublishReq;
            typedef PLbesPublishRes_v2 PLbesPublishRes;
            typedef PLbesTranscodingUpdateReq_v2 PLbesTranscodingUpdateReq;
            typedef PLbesTranscodingUpdateRes_v2 PLbesTranscodingUpdateRes;
#define SIGNALING_LBES_PUBLISH_REQ_URI SIGNALING_LBES_PUBLISH_REQ_URI_v2
#define SIGNALING_LBES_PUBLISH_RES_URI SIGNALING_LBES_PUBLISH_RES_URI_v1
#define SIGNALING_LBES_TRANSCODING_UPDATE_REQ_URI SIGNALING_LBES_TRANSCODING_UPDATE_REQ_URI_v2
#define SIGNALING_LBES_TRANSCODING_UPDATE_RES_URI SIGNALING_LBES_TRANSCODING_UPDATE_RES_URI_v2
        }
    }
}
#endif /* signaling_protocol_h */
