//
//  Agora Media SDK
//
//  Created by Sting Feng in 2015-05.
//  Copyright (c) 2015 Agora IO. All rights reserved.
//
#pragma once
#include <functional>
#include <string>
#include <internal/IAgoraRtcEngine2.h>
#include <rtc/rtc_event.h>

#if defined(_WIN32)
extern HINSTANCE GetCurrentModuleInstance();
#elif defined(__ANDROID__) || defined(__linux__)
#include <dlfcn.h>
#endif

namespace agora {

  namespace commons {
    namespace network {
      struct network_info_t;
    }
    namespace cjson {
      class JsonWrapper;
    }
  }
  typedef agora::commons::cjson::JsonWrapper any_document_t;

class VideoCapturerInterface;
class VideoRendererInterface;

namespace rtc {
    struct VideoNetOptions;
    namespace protocol {
        struct CmdSeiLayout;
        struct CmdInjectStreamConfig;
    }

enum INTERFACE_ID_EX_TYPE
{
    AGORA_IID_RTC_ENGINE_EX = 0xacbd,
};

enum SCREEN_SHARING_MODE {
    SCREEN_SHARING_NORMAL = 0,
    SCREEN_SHARING_MOVIE = 1
};

#ifdef INTERNAL_ENGINE_STATUS
struct InternalEngineStatus{
    int recfreq;
    int playoutfreq;
    int audio_send_frame_rate;
    int audio_send_packet_rate;
    int audio_recv_packet_rate;
    int nearin_signal_level;
    int nearout_signal_level;
    int farin_signal_level;
};
#endif
enum class MediaIOType : unsigned
{
   None = 0,
   Default = 1,
   Custom = 2,
   DefaultScreenCapture = 3,
};

class IRtcEngineEventHandlerEx : public IRtcEngineEventHandler
{
public:
    virtual void onMediaEngineLoadSuccess() {}
    virtual void onMediaEngineStartCallSuccess() {}
    virtual void onAudioTransportQuality(uid_t uid, unsigned int bitrate, unsigned short delay, unsigned short lost) {
      (void)uid;
      (void)bitrate;
      (void)delay;
      (void)lost;
    }

    virtual void onVideoTransportQuality(uid_t uid, unsigned int bitrate, unsigned short delay, unsigned short lost) {
      (void)uid;
      (void)bitrate;
      (void)delay;
      (void)lost;
    }

    virtual void onRecap(const char* recapData, int length) {
        (void)recapData;
        (void)length;
    }

    virtual bool onEvent(RTC_EVENT evt, std::string* payload) {
        (void)evt;
        (void)payload;

        /* return false to indicate this event is not handled */
        return false;
    }

    virtual void onLogEvent(int level, const char* message, int length) {
        (void)level;
        (void)message;
        (void)length;
    }

    virtual void onMediaEngineEvent(int evt) {
        (void)evt;
    }

    virtual bool onCustomizedSei(const void **content, int *length) {
      (void)content;
      (void)length;

      /* return false to indicate the SEI content is left to SDK to generate */
      return false;
    }

#ifdef INTERNAL_ENGINE_STATUS
    virtual void onInternalEngineStatus(InternalEngineStatus state) {
        (void)state;
    }
#endif
};

struct RtcEngineContextEx {
    IRtcEngineEventHandler* eventHandler;
    bool isExHandler;
    bool forceAlternativeNetworkEngine;
    const char* appId;
	RtcEngineContextEx()
		:eventHandler(NULL)
		, isExHandler(false)
#if defined(_WIN32)
        , forceAlternativeNetworkEngine(false)
#else
        , forceAlternativeNetworkEngine(false)
#endif
		, appId(NULL)
	{}
};

struct WebAgentVideoStats {
	uid_t uid;
	int delay;
	int sentFrameRate;
	int renderedFrameRate;
	int skippedFrames;
};
class RtcContext;

//full feature definition of rtc engine interface
class IRtcEngineEx : public IRtcEngine2
{
public:
    static const char* getSdkVersion(int* build);
public:
    virtual ~IRtcEngineEx() {}
    virtual int initializeEx(const RtcEngineContextEx& context) = 0;
    virtual int setVideoCanvas(const VideoCanvas& canvas) = 0;
    virtual int setParameters(const char* parameters) = 0;
    /**
    * get multiple parameters.
    */
    virtual int getParameters(const char* key, any_document_t& result) = 0;
    virtual int setProfile(const char* profile, bool merge) = 0;
    virtual int getProfile(any_document_t& result) = 0;
    virtual int notifyNetworkChange(agora::commons::network::network_info_t&& networkInfo) = 0;
    virtual int sendReportMessage(const char* data, size_t length, int type) = 0;
    virtual int getOptionsByVideoProfile(int profile, VideoNetOptions& options) = 0;
    virtual RtcContext* getRtcContext() = 0;
    virtual int setLogCallback(bool enabled) = 0;
    virtual int makeQualityReportUrl(const char* channel, uid_t listenerUid, uid_t speakerUid, QUALITY_REPORT_FORMAT_TYPE format, agora::util::AString& url) = 0;

