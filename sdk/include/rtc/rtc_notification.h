//
//  Agora Media SDK
//
//  Created by Sting Feng in 2015-05.
//  Copyright (c) 2015 Agora IO. All rights reserved.
//
#pragma once
#include <string>
#include <sigslot.h>
#include <memory>
#include <internal/rtc_engine_i.h>
#include <base/base_context.h>

namespace agora {
	namespace commons {
		struct perf_counter_data;
	}
	namespace rtc {
		class RtcContext;
		class AsyncRtcEventHandler;
		namespace protocol {
			struct PAudioReport_v4;
			struct simple_receive_stat_data;
			struct PVideoStats;
		}
		namespace signal {
			struct VocsEventData;
			struct VosEventData;
            struct DataStreamEventData;
		}

class RtcEngineNotification : public agora::base::INotification, public sigslot::has_slots<>
{
public:
	struct AudioQuality {
		uid_t uid;
		int quality;
		uint16_t delay;
		uint16_t lost;
		uint16_t ka;
	};
	enum class ChannelState {
		INITIAL = 0,
		JOINED = 1,
		REJOINED = 2,
		INTERRUPTED = 3,
	};
public:
	RtcEngineNotification(RtcContext& ctx, IRtcEngineEventHandler* eh, bool isExHandler);
	~RtcEngineNotification();
	void stopAsyncHandler(bool waitForExit);
    void onCallContextCreated();
	void onStartCall();
	void onEndCall();
	void suppressNotification(bool suppressed) { m_notificationSuppressed = suppressed; }
	bool isNotificationSuppressed() const;
	bool getQueuePerfCounters(commons::perf_counter_data& counters) const;
	IRtcEngineEventHandler* getEventHandler();
	bool isExHandler() const;
public:
    virtual void suppressApiCallNotification(bool suppressed) override { m_apiCallSuppressed = suppressed; }
    virtual bool isApiCallNotificationSuppressed() const override { return m_apiCallSuppressed; }
    virtual void onApiCallExecuted(int err, const char* api, const char* result) override;
    void onJoinedChannel();
	void onVocsEvent(const signal::VocsEventData& ed);
	void onVosEvent(const signal::VosEventData& ed);
	void onLeaveChannel(const RtcStats& stat);
	void onStopEchoTest();
	void onRtcStats(const RtcStats& stat);
	void onWarning(int warn, const char* msg = nullptr);
	void onError(int err, const char* msg = nullptr);
	void onEvent(RTC_EVENT code, const char* msg = nullptr);
	void onLogging(int level, const char* msg);
	void onAudioStat(const protocol::PAudioReport_v4& stat);
	void onListenerStat(bool audio, uid_t peerUid, const protocol::simple_receive_stat_data& stat);
	void onNetworkStat(uid_t uid, int txQuality, int rxQuality);
	void onSignalStrenth(int quality);
	void onPeerJoined(uid_t uid, int elapsed);
	void onPeerOffline(uid_t uid, int reason);
	void onPeerMuteAudio(uid_t uid, bool muted);
	void onPeerMuteVideo(uid_t uid, bool muted);
    void onPeerEnableVideo(uid_t uid, bool enabled);
    void onPeerEnableLocalVideo(uid_t uid, bool enabled);
	void onRecap(const char* data, size_t length);
	void onAudioVolumeIndication(const AudioVolumeInfo* speakers, int count, int totalVolume);
    void onActiveSpeaker(uid_t uid);
	void onFirstVideoFrame(
        uid_t uid, int width, int height, bool local, int elapsed);
	void onFirstFrameDecoded(uid_t uid, int width, int height, int elapsed);
    void onCameraFocusAreaChanged(int x, int y, int width, int height);
    void onVideoSizeChanged(uid_t uid, int width, int height, int rotation);
	void onVideoStats(const protocol::PVideoStats& stats);
	void onConnectionLost();
	void onConnectionInterrupted();
    void onConnectionBanned();
	void onChatEngineEvent(int evt);
	void onAudioDeviceStateChange(const char* deviceId, int deviceType, int newState);
	void onVideoDeviceStateChange(const char* deviceId, int deviceType, int newState);
    void onAudioEffectFinished(int soundId);
	void onRefreshRecordingServiceStatus(int status);
    void onStreamMessage(uid_t uid, stream_id_t streamId, const char* data, size_t length);
    void onStreamMessageError(const signal::DataStreamEventData& evt);
	void onVideoDisabled();
    void onFirstLocalAudioFrame(int elapsed);
    void onFirstRemoteAudioFrame(uid_t uid, int elapsed);
    void onClientRoleChanged(CLIENT_ROLE_TYPE oldRole, CLIENT_ROLE_TYPE newRole);
#if defined(FEATURE_LIVE_SDK)
    void onStreamPublished(const std::string &url, int error);
    void onStreamUnpublished(const std::string &url);
    void onTranscodingUpdate();
    void onJoinPublisherResponse(uid_t owner, int response, int error);
    void onJoinPublisherRequest(uid_t uid);
    void onPublisherRemoved(uid_t owner);
    void onStreamInjectedStatus(const std::string& url, uid_t uid, int status);
#endif

    void onAudioDeviceVolumeChanged(MEDIA_DEVICE_TYPE deviceType, int volume, bool muted);
#ifdef INTERNAL_ENGINE_STATUS
    void onInternalEngineStatus(InternalEngineStatus state);
#endif
    bool registerEventHandler(IRtcEngineEventHandler *eh, bool isExHandler);
    bool unregisterEventHandler(IRtcEngineEventHandler *eh);
private:
	RtcContext& m_context;
	std::unique_ptr<AsyncRtcEventHandler> m_eh;
	ChannelState m_channelState;
	bool m_apiCallSuppressed;
	bool m_notificationSuppressed;
};

}}
