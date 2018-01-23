//
//  Agora Media SDK
//
//  Created by Sting Feng in 2015-05.
//  Copyright (c) 2015 Agora IO. All rights reserved.
//
#pragma once
#include <packet.h>
#include <base/base_type.h>
#include <list>
#include <string>
#include <vector>

#define CMD_START_SERVICE_URI 1
#define CMD_STOP_SERVICE_URI 2
#define CMD_JOIN_CHANNEL_URI 3
#define CMD_LEAVE_CHANNEL_URI 4
#define CMD_SET_PARAMETERS_URI 5
#define CMD_GET_PARAMETERS_URI 6
#define CMD_NETWORK_TEST_URI 7
#define CMD_LASTMILE_TEST_URI 8
#define CMD_ECHO_TEST_URI 9
#define CMD_CALL_RATE_URI 10
#define CMD_SET_PROFILE_URI 11
#define CMD_GET_PROFILE_URI 12
#define CMD_NETWORK_INFO_URI 13
#define CMD_SET_REMOTE_CANVAS_URI 14
#define CMD_VENDOR_MESSAGE_URI 15
#define CMD_REPORT_MESSAGE_URI 16
#define CMD_WEBAGENT_VIDEO_STATS_URI 17

#if defined(__ANDROID__)
#define CMD_UPDATE_SHARED_CONTEXT 18
#define CMD_SET_TEXTURE_ID 19
#endif

#define CMD_VIDEO_COMPOSITING_LAYOUT_URI 20
#define CMD_REPORT_ARGUS_COUNTERS_URI 21
#define CMD_VIDEO_PROFILE_EX 22

#define CMD_TRANSCODING_URI 23
#define CMD_PUBLISH_URL_URI 24
#define CMD_INJECT_STREAM_CONFIG_URI 25

namespace agora {
	using namespace commons;
	namespace rtc {
		namespace protocol {
			enum { CMD_SERVER_TYPE = 0 };

			DECLARE_PACKET_3(CmdStartService, CMD_SERVER_TYPE,CMD_START_SERVICE_URI, std::string, configDir, std::string, dataDir, std::string, pluginDir);
			DECLARE_PACKET_0(CmdStopService, CMD_SERVER_TYPE,CMD_STOP_SERVICE_URI);
            DECLARE_SIMPLE_STRUCT_5(CmdJoinChannel, uint64_t, tick0, std::string, appId, std::string, channel, std::string, info, uint32_t, uid);
			DECLARE_PACKET_0(CmdLeaveChannel, CMD_SERVER_TYPE,CMD_LEAVE_CHANNEL_URI);
			DECLARE_PACKET_1(CmdSetParameters, CMD_SERVER_TYPE,CMD_SET_PARAMETERS_URI, std::string, parameters);
			DECLARE_PACKET_0(CmdGetProfile, CMD_SERVER_TYPE,CMD_GET_PROFILE_URI);
			DECLARE_PACKET_2(CmdSetProfile, CMD_SERVER_TYPE, CMD_SET_PROFILE_URI, std::string, profile, uint8_t, merge);
			DECLARE_PACKET_1(CmdGetParameters, CMD_SERVER_TYPE, CMD_GET_PARAMETERS_URI, std::string, parameters);
			DECLARE_PACKET_13(CmdNetworkInfo, CMD_SERVER_TYPE, CMD_NETWORK_INFO_URI, std::string, localIp4, std::string, gatewayIp4, std::string, localIp6, std::string, gatewayIp6, std::vector<std::string>, localIp6List, std::vector<std::string>, dnsList, int, networkType, int, networkSubtype, int, level, int, rssi, int, asu, std::string, ssid, std::string, bssid);
#if defined(__ANDROID__)
            DECLARE_PACKET_1(CmdUpdateSharedContext, CMD_SERVER_TYPE, CMD_UPDATE_SHARED_CONTEXT, void *, eglContext);
            DECLARE_PACKET_5(CmdSetTextureId, CMD_SERVER_TYPE, CMD_SET_TEXTURE_ID, int, id, void *, eglContext, int, width, int, height, int64_t, ts);
#endif
#if defined(ENABLE_NTEST)
			DECLARE_PACKET_3(CmdNetworkTest, CMD_SERVER_TYPE, CMD_NETWORK_TEST_URI, uint8_t, enable, int32_t, interval, uint32_t, kbps);
#endif
			DECLARE_PACKET_1(CmdEchoTest, CMD_SERVER_TYPE, CMD_ECHO_TEST_URI, uint8_t, enable);
			DECLARE_PACKET_3(CmdCallRate, CMD_SERVER_TYPE, CMD_CALL_RATE_URI, std::string, callId, int, rating, std::string, description);
			DECLARE_PACKET_3(CmdSetVideoCanvas, CMD_SERVER_TYPE, CMD_SET_REMOTE_CANVAS_URI, uid_t, uid, void *, viewRef, int, renderMode);
            DECLARE_SIMPLE_STRUCT_3(CmdStreamMessage, stream_id_t, streamId, uint32_t, seq, std::string, message);
			DECLARE_PACKET_2(CmdReportMessage, CMD_SERVER_TYPE, CMD_REPORT_MESSAGE_URI, std::string, message, int, type);
            DECLARE_SIMPLE_STRUCT_5(CmdWebAgentVideoStats, uid_t, uid, int, delay, int, sentFrameRate, int, renderedFrameRate, int, skippedFrames);
            DECLARE_PACKABLE_8(PSeiRegion, uid_t, uid, double, x, double, y, double, width, double, height, int, z, double, alpha, int, renderMode);
            DECLARE_PACKET_5(CmdSeiLayout, CMD_SERVER_TYPE, CMD_VIDEO_COMPOSITING_LAYOUT_URI, int, canvasWidth, int, canvasHeight, std::string, backgroundColor, std::string, appData, std::vector<PSeiRegion>, regions);

#if defined(FEATURE_LIVE_SDK)
            DECLARE_PACKABLE_8(PTranscodingUser, uid_t, uid, int32_t, x, int32_t, y, int32_t, width, int32_t, height, int32_t, zOrder, double, alpha, int32_t, audioChannel);
            DECLARE_PACKET_13(CmdTranscoding, CMD_SERVER_TYPE, CMD_TRANSCODING_URI, int32_t, width, int32_t, height, int32_t, videoGop, int32_t, videoFramerate, int32_t, videoCodecProfile, int32_t, videoBitrate, bool, lowLatency, int32_t, audioSampleRate, int32_t, audioBitrate, int32_t, audioChannels, uint32_t, backgroundColor, std::string, userConfigExtraInfo, std::vector<PTranscodingUser>, userConfigs);
            DECLARE_PACKET_2(CmdPublishUrl, CMD_SERVER_TYPE, CMD_PUBLISH_URL_URI, std::string, url, bool, transcodingEnabled);
            DECLARE_PACKET_8(CmdInjectStreamConfig, CMD_SERVER_TYPE, CMD_INJECT_STREAM_CONFIG_URI, int32_t, width, int32_t, height, int32_t, videoGop, int32_t, videoFramerate, int32_t, videoBitrate, int32_t, audioSampleRate, int32_t, audioBitrate, int32_t, audioChannels);
#endif