    /**
    * get SHA1 values of source files for building the binaries being used, for bug tracking.
    */
    //virtual const char* getSourceVersion() const = 0;

    virtual int reportWebAgentVideoStats(const WebAgentVideoStats& stats) = 0;

    virtual int printLog(int level, const char* message) = 0;
    virtual int runOnWorkerThread(std::function<void(void)>&& f) = 0;
    virtual int reportArgusCounters(int *counterId, int *value, int count, uid_t uid) = 0;
    virtual int setVideoCompositingLayout2(protocol::CmdSeiLayout& sei) = 0;
#if defined(__APPLE__) || defined(_WIN32)
    virtual int startScreenCaptureEx(WindowIDType windowId, int captureFreq, const Rect *rect, int bitrate = 0, SCREEN_SHARING_MODE mode = SCREEN_SHARING_NORMAL) = 0;
#endif

    virtual int addVideoCapture(agora::VideoCapturerInterface* capturer, MediaIOType type) = 0;
    virtual	int addLocalVideoRender(agora::VideoRendererInterface* renderer, MediaIOType type) = 0;
	virtual int addVideoReceiveTrack(uid_t uid, agora::VideoRendererInterface* render, MediaIOType type) = 0;
	virtual int removeVideoReceiveTrack(uid_t uid) = 0;
    virtual int addInjectStreamUrl2(const char* url, protocol::CmdInjectStreamConfig& config) = 0;
};

class RtcEngineParametersEx : public RtcEngineParameters
{
public:
    RtcEngineParametersEx(IRtcEngine* engine)
        :RtcEngineParameters(engine)
    {}
    int enableAudioQualityIndication(bool enabled) {
        return parameter()->setBool("rtc.audio_quality_indication", enabled);
    }
    int enableTransportQualityIndication(bool enabled) {
        return parameter()->setBool("rtc.transport_quality_indication", enabled);
    }
    int enableRecap(int interval) {
        return parameter()->setInt("che.audio.recap.interval", interval);
    }
    int playRecap() {
        return parameter()->setBool("che.audio.recap.start_play", true);
    }
    int playAudioFile(const char* filePath) {
        if (!filePath || *filePath == '\0')
            return -ERR_INVALID_ARGUMENT;
        return parameter()->setString("che.audio.start_audio_file", filePath);
    }
    int playVideoFile(const char* filePath) {
        if (!filePath || *filePath == '\0')
            return -ERR_INVALID_ARGUMENT;
        return parameter()->setString("che.video.start_video_file", filePath);
    }
    // many players (up to 25): special settings in underlying engine
    int enableManyPlayers(bool enabled) {
        return parameter()->setBool("che.video.enableManyParties", enabled);
    }
    // request video engine to play specified stream.
    // streamType: 0 - large resolution, 1 - 1/4 resolution and 1/2 fps of large resolution
    int requestBitstreamType(unsigned int uid, unsigned int streamType) {
        if (streamType != 0 && streamType != 1)
            return -1;
        return setObject("che.video.setstream", "{\"uid\":%u,\"stream\":%d}", uid, streamType);
    }

protected:
    int setProfile(const char* format, ...) {
        char buf[512];
        va_list args;
        va_start(args, format);
        vsnprintf(buf, sizeof(buf)-1, format, args);
        va_end(args);
        return parameter()->setProfile(buf, true);
    }
};

class RtcEngineLibHelper
{
	typedef const char* (AGORA_CALL *PFN_GetAgoraRtcEngineVersion)(int* build);
	typedef IRtcEngine* (AGORA_CALL *PFN_CreateAgoraRtcEngine)();
    typedef void (AGORA_CALL *PFN_ShutdownAgoraRtcEngineService)();
#if defined(_WIN32)
    typedef HINSTANCE lib_handle_t;
    static HINSTANCE MyLoadLibrary(const char* dllname)
    {
        char path[MAX_PATH];
        GetModuleFileNameA(GetCurrentModuleInstance(), path, MAX_PATH);
        auto p = strrchr(path, '\\');
        strcpy(p + 1, dllname);
        HINSTANCE hDll = LoadLibraryA(path);
        if (hDll)
            return hDll;

        hDll = LoadLibraryA(dllname);
        return hDll;
    }
#else
    typedef void* lib_handle_t;
#endif
public:
    RtcEngineLibHelper(const char* dllname)
        : m_firstInit(true)
        , m_lib(NULL)
        , m_dllName(dllname)
        , m_pfnCreateAgoraRtcEngine(nullptr)
		, m_pfnGetAgoraRtcEngineVersion(nullptr)
    {
    }

