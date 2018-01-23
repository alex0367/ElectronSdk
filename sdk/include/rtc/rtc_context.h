//
//  Agora Media SDK
//
//  Created by Sting Feng in 2015-05.
//  Copyright (c) 2015 Agora IO. All rights reserved.
//
#pragma once
#include <cassert>
#include <unordered_map>
#include <util.h>
#include <io_engine.h>
#include <base/base_type.h>
#include <base/base_context.h>
#include <base/config_engine.h>
#include <rtc/rtc_notification.h>
#include <rtc/media_engine.h>
#include <rtc/data_stream.h>

#if defined(FEATURE_LIVE_SDK)
#include <rtc/signaling_service.h>
#endif

#define VIDEO_PROFILE_EX_SLOT -1

namespace agora {
	class IRtcEngineEventHandlerEx;
	namespace commons {
		class dns_parser;
	}
namespace rtc {
    class CallContext;
    class MediaIOController;

class RtcContext
{
public: // getters
    RtcContext(agora::base::BaseContext& baseContext, IRtcEngineEx& engine, const RtcEngineContextEx& context);
	~RtcContext();
    agora::base::BaseContext& getBaseContext() { return m_baseContext; }
    agora::base::BaseContext::worker_type& worker() { return m_worker; }
    CallContext* getCallContext() { return m_callContext.get(); };
    agora::base::CacheManager& cache() { return m_baseContext.cache(); }
    agora::base::ConfigEngine& getConfigEngine() { return m_configEngine; }
    int onSetParameters(const std::string& parameters, bool cache, bool suppressNotification) {
        return m_configEngine.onSetParameters(&m_notification, parameters, cache, suppressNotification);
    }
    int onSetParameters(any_document_t& doc, bool cache, bool suppressNotification, bool setOriginalValue=false) {
        return m_configEngine.onSetParameters(&m_notification, doc, cache, suppressNotification, setOriginalValue);
    }

	any_document_t& getProfile() {
		if (!m_profile)
			m_profile = agora::commons::make_unique<any_document_t>();
		return *m_profile.get();
	}
	void setProfile(any_document_t* profile) {
		m_profile.reset(profile);
	}
	void setMediaEngineContext(IMediaEngineContext* ctx) {
		m_mediaEngineContext.reset(ctx);
	}
	IMediaEngineContext* getMediaEngineContext() {
		return m_mediaEngineContext.get();
	}
	RtcEngineNotification& getNotification() { return m_notification; }
    agora::base::ReportService& getReportService() {
        return m_baseContext.getReportService();
	}
#if defined(FEATURE_LIVE_SDK)
    SignalingService& getSignalingService() {
        if (!m_signalingService)
            m_signalingService = agora::commons::make_unique<SignalingService>(*this);
        return *m_signalingService.get();
    }
#endif
	IRtcEngineEx& getRtcEngine() { return m_rtcEngine; }
    agora::commons::dns_parser* queryDns(void* id, const std::string& dns, std::function<void(int, const std::vector<commons::ip_t>&)>&& cb, bool cache = true) {
        return m_baseContext.queryDns(worker().get(), id, dns, std::move(cb), cache);
	}
    template <class T>
    int setParameter(const char* key, const T& value) {
        return m_configEngine.setParameter(&m_notification, key, value);
    }
public:
	void clear() {
		m_appId.clear();
	}
	const std::string& getAppId() const { return m_appId; }
	const std::string& getDeviceId() const { return m_baseContext.getDeviceId(); }
	void setAppId(const std::string& appId) { m_appId = appId; }
    agora::base::NetworkMonitor* networkMonitor() const { return m_baseContext.networkMonitor(); }
	//not thread-safe, must be set before joining channel
	void setPacketObserver(IPacketObserver* observer) { m_packetObserver = observer; }
	IPacketObserver* getPacketObserver() { return m_packetObserver; }
    bool getVideoOptionsByProfile(int profile, bool swapWidthAndHeight, VideoNetOptions& options);
	int setVideoProfileEx(int profile, int width, int height, int frameRate, int bitrate);
	bool ipv4() const { return m_baseContext.ipv4(); }
    int af_family() const { return m_baseContext.af_family(); }
    agora::base::PluginManager* pluginManager() { return m_baseContext.pluginManager(); }
	DataStreamManager& dataStreamManager() { return m_dataStreamManager; }
	MediaIOController* mediaIOController() { return m_mediaIO.get(); }
public:
    void startService(const RtcEngineContextEx& context);
	void stopService();
    void applyProfile();
public:
	void onLogging(int level, const char* format, ...);
private:
    agora::base::BaseContext& m_baseContext;
	std::string m_appId;
	//dont use unique_ptr for map, gcc failed
	//std::unordered_map<int,ITaskContext*> m_tasks;
    agora::base::BaseContext::worker_type m_worker;
    std::unique_ptr<CallContext> m_callContext;
	RtcEngineNotification m_notification;
    agora::base::ConfigEngine m_configEngine;
	std::unique_ptr<any_document_t> m_profile;
	std::unique_ptr<IMediaEngineContext> m_mediaEngineContext;
	IRtcEngineEx& m_rtcEngine;
	IPacketObserver* m_packetObserver;
	
	std::unique_ptr<MediaIOController> m_mediaIO;
    
    DataStreamManager m_dataStreamManager;

#if defined(FEATURE_LIVE_SDK)
    std::unique_ptr<SignalingService> m_signalingService;
#endif
};

}}