            DECLARE_SIMPLE_STRUCT_2(PCounterItem, int, counterId, int, value);
            DECLARE_SIMPLE_STRUCT_2(CmdReportArgusCounters, uid_t, uid, std::vector<PCounterItem>, counters);
            DECLARE_SIMPLE_STRUCT_4(CmdSetVideoProfileEx, int, width, int, height, int, frameRate, int, bitrate);

			//video stats
			DECLARE_PACKABLE_7(PLocalVideoStream, uint16_t, width, uint16_t, height, int, preFrameNumber, int, bitrate, int, frameRate, int, sentQP, int, packetRate);
            DECLARE_PACKABLE_14(PLocalVideoExtraStat, uint16_t, duration, uint16_t, captureWidth, uint16_t, captureHeight, uint16_t, captureFrames, uint16_t, encoderInFrames, uint16_t, encoderOutFrames, uint16_t, encoderFailedFrames, uint16_t, encoderSkipFrames, uint16_t, encodeTimeMs, uint16_t, uplinkFreezeCount, uint16_t, uplinkFreezeTime, uint32_t, bitFieldStates, uint16_t, cameraOpenStatus, uint16_t, captureType);
			DECLARE_PACKABLE_9(PLocalVideoStat, PLocalVideoStream, high, PLocalVideoStream, low, int, sentRtt, int, sentLoss, int, sentTargetBitRate, int, fecLevel, int, estimateBandwidth, unsigned int, maxFrameOutInterval, PLocalVideoExtraStat, extra);
            DECLARE_PACKABLE_12(PRemoteVideoExtraStat2, int32_t, duration, uint16_t, rxPackets, uint16_t, decodeFailedFrames, uint16_t, decoderOutFrames, uint16_t, rendererInFrames, uint16_t, rendererOutFrames, uint16_t, decodeTimeMs, uint16_t, decoderInFrameRate, uint16_t, decoderOutFrameRate, uint16_t, decodeQP, uint16_t, renderFreezeCount, uint16_t, renderFreezeTime);
			DECLARE_PACKABLE_9(PRemoteVideoExtraStat, uint16_t, width, uint16_t, height, int, lossAfterFec, int, bytes, uint32_t, flags, int, decodeFailedFrames, int, decodeRejectedFrames, unsigned int, maxRenderInterval, int, streamType);
			DECLARE_PACKABLE_8(PRemoteVideoStat, uid_t, uid, int, delay, int, bitrate, int, frameRate, int, packetRate, PRemoteVideoExtraStat, extra, PRemoteVideoExtraStat2, extra2, int, renderType);
			DECLARE_PACKABLE_2(PVideoStats, PLocalVideoStat, local, std::list<PRemoteVideoStat>, remotes);
		}
}
}