    bool initialize()
    {
        if (!m_firstInit)
            return isValid();
        m_firstInit = false;
#if defined(_WIN32)
        m_lib = MyLoadLibrary(m_dllName.c_str());
        if (m_lib)
        {
            m_pfnCreateAgoraRtcEngine = (PFN_CreateAgoraRtcEngine)GetProcAddress(m_lib, "createAgoraRtcEngine");
			m_pfnGetAgoraRtcEngineVersion = (PFN_GetAgoraRtcEngineVersion)GetProcAddress(m_lib, "getAgoraRtcEngineVersion");
        }
#elif defined(__ANDROID__) || defined(__linux__)
        m_lib = dlopen(m_dllName.c_str(), RTLD_LAZY);
        if (m_lib)
        {
            m_pfnCreateAgoraRtcEngine = (PFN_CreateAgoraRtcEngine)dlsym(m_lib, "createAgoraRtcEngine");
			m_pfnGetAgoraRtcEngineVersion = (PFN_GetAgoraRtcEngineVersion)dlsym(m_lib, "getAgoraRtcEngineVersion");
		}
#endif
        return isValid();
    }

    void deinitialize()
    {
        if (m_lib)
        {
#if defined(_WIN32)
            FreeLibrary(m_lib);
#elif defined(__ANDROID__) || defined(__linux__)
            dlclose(m_lib);
#endif
            m_lib = NULL;
        }
        m_pfnCreateAgoraRtcEngine = nullptr;
		m_pfnGetAgoraRtcEngineVersion = nullptr;
    }

    ~RtcEngineLibHelper()
    {
        deinitialize();
    }

    bool isValid()
    {
        return m_pfnCreateAgoraRtcEngine != NULL;
    }

    agora::rtc::IRtcEngine* createEngine()
    {
        return m_pfnCreateAgoraRtcEngine ? m_pfnCreateAgoraRtcEngine() : NULL;
    }
	const char* getVersion(int* build)
	{
		return m_pfnGetAgoraRtcEngineVersion ? m_pfnGetAgoraRtcEngineVersion(build) : nullptr;
	}
private:
    bool m_firstInit;
    lib_handle_t m_lib;
    std::string m_dllName;
    PFN_CreateAgoraRtcEngine m_pfnCreateAgoraRtcEngine;
	PFN_GetAgoraRtcEngineVersion m_pfnGetAgoraRtcEngineVersion;
};


// A helper function for decoding out the SEI layout

struct canvas_info {
  int width;
  int height;
  int bgcolor;
};

struct region_info {
  unsigned id;

  double x;
  double y;
  double width;
  double height;

  int alpha; // [0, 255]

  int render_mode; // 0, crop; 1, ZoomtoFit
  int zorder; // [0, 100]
};

#define NUM_OF_LAYOUT_REGIONS 17
struct layout_info {
  long long ms;
  canvas_info canvas;
  // At most 9 broadcasters: 1 host, 8 guests.
  unsigned int region_count;
  region_info regions[NUM_OF_LAYOUT_REGIONS];
};

bool decode_sei_layout(const void *data, unsigned size, layout_info *layout);

}}
