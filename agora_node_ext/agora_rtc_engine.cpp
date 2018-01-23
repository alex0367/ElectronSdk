/*
* Copyright (c) 2017 Agora.io
* All rights reserved.
* Proprietry and Confidential -- Agora.io
*/

/*
*  Created by Wang Yongli, 2017
*/

#include "node_napi_api.h"
#include "agora_rtc_engine.h"
#include "node_video_render.h"
#include "node_uid.h"
#include "agora_video_source.h"
#include <string>

using std::string;
namespace agora {
    namespace rtc {

        DEFINE_CLASS(NodeRtcEngine);

        /**
         * To declared class and member functions that could be used in JS layer directly.
         */
        void NodeRtcEngine::Init(Local<Object>& module)
        {
            Isolate *isolate = module->GetIsolate();
            BEGIN_PROPERTY_DEFINE(NodeRtcEngine, createInstance, 5)
                PROPERTY_METHOD_DEFINE(onEvent)
                PROPERTY_METHOD_DEFINE(initialize)
                PROPERTY_METHOD_DEFINE(getVersion)
                PROPERTY_METHOD_DEFINE(getErrorDescription)
                PROPERTY_METHOD_DEFINE(joinChannel)
                PROPERTY_METHOD_DEFINE(leaveChannel)
                PROPERTY_METHOD_DEFINE(renewChannelKey)
                PROPERTY_METHOD_DEFINE(setChannelProfile)
                PROPERTY_METHOD_DEFINE(setClientRole)
                PROPERTY_METHOD_DEFINE(startEchoTest)
                PROPERTY_METHOD_DEFINE(stopEchoTest)
                PROPERTY_METHOD_DEFINE(enableLastmileTest)
                PROPERTY_METHOD_DEFINE(disableLastmileTest)
                PROPERTY_METHOD_DEFINE(enableVideo)
                PROPERTY_METHOD_DEFINE(disableVideo)
                PROPERTY_METHOD_DEFINE(startPreview)
                PROPERTY_METHOD_DEFINE(stopPreview)
                PROPERTY_METHOD_DEFINE(setVideoProfile)
                PROPERTY_METHOD_DEFINE(enableAudio)
                PROPERTY_METHOD_DEFINE(disableAudio)
                PROPERTY_METHOD_DEFINE(setAudioProfile)
                PROPERTY_METHOD_DEFINE(getCallId)
                PROPERTY_METHOD_DEFINE(rate)
                PROPERTY_METHOD_DEFINE(complain)
                PROPERTY_METHOD_DEFINE(setEncryptionSecret)
                PROPERTY_METHOD_DEFINE(createDataStream)
                PROPERTY_METHOD_DEFINE(sendStreamMessage)
                PROPERTY_METHOD_DEFINE(muteLocalAudioStream)
                PROPERTY_METHOD_DEFINE(muteAllRemoteAudioStreams)
                PROPERTY_METHOD_DEFINE(muteRemoteAudioStream)
                PROPERTY_METHOD_DEFINE(muteLocalVideoStream)
                PROPERTY_METHOD_DEFINE(enableLocalVideo)
                PROPERTY_METHOD_DEFINE(muteAllRemoteVideoStreams)
                PROPERTY_METHOD_DEFINE(muteRemoteVideoStream)
                PROPERTY_METHOD_DEFINE(setRemoteVideoStreamType)
                PROPERTY_METHOD_DEFINE(setRemoteDefaultVideoStreamType)
                PROPERTY_METHOD_DEFINE(setPlaybackDeviceVolume)
                PROPERTY_METHOD_DEFINE(enableAudioVolumeIndication)
                PROPERTY_METHOD_DEFINE(startAudioRecording)
                PROPERTY_METHOD_DEFINE(stopAudioRecording)
                PROPERTY_METHOD_DEFINE(startAudioMixing)
                PROPERTY_METHOD_DEFINE(stopAudioMixing)
                PROPERTY_METHOD_DEFINE(pauseAudioMixing)
                PROPERTY_METHOD_DEFINE(resumeAudioMixing)
                PROPERTY_METHOD_DEFINE(adjustAudioMixingVolume)
                PROPERTY_METHOD_DEFINE(getAudioMixingDuration)
                PROPERTY_METHOD_DEFINE(getAudioMixingCurrentPosition)
                PROPERTY_METHOD_DEFINE(setAudioMixingPosition)
                PROPERTY_METHOD_DEFINE(setLocalVoicePitch)
                PROPERTY_METHOD_DEFINE(setInEarMonitoringVolume)
                PROPERTY_METHOD_DEFINE(setAudioProfile)
                PROPERTY_METHOD_DEFINE(pauseAudio)
                PROPERTY_METHOD_DEFINE(resumeAudio)
                PROPERTY_METHOD_DEFINE(startScreenCapture)
                PROPERTY_METHOD_DEFINE(stopScreenCapture)
                PROPERTY_METHOD_DEFINE(updateScreenCaptureRegion)
                PROPERTY_METHOD_DEFINE(setLogFile)
                PROPERTY_METHOD_DEFINE(setLogFilter)
                PROPERTY_METHOD_DEFINE(startRecordingService)
                PROPERTY_METHOD_DEFINE(stopRecordingService)
                PROPERTY_METHOD_DEFINE(refreshRecordingServiceStatus)
                PROPERTY_METHOD_DEFINE(enableDualStreamMode)
                PROPERTY_METHOD_DEFINE(setRecordingAudioFrameParameters)
                PROPERTY_METHOD_DEFINE(setPlaybackAudioFrameParameters)
                PROPERTY_METHOD_DEFINE(setMixedAudioFrameParameters)
                PROPERTY_METHOD_DEFINE(adjustRecordingSignalVolume)
                PROPERTY_METHOD_DEFINE(adjustPlaybackSignalVolume)
                PROPERTY_METHOD_DEFINE(setHighQualityAudioParameters)
                PROPERTY_METHOD_DEFINE(enableWebSdkInteroperability)
                PROPERTY_METHOD_DEFINE(setVideoQualityParameters)
                PROPERTY_METHOD_DEFINE(registerDeliverFrame)
                PROPERTY_METHOD_DEFINE(setupLocalVideo)
                PROPERTY_METHOD_DEFINE(subscribe)
                PROPERTY_METHOD_DEFINE(unsubscribe)
                PROPERTY_METHOD_DEFINE(getVideoDevices)
                PROPERTY_METHOD_DEFINE(setVideoDevice)
                PROPERTY_METHOD_DEFINE(getCurrentVideoDevice)
                PROPERTY_METHOD_DEFINE(startVideoDeviceTest)
                PROPERTY_METHOD_DEFINE(stopVideoDeviceTest)
                PROPERTY_METHOD_DEFINE(getAudioPlaybackDevices)
                PROPERTY_METHOD_DEFINE(setAudioPlaybackDevice)
                PROPERTY_METHOD_DEFINE(getCurrentAudioPlaybackDevice)
                PROPERTY_METHOD_DEFINE(setAudioPlaybackVolume)
                PROPERTY_METHOD_DEFINE(getAudioPlaybackVolume)
                PROPERTY_METHOD_DEFINE(getAudioRecordingDevices)
                PROPERTY_METHOD_DEFINE(setAudioRecordingDevice)
                PROPERTY_METHOD_DEFINE(getCurrentAudioRecordingDevice)
                PROPERTY_METHOD_DEFINE(getAudioRecordingVolume)
                PROPERTY_METHOD_DEFINE(setAudioRecordingVolume)
                PROPERTY_METHOD_DEFINE(startAudioPlaybackDeviceTest)
                PROPERTY_METHOD_DEFINE(stopAudioPlaybackDeviceTest)
                PROPERTY_METHOD_DEFINE(startAudioRecordingDeviceTest)
                PROPERTY_METHOD_DEFINE(stopAudioRecordingDeviceTest)
                PROPERTY_METHOD_DEFINE(getAudioPlaybackDeviceMute)
                PROPERTY_METHOD_DEFINE(setAudioPalybackDeviceMute)
                PROPERTY_METHOD_DEFINE(getAudioRecordingDeviceMute)
                PROPERTY_METHOD_DEFINE(setAudioRecordingDeviceMute)
                PROPERTY_METHOD_DEFINE(setEncryptionMode)
                PROPERTY_METHOD_DEFINE(setVideoCompositingLayout)
                PROPERTY_METHOD_DEFINE(addPublishUrl)
                PROPERTY_METHOD_DEFINE(removePublishUrl)
                PROPERTY_METHOD_DEFINE(startScreenCapture2)
                PROPERTY_METHOD_DEFINE(stopScreenCatpure2);
                PROPERTY_METHOD_DEFINE(videoSourceInitialize);
                PROPERTY_METHOD_DEFINE(videoSourceJoin);
                PROPERTY_METHOD_DEFINE(videoSourceRenewToken);
                PROPERTY_METHOD_DEFINE(videoSourceSetChannelProfile);
                PROPERTY_METHOD_DEFINE(videoSourceSetVideoProfile);
                PROPERTY_METHOD_DEFINE(videoSourceRelease);
                /*PROPERTY_METHOD_DEFINE(clearVideoCompositingLayout)
                PROPERTY_METHOD_DEFINE(configPublisher)
                PROPERTY_METHOD_DEFINE(setLiveTranscoding)
                PROPERTY_METHOD_DEFINE(addInjectStreamUrl)
                PROPERTY_METHOD_DEFINE(removeInjectStreamUrl)*/
            EN_PROPERTY_DEFINE()
            module->Set(String::NewFromUtf8(isolate, "NodeRtcEngine"), tpl->GetFunction());
        }

        /**
         * The function is used as class constructor in JS layer
         */
        void NodeRtcEngine::createInstance(const FunctionCallbackInfo<Value>& args)
        {
            LOG_ENTER;
            Isolate *isolate = args.GetIsolate();
            /*
            *  Called from new
            */
            if (args.IsConstructCall()) {
                NodeRtcEngine *engine = new NodeRtcEngine(isolate);
                engine->Wrap(args.This());
                args.GetReturnValue().Set(args.This());
            }
            else {
                Local<Function> cons = Local<Function>::New(isolate, constructor);
                Local<Context> context = isolate->GetCurrentContext();
                Local<Object> instance = cons->NewInstance(context).ToLocalChecked();
                args.GetReturnValue().Set(instance);
            }
            LOG_LEAVE;
        }

        /**
         * Constructor
         */
        NodeRtcEngine::NodeRtcEngine(Isolate* isolate)
            : m_isolate(isolate)
        {
            LOG_ENTER;
            /** m_engine provide SDK functionality */
			m_engine = createAgoraRtcEngine();
            /** m_eventHandler provide SDK event handler. */
            m_eventHandler = new NodeEventHandler(this);
            /** Node ADDON takes advantage of self render interface */
            m_externalVideoRenderFactory = new NodeVideoRenderFactory(*this);
            /** m_videoSourceSink provide facilities to multiple video source based on multiple process */
            m_videoSourceSink.reset(createVideoSource());
            LOG_LEAVE;
		}

        NodeRtcEngine::~NodeRtcEngine()
        {
            LOG_ENTER;
			if (m_engine) {
				m_engine->release();
				m_engine = nullptr;
			}
            if (m_eventHandler){
                delete m_eventHandler;
            }
            if (m_externalVideoRenderFactory) {
                delete m_externalVideoRenderFactory;
            }
            LOG_LEAVE;
        }
        
        NodeVideoStreamChannel* NodeRtcEngine::findChannelByUid(uid_t uid) {
            for (auto& pChannel : m_channels) {
                if (pChannel->getUid() == uid) {
                    return pChannel.get();
                }
            }
            return nullptr;
        }

        bool NodeRtcEngine::removeChannelByUid(uid_t uid) {
            for (auto& pChannel : m_channels) {
                if (pChannel->getUid() == uid) {
                    m_channels.remove(pChannel);
                    return true;
                }
            }
            return false;
        }

        NAPI_API_DEFINE_WRAPPER_PARAM_0(startEchoTest);

        NAPI_API_DEFINE_WRAPPER_PARAM_0(stopEchoTest);

        NAPI_API_DEFINE_WRAPPER_PARAM_0(enableLastmileTest);

        NAPI_API_DEFINE_WRAPPER_PARAM_0(disableLastmileTest);

        NAPI_API_DEFINE_WRAPPER_PARAM_0(enableVideo);

        NAPI_API_DEFINE_WRAPPER_PARAM_0(disableVideo);

        NAPI_API_DEFINE_WRAPPER_PARAM_0(startPreview);

        NAPI_API_DEFINE_WRAPPER_PARAM_0(stopPreview);

        NAPI_API_DEFINE_WRAPPER_PARAM_0(enableAudio);

        NAPI_API_DEFINE_WRAPPER_PARAM_0(disableAudio);

        NAPI_API_DEFINE_WRAPPER_PARAM_0(stopScreenCapture);

        NAPI_API_DEFINE_WRAPPER_SET_PARAMETER_0(stopAudioRecording);

        NAPI_API_DEFINE_WRAPPER_SET_PARAMETER_0(stopAudioMixing);

        NAPI_API_DEFINE_WRAPPER_SET_PARAMETER_0(pauseAudioMixing);

        NAPI_API_DEFINE_WRAPPER_SET_PARAMETER_0(resumeAudioMixing);

        NAPI_API_DEFINE_WRAPPER_SET_PARAMETER_0(pauseAudio);

        NAPI_API_DEFINE_WRAPPER_SET_PARAMETER_0(resumeAudio);

        NAPI_API_DEFINE_WRAPPER_SET_PARAMETER_0(refreshRecordingServiceStatus);

        NAPI_API_DEFINE_WRAPPER_SET_PARAMETER_1(muteLocalAudioStream, bool);

        NAPI_API_DEFINE_WRAPPER_SET_PARAMETER_1(muteAllRemoteAudioStreams, bool);

        NAPI_API_DEFINE_WRAPPER_SET_PARAMETER_1(muteLocalVideoStream, bool);

        NAPI_API_DEFINE_WRAPPER_SET_PARAMETER_1(enableLocalVideo, bool);

        NAPI_API_DEFINE_WRAPPER_SET_PARAMETER_1(muteAllRemoteVideoStreams, bool);

        NAPI_API_DEFINE_WRAPPER_SET_PARAMETER_1(setPlaybackDeviceVolume, int32);

        NAPI_API_DEFINE_WRAPPER_SET_PARAMETER_1(adjustAudioMixingVolume, int32);

        NAPI_API_DEFINE_WRAPPER_SET_PARAMETER_1(setAudioMixingPosition, int32);

        NAPI_API_DEFINE_WRAPPER_SET_PARAMETER_1(setLocalVoicePitch, double);

        NAPI_API_DEFINE_WRAPPER_SET_PARAMETER_1(setInEarMonitoringVolume, int32);

        NAPI_API_DEFINE_WRAPPER_SET_PARAMETER_1(setLogFile, nodestring);

        NAPI_API_DEFINE_WRAPPER_SET_PARAMETER_1(setLogFilter, uint32);

        NAPI_API_DEFINE_WRAPPER_SET_PARAMETER_1(startRecordingService, nodestring);

        NAPI_API_DEFINE_WRAPPER_SET_PARAMETER_1(stopRecordingService, nodestring);

        NAPI_API_DEFINE_WRAPPER_SET_PARAMETER_1(enableDualStreamMode, bool);

        NAPI_API_DEFINE_WRAPPER_SET_PARAMETER_1(adjustRecordingSignalVolume, int32);

        NAPI_API_DEFINE_WRAPPER_SET_PARAMETER_1(adjustPlaybackSignalVolume, int32);

        NAPI_API_DEFINE_WRAPPER_SET_PARAMETER_1(enableWebSdkInteroperability, bool);

        NAPI_API_DEFINE_WRAPPER_SET_PARAMETER_1(setVideoQualityParameters, bool);

        NAPI_API_DEFINE(NodeRtcEngine, videoSourceInitialize)
        {
            LOG_ENTER;
            int result = -1;
            do{
                NodeRtcEngine *pEngine = nullptr;
                napi_get_native_this(args, pEngine);
                CHECK_NATIVE_THIS(pEngine);
                if (!pEngine->m_videoSourceSink->initialize(pEngine->m_eventHandler)){
                    break;
                }
                result = 0;
            } while (false);
            napi_set_int_result(args, result);
            LOG_LEAVE;
        }

        NAPI_API_DEFINE(NodeRtcEngine, videoSourceJoin)
        {
            LOG_ENTER;
            int result = -1;
            do{
                NodeRtcEngine *pEngine = nullptr;
                napi_get_native_this(args, pEngine);
                CHECK_NATIVE_THIS(pEngine);
                NodeString key, name, chan_info;
                uid_t uid;
                napi_status status = napi_get_value_nodestring_(args[0], key);
                CHECK_NAPI_STATUS(status);

                status = napi_get_value_nodestring_(args[1], name);
                CHECK_NAPI_STATUS(status);

                status = napi_get_value_nodestring_(args[2], chan_info);
                CHECK_NAPI_STATUS(status);

                status = NodeUid::getUidFromNodeValue(args[3], uid);
                CHECK_NAPI_STATUS(status);

                pEngine->m_videoSourceSink->join(key, name, chan_info, uid);
                result = 0;
            } while (false);
            napi_set_int_result(args, result);
            LOG_LEAVE;
        }

        NAPI_API_DEFINE(NodeRtcEngine, videoSourceRenewToken)
        {
            LOG_ENTER;
            int result = -1;
            do{
                NodeRtcEngine *pEngine = nullptr;
                napi_get_native_this(args, pEngine);
                CHECK_NATIVE_THIS(pEngine);
                NodeString token;
                napi_status status = napi_get_value_nodestring_(args[0], token);
                CHECK_NAPI_STATUS(status);

                pEngine->m_videoSourceSink->renewVideoSourceToken(token);
                result = 0;
            } while (false);
            napi_set_int_result(args, result);
            LOG_LEAVE;
        }

        NAPI_API_DEFINE(NodeRtcEngine, videoSourceSetChannelProfile)
        {
            LOG_ENTER;
            int result = -1;
            do{
                NodeRtcEngine *pEngine = nullptr;
                napi_get_native_this(args, pEngine);
                CHECK_NATIVE_THIS(pEngine);
                int profile;
                napi_status status = napi_get_value_int32_(args[0], profile);
                CHECK_NAPI_STATUS(status);

                pEngine->m_videoSourceSink->setVideoSourceChannelProfile((agora::rtc::CHANNEL_PROFILE_TYPE)profile);
                result = 0;
            } while (false);
            napi_set_int_result(args, result);
            LOG_LEAVE;
        }

        NAPI_API_DEFINE(NodeRtcEngine, videoSourceSetVideoProfile)
        {
            LOG_ENTER;
            int result = -1;
            do{
                NodeRtcEngine *pEngine = nullptr;
                napi_get_native_this(args, pEngine);
                CHECK_NATIVE_THIS(pEngine);
                int profile;
                bool swapWidthAndHeight;
                napi_status status = napi_get_value_int32_(args[0], profile);
                CHECK_NAPI_STATUS(status);
                status = napi_get_value_bool_(args[1], swapWidthAndHeight);
                CHECK_NAPI_STATUS(status);

                pEngine->m_videoSourceSink->setVideoSourceVideoProfile((agora::rtc::VIDEO_PROFILE_TYPE)profile, swapWidthAndHeight);
                result = 0;
            } while (false);
            napi_set_int_result(args, result);
            LOG_LEAVE;
        }

        NAPI_API_DEFINE(NodeRtcEngine, startScreenCapture2)
        {
            LOG_ENTER;
            do{
                NodeRtcEngine *pEngine = nullptr;
                napi_get_native_this(args, pEngine);
                napi_status status;
                CHECK_NATIVE_THIS(pEngine);
                uint32_t captureFreq, bitrate;
                int top, left, bottom, right;

#if defined(__APPLE__)
                unsigned int windowId;
                status = napi_get_value_uint32_(args[0], windowId);
                CHECK_NAPI_STATUS(status);
#elif defined(_WIN32)
#if defined(_WIN64)
                int64_t wid;
                status = napi_get_value_int64_(args[0], wid);
#else
                uint32_t wid;
                status = napi_get_value_uint32_(args[0], wid);
#endif

                CHECK_NAPI_STATUS(status);
                HWND windowId = (HWND)wid;
#endif
                status = napi_get_value_uint32_(args[1], captureFreq);
                CHECK_NAPI_STATUS(status);

                Local<Object> rect = args[2]->ToObject(args.GetIsolate());
                Local<Name> topKey = String::NewFromUtf8(args.GetIsolate(), "top", NewStringType::kInternalized).ToLocalChecked();
                Local<Value> topValue = rect->Get(args.GetIsolate()->GetCurrentContext(), topKey).ToLocalChecked();
                top = topValue->Int32Value();

                Local<Name> leftKey = String::NewFromUtf8(args.GetIsolate(), "left", NewStringType::kInternalized).ToLocalChecked();
                Local<Value> leftValue = rect->Get(args.GetIsolate()->GetCurrentContext(), leftKey).ToLocalChecked();
                left = leftValue->Int32Value();

                Local<Name> bottomKey = String::NewFromUtf8(args.GetIsolate(), "bottom", NewStringType::kInternalized).ToLocalChecked();
                Local<Value> bottomValue = rect->Get(args.GetIsolate()->GetCurrentContext(), bottomKey).ToLocalChecked();
                bottom = bottomValue->Int32Value();

                Local<Name> rightKey = String::NewFromUtf8(args.GetIsolate(), "right", NewStringType::kInternalized).ToLocalChecked();
                Local<Value> rightValue = rect->Get(args.GetIsolate()->GetCurrentContext(), rightKey).ToLocalChecked();
                right = rightValue->Int32Value();

                status = napi_get_value_uint32_(args[3], bitrate);
                CHECK_NAPI_STATUS(status);

                Rect region(top, left, bottom, right);
                pEngine->m_videoSourceSink->captureScreen(windowId, captureFreq, &region, bitrate);
                napi_set_int_result(args, 0);
            } while (false);
            LOG_LEAVE;
        }

        NAPI_API_DEFINE(NodeRtcEngine, stopScreenCatpure2)
        {
            LOG_ENTER;
            do{
                NodeRtcEngine *pEngine = nullptr;
                napi_get_native_this(args, pEngine);
                CHECK_NATIVE_THIS(pEngine);

                pEngine->m_videoSourceSink->stopCaptureScreen();
                napi_set_int_result(args, 0);
            } while (false);
            LOG_LEAVE;
        }

        NAPI_API_DEFINE(NodeRtcEngine, videoSourceRelease)
        {
            LOG_ENTER;
            do{
                NodeRtcEngine *pEngine = nullptr;
                napi_get_native_this(args, pEngine);
                CHECK_NATIVE_THIS(pEngine);

                pEngine->m_videoSourceSink->release();
                napi_set_int_result(args, 0);
            } while (false);
            LOG_LEAVE;
        }

        NAPI_API_DEFINE(NodeRtcEngine, leaveChannel)
        {
            LOG_ENTER;
            do {
                NodeRtcEngine *pEngine = nullptr;
                napi_get_native_this(args, pEngine);
                CHECK_NATIVE_THIS(pEngine);
                std::shared_ptr<agora::rtc::NodeVideoStreamChannel> chan;
                for (auto& channel : pEngine->m_channels) {
                    if (channel->getUid() == 0) {
                        chan = channel;
                        break;
                    }
                }
                pEngine->m_channels.clear();
                pEngine->m_channels.push_back(chan);
                int result = pEngine->m_engine->leaveChannel();
                args.GetReturnValue().Set(Integer::New(args.GetIsolate(), result));
            } while (false);
            LOG_LEAVE;
        }

        NAPI_API_DEFINE(NodeRtcEngine, registerDeliverFrame)
        {
            LOG_ENTER;
            do {
                Isolate *isolate = args.GetIsolate();
                NodeRtcEngine *pEngine = nullptr;
                napi_get_native_this(args, pEngine);
                CHECK_NATIVE_THIS(pEngine);
                agora::rtc::NodeVideoStreamChannel::s_nodeInfo.env = isolate;
                agora::rtc::NodeVideoStreamChannel::s_nodeInfo.callback.Reset(isolate, args[0].As<Function>());
                agora::rtc::NodeVideoStreamChannel::s_nodeInfo.js_this.Reset(isolate, args.This());
                agora::rtc::NodeVideoStreamChannel::s_nodeInfo.init = true;
            } while (false);
            LOG_LEAVE;
        }

        NAPI_API_DEFINE(NodeRtcEngine, initialize)
        {
            LOG_ENTER;            
            do {
                Isolate *isolate = args.GetIsolate();
                NodeRtcEngine *pEngine = nullptr;
                napi_get_native_this(args, pEngine);
                CHECK_NATIVE_THIS(pEngine);
                NodeString appid;
                napi_status status = napi_get_value_nodestring_(args[0], appid);
                CHECK_NAPI_STATUS(status);
                RtcEngineContext context;
                context.eventHandler = pEngine->m_eventHandler;
                context.appId = appid;
                int suc = pEngine->m_engine->initialize(context);
                if (0 != suc) {
                    LOG_ERROR("Rtc engine initialize failed with error :%d\n", suc);
                    break;
                }
                agora::util::AutoPtr<agora::media::IMediaEngine> pMediaEngine;
                pMediaEngine.queryInterface(pEngine->m_engine, AGORA_IID_MEDIA_ENGINE);
                if (pMediaEngine) {
                    pMediaEngine->registerVideoRenderFactory(pEngine->m_externalVideoRenderFactory);
                }
                pEngine->m_engine->enableVideo();
                RtcEngineParameters rep(pEngine->m_engine);
                rep.enableLocalVideo(true);
            } while (false);
            LOG_LEAVE;
        }

        NAPI_API_DEFINE(NodeRtcEngine, getVersion)
        {
            LOG_ENTER;
            do {
                NodeRtcEngine *pEngine = nullptr;
                napi_get_native_this(args, pEngine);
                CHECK_NATIVE_THIS(pEngine);
                int build = 0;
                const char* strVersion = pEngine->m_engine->getVersion(&build);
                Local<Object> obj = Object::New(args.GetIsolate());
                CHECK_NAPI_OBJECT(obj);
                Local<Value> key = String::NewFromUtf8(args.GetIsolate(), "version", NewStringType::kInternalized).ToLocalChecked();
                CHECK_NAPI_OBJECT(key);
                Local<Value> value = String::NewFromUtf8(args.GetIsolate(), strVersion, NewStringType::kInternalized).ToLocalChecked();
                CHECK_NAPI_OBJECT(value);
                obj->Set(args.GetIsolate()->GetCurrentContext(), key, value);
                Local<Value> buildKey = String::NewFromUtf8(args.GetIsolate(), "build", NewStringType::kInternalized).ToLocalChecked();
                CHECK_NAPI_OBJECT(buildKey);
                Local<Value> buildValue = Integer::New(args.GetIsolate(), build);
                CHECK_NAPI_OBJECT(buildValue);
                obj->Set(args.GetIsolate()->GetCurrentContext(), buildKey, buildValue);
                args.GetReturnValue().Set(obj);
            } while (false);
            LOG_LEAVE;
        }

        NAPI_API_DEFINE(NodeRtcEngine, setEncryptionMode)
        {
            LOG_ENTER;
            do{
                NodeRtcEngine *pEngine = nullptr;
                napi_get_native_this(args, pEngine);
                CHECK_NATIVE_THIS(pEngine);
                nodestring mode;
                napi_status status = napi_get_value_nodestring_(args[0], mode);
                CHECK_NAPI_STATUS(status);

                int result = pEngine->m_engine->setEncryptionMode(mode);
                napi_set_int_result(args, result);
            } while (false);
            LOG_LEAVE;
        }

        NAPI_API_DEFINE(NodeRtcEngine, setVideoCompositingLayout)
        {
            LOG_ENTER;
            do{
                NodeRtcEngine *pEngine = nullptr;
                napi_get_native_this(args, pEngine);
                CHECK_NATIVE_THIS(pEngine);
                Isolate *iso = args.GetIsolate();
                Local<Object> comLayout = args[0]->ToObject(iso);
                VideoCompositingLayout layout;

                Local<Name> canvasWidthKey = String::NewFromUtf8(iso, "canvasWidth", NewStringType::kInternalized).ToLocalChecked();
                Local<Value> canvasWidthValue = comLayout->Get(iso->GetCurrentContext(), canvasWidthKey).ToLocalChecked();
                layout.canvasWidth = canvasWidthValue->Int32Value();

                Local<Name> canvasHeightKey = String::NewFromUtf8(iso, "canvasHeight", NewStringType::kInternalized).ToLocalChecked();
                Local<Value> canvasHeightValue = comLayout->Get(iso->GetCurrentContext(), canvasHeightKey).ToLocalChecked();
                layout.canvasHeight = canvasHeightValue->Int32Value();
            } while (false);
            LOG_LEAVE;
            /*Local<Object> rect = args[2]->ToObject(args.GetIsolate());
            Local<Name> topKey = String::NewFromUtf8(args.GetIsolate(), "top", NewStringType::kInternalized).ToLocalChecked();
            Local<Value> topValue = rect->Get(args.GetIsolate()->GetCurrentContext(), topKey).ToLocalChecked();
            top = topValue->Int32Value();*/
        }
        //NAPI_API(clearVideoCompositingLayout);
        //NAPI_API(configPublisher);
        NAPI_API_DEFINE(NodeRtcEngine, addPublishUrl)
        {
            LOG_ENTER;
            do{
                NodeRtcEngine *pEngine = nullptr;
                napi_get_native_this(args, pEngine);
                CHECK_NATIVE_THIS(pEngine);
                nodestring url;
                bool transcodingEnabled;
                napi_status status = napi_get_value_nodestring_(args[0], url);
                CHECK_NAPI_STATUS(status);
                status = napi_get_value_bool_(args[1], transcodingEnabled);
                CHECK_NAPI_STATUS(status);

                int result = pEngine->m_engine->addPublishStreamUrl(url, transcodingEnabled);
                napi_set_int_result(args, result);
            } while (false);
            LOG_LEAVE;
        }

        NAPI_API_DEFINE(NodeRtcEngine, removePublishUrl)
        {
            LOG_ENTER;
            do{
                NodeRtcEngine *pEngine = nullptr;
                napi_get_native_this(args, pEngine);
                CHECK_NATIVE_THIS(pEngine);
                nodestring url;
                napi_status status = napi_get_value_nodestring_(args[0], url);
                CHECK_NAPI_STATUS(status);
                int result = pEngine->m_engine->removePublishStreamUrl(url);
                napi_set_int_result(args, result);
            } while (false);
            LOG_LEAVE;
        }
        NAPI_API(setLiveTranscoding);
        NAPI_API(addInjectStreamUrl);
        NAPI_API(removeInjectStreamUrl);

        NAPI_API_DEFINE(NodeRtcEngine, getErrorDescription)
        {
            LOG_ENTER;
            do {
                NodeRtcEngine *pEngine = nullptr;
                napi_get_native_this(args, pEngine);
                CHECK_NATIVE_THIS(pEngine);
                int errorCode;
                napi_status status = napi_get_value_int32_(args[0], errorCode);
                CHECK_NAPI_STATUS(status);
                const char* desc = pEngine->m_engine->getErrorDescription(errorCode);
                Local<Value> descValue = String::NewFromUtf8(args.GetIsolate(), desc, NewStringType::kInternalized).ToLocalChecked();
                CHECK_NAPI_OBJECT(descValue);
                args.GetReturnValue().Set(descValue);
            } while (false);
            LOG_LEAVE;
        }

        NAPI_API_DEFINE(NodeRtcEngine, joinChannel)
        {
            LOG_ENTER;
            NodeString key, name, chan_info;
            do {
                NodeRtcEngine *pEngine = nullptr;
                napi_get_native_this(args, pEngine);
                CHECK_NATIVE_THIS(pEngine);
                uid_t uid;
                napi_status status = napi_get_value_nodestring_(args[0], key);
                CHECK_NAPI_STATUS(status);
                
                status = napi_get_value_nodestring_(args[1], name);
                CHECK_NAPI_STATUS(status);

                status = napi_get_value_nodestring_(args[2], chan_info);
                CHECK_NAPI_STATUS(status);

                status = NodeUid::getUidFromNodeValue(args[3], uid);
                CHECK_NAPI_STATUS(status);

                int result = pEngine->m_engine->joinChannel(key, name, chan_info, uid);
                args.GetReturnValue().Set(Integer::New(args.GetIsolate(), result));
            } while (false);
            LOG_LEAVE;
        }

        NAPI_API_DEFINE(NodeRtcEngine, renewChannelKey)
        {
            LOG_ENTER;
            do {
                NodeRtcEngine *pEngine = nullptr;
                NodeString newkey;
                napi_get_native_this(args, pEngine);
                CHECK_NATIVE_THIS(pEngine);
                napi_status status = napi_get_value_nodestring_(args[0], newkey);
                CHECK_NAPI_STATUS(status);

                int result = pEngine->m_engine->renewToken(newkey);
                napi_set_int_result(args, result);
            } while (false);
            LOG_LEAVE;
        }

        NAPI_API_DEFINE(NodeRtcEngine, setChannelProfile)
        {
            LOG_ENTER;
            do {
                NodeRtcEngine *pEngine = nullptr;
                napi_get_native_this(args, pEngine);
                CHECK_NATIVE_THIS(pEngine);
                napi_status status = napi_ok;
                unsigned int profile = 0;
                status = napi_get_value_uint32_(args[0], profile);
                CHECK_NAPI_STATUS(status);
                int result = pEngine->m_engine->setChannelProfile(CHANNEL_PROFILE_TYPE(profile));
                napi_set_int_result(args, result);
            } while (false);
            LOG_LEAVE;
        }

        NAPI_API_DEFINE(NodeRtcEngine, setClientRole)
        {
            LOG_ENTER;
            do {
                NodeRtcEngine *pEngine = nullptr;
                napi_get_native_this(args, pEngine);
                CHECK_NATIVE_THIS(pEngine);
                napi_status status = napi_ok;
                unsigned int role;
                status = napi_get_value_uint32_(args[0], role);
                CHECK_NAPI_STATUS(status);
                int result = pEngine->m_engine->setClientRole(CLIENT_ROLE_TYPE(role));
                napi_set_int_result(args, result);
            } while (false);
            LOG_LEAVE;
        }

        NAPI_API_DEFINE(NodeRtcEngine, setVideoProfile)
        {
            LOG_ENTER;
            do {
                NodeRtcEngine *pEngine = nullptr;
                napi_get_native_this(args, pEngine);
                CHECK_NATIVE_THIS(pEngine);
                napi_status status = napi_ok;
                unsigned int profile;
                bool swapWandH;
                napi_get_param_2(args, uint32, profile, bool, swapWandH);
                CHECK_NAPI_STATUS(status);
                int result = pEngine->m_engine->setVideoProfile(VIDEO_PROFILE_TYPE(profile), swapWandH);
                napi_set_int_result(args, result);
            } while (false);
            LOG_LEAVE;
        }

        NAPI_API_DEFINE(NodeRtcEngine, setAudioProfile)
        {
            LOG_ENTER;
            do {
                NodeRtcEngine *pEngine = nullptr;
                napi_get_native_this(args, pEngine);
                CHECK_NATIVE_THIS(pEngine);
                napi_status status = napi_ok;
                unsigned int profile, scenario;
                napi_get_param_2(args, uint32, profile, uint32, scenario);
                CHECK_NAPI_STATUS(status);

                int result = pEngine->m_engine->setAudioProfile(AUDIO_PROFILE_TYPE(profile), AUDIO_SCENARIO_TYPE(scenario));
                napi_set_int_result(args, result);
            } while (false);
            LOG_LEAVE;
        }

        NAPI_API_DEFINE(NodeRtcEngine, getCallId)
        {
            LOG_ENTER;
            do {
                NodeRtcEngine *pEngine = nullptr;
                napi_get_native_this(args, pEngine);
                CHECK_NATIVE_THIS(pEngine);
                util::AString callId;
                if (-ERR_FAILED != pEngine->m_engine->getCallId(callId)) {
                    napi_set_string_result(args, callId->c_str());
                }
            } while (false);
            LOG_LEAVE;
        }

        NAPI_API_DEFINE(NodeRtcEngine, rate)
        {
            LOG_ENTER;
            NodeString callId, desc;
            do {
                NodeRtcEngine *pEngine = nullptr;
                napi_get_native_this(args, pEngine);
                CHECK_NATIVE_THIS(pEngine);
                napi_status status = napi_ok;
                int rating;
                napi_get_value_nodestring_(args[0], callId);
                CHECK_NAPI_STATUS(status);
                status = napi_get_value_int32_(args[1], rating);
                CHECK_NAPI_STATUS(status);
                napi_get_value_nodestring_(args[2], desc);
                CHECK_NAPI_STATUS(status);

                int result = pEngine->m_engine->rate(callId, rating, desc);
                napi_set_int_result(args, result);
            } while (false);
            LOG_LEAVE;
        }

        NAPI_API_DEFINE(NodeRtcEngine, complain)
        {
            LOG_ENTER;
            NodeString callId, desc;
            do {
                NodeRtcEngine *pEngine = nullptr;
                napi_get_native_this(args, pEngine);
                CHECK_NATIVE_THIS(pEngine);
                napi_status status = napi_ok;

                napi_get_value_nodestring_(args[0], callId);
                CHECK_NAPI_STATUS(status);
                napi_get_value_nodestring_(args[1], desc);
                CHECK_NAPI_STATUS(status);

                int result = pEngine->m_engine->complain(callId, desc);
                napi_set_int_result(args, result);
            } while (false);
            LOG_LEAVE;
        }

        NAPI_API_DEFINE(NodeRtcEngine, setEncryptionSecret)
        {
            LOG_ENTER;
            NodeString secret;
            do{
                NodeRtcEngine *pEngine = nullptr;
                napi_get_native_this(args, pEngine);
                napi_status status = napi_ok;
                CHECK_NATIVE_THIS(pEngine);
                napi_get_value_nodestring_(args[0], secret);
                CHECK_NAPI_STATUS(status);

                int result = pEngine->m_engine->setEncryptionSecret(secret);
                napi_set_int_result(args, result);
            } while (false);
            LOG_LEAVE;
        }

        NAPI_API_DEFINE(NodeRtcEngine, createDataStream)
        {
            LOG_ENTER;
            do {
                NodeRtcEngine *pEngine = nullptr;
                napi_get_native_this(args, pEngine);
                CHECK_NATIVE_THIS(pEngine);
                napi_status status = napi_ok;
                int streamId;
                bool reliable, ordered;
                napi_get_param_2(args, bool, reliable, bool, ordered);
                CHECK_NAPI_STATUS(status);

                int result = pEngine->m_engine->createDataStream(&streamId, reliable, ordered);
                napi_set_int_result(args, streamId);
            } while (false);
            LOG_LEAVE;
        }

        NAPI_API_DEFINE(NodeRtcEngine, sendStreamMessage)
        {
            LOG_ENTER;
            NodeString msg;
            do {
                NodeRtcEngine *pEngine = nullptr;
                napi_get_native_this(args, pEngine);
                CHECK_NATIVE_THIS(pEngine);
                napi_status status = napi_ok;
                int streamId;
                status = napi_get_value_int32_(args[0], streamId);
                CHECK_NAPI_STATUS(status);
                napi_get_value_nodestring_(args[1], msg);
                CHECK_NAPI_STATUS(status);
                int result = pEngine->m_engine->sendStreamMessage(streamId, msg, strlen(msg));
                napi_set_int_result(args, result);
            } while (false);
            LOG_LEAVE;
        }

        NAPI_API_DEFINE(NodeRtcEngine, muteRemoteAudioStream)
        {
            LOG_ENTER;
            do {
                NodeRtcEngine *pEngine = nullptr;
                napi_status status = napi_ok;
                napi_get_native_this(args, pEngine);
                CHECK_NATIVE_THIS(pEngine);
                uid_t uid;
                bool mute;
                status = NodeUid::getUidFromNodeValue(args[0], uid);
                CHECK_NAPI_STATUS(status);
                status = napi_get_value_bool_(args[1], mute);
                CHECK_NAPI_STATUS(status);

                RtcEngineParameters rep(pEngine->m_engine);
                int result = rep.muteRemoteAudioStream(uid, mute);
                napi_set_int_result(args, result);
            } while (false);
            LOG_LEAVE;
        }

        NAPI_API_DEFINE(NodeRtcEngine, subscribe)
        {
            LOG_ENTER;
            do {
                NodeRtcEngine *pEngine = nullptr;
                napi_status status = napi_ok;
                napi_get_native_this(args, pEngine);
                CHECK_NATIVE_THIS(pEngine);
                uid_t uid;
                status = NodeUid::getUidFromNodeValue(args[0], uid);
                CHECK_NAPI_STATUS(status);

                auto channel = pEngine->findChannelByUid(uid);
                if (channel) {
                    break;
                }
                channel_context context;
                context.uid = uid;
                std::shared_ptr<NodeVideoStreamChannel> pChannel = std::make_shared<NodeVideoStreamChannel>(*pEngine, context);
                pEngine->m_channels.push_back(pChannel);
                pChannel->setWeakToSelf(pChannel);
                VideoCanvas canvas;
                canvas.uid = uid;
                canvas.renderMode = RENDER_MODE_HIDDEN;
                canvas.view = (view_t)pChannel.get();
                pEngine->m_engine->setupRemoteVideo(canvas);
            } while (false);
            LOG_LEAVE;
        }

        NAPI_API_DEFINE(NodeRtcEngine, unsubscribe)
        {
            LOG_ENTER;
            do{
                NodeRtcEngine *pEngine = nullptr;
                napi_status status = napi_ok;
                napi_get_native_this(args, pEngine);
                CHECK_NATIVE_THIS(pEngine);
                uid_t uid;
                status = NodeUid::getUidFromNodeValue(args[0], uid);
                CHECK_NAPI_STATUS(status);
                
                bool result = pEngine->removeChannelByUid(uid);
                args.GetReturnValue().Set(result);
            } while (false);
            LOG_LEAVE;
        }

        NAPI_API_DEFINE(NodeRtcEngine, setupLocalVideo)
        {
            LOG_ENTER;
            do {
                NodeRtcEngine *pEngine = nullptr;
                napi_status status = napi_ok;
                napi_get_native_this(args, pEngine);
                CHECK_NATIVE_THIS(pEngine);
                auto localChannel = pEngine->findChannelByUid(0);
                if (!localChannel) {
                    channel_context context;
                    std::shared_ptr<NodeVideoStreamChannel> pChannel = std::make_shared<NodeVideoStreamChannel>(*pEngine, context);
                    pEngine->m_channels.push_back(pChannel);
                    pChannel->setWeakToSelf(pChannel);
                    VideoCanvas canvas;
                    canvas.uid = 0;
                    canvas.renderMode = RENDER_MODE_HIDDEN;
                    canvas.view = (view_t)pChannel.get();
                    pEngine->m_engine->setupLocalVideo(canvas);
                }
            } while (false);
            LOG_LEAVE;
        }

        NAPI_API_DEFINE(NodeRtcEngine, muteRemoteVideoStream)
        {
            LOG_ENTER;
            do {
                NodeRtcEngine *pEngine = nullptr;
                napi_status status = napi_ok;
                napi_get_native_this(args, pEngine);
                CHECK_NATIVE_THIS(pEngine);
                uid_t uid;
                bool mute;
                status = NodeUid::getUidFromNodeValue(args[0], uid);
                CHECK_NAPI_STATUS(status);
                status = napi_get_value_bool_(args[1], mute);
                CHECK_NAPI_STATUS(status);

                RtcEngineParameters rep(pEngine->m_engine);
                int result = rep.muteRemoteVideoStream(uid, mute);
                napi_set_int_result(args, result);
            } while (false);
            LOG_LEAVE;
        }

        NAPI_API_DEFINE(NodeRtcEngine, setRemoteVideoStreamType)
        {
            LOG_ENTER;
            do {
                NodeRtcEngine *pEngine = nullptr;
                napi_status status = napi_ok;
                napi_get_native_this(args, pEngine);
                CHECK_NATIVE_THIS(pEngine);

                uid_t uid;
                int streamType;
                status = NodeUid::getUidFromNodeValue(args[0], uid);
                CHECK_NAPI_STATUS(status);
                status = napi_get_value_int32_(args[1], streamType);
                CHECK_NAPI_STATUS(status);

                RtcEngineParameters rep(pEngine->m_engine);
                int result = rep.setRemoteVideoStreamType(uid, REMOTE_VIDEO_STREAM_TYPE(streamType));
                napi_set_int_result(args, result);
            } while (false);
            LOG_LEAVE;
        }

        NAPI_API_DEFINE(NodeRtcEngine, setRemoteDefaultVideoStreamType)
        {
            LOG_ENTER;
            do {
                NodeRtcEngine *pEngine;
                napi_status status = napi_ok;
                int streamType;
                napi_get_native_this(args, pEngine);
                CHECK_NATIVE_THIS(pEngine);
                napi_get_param_1(args, int32, streamType);
                CHECK_NAPI_STATUS(status);

                RtcEngineParameters rep(pEngine->m_engine);
                int result = rep.setRemoteDefaultVideoStreamType(REMOTE_VIDEO_STREAM_TYPE(streamType));
                napi_set_int_result(args, result);
            } while (false);
            LOG_LEAVE;
        }

        NAPI_API_DEFINE(NodeRtcEngine, enableAudioVolumeIndication)
        {
            LOG_ENTER;
            do {
                NodeRtcEngine *pEngine = nullptr;
                napi_status status = napi_ok;
                int interval, smooth;
                napi_get_native_this(args, pEngine);
                CHECK_NATIVE_THIS(pEngine);
                napi_get_param_2(args, int32, interval, int32, smooth);
                CHECK_NAPI_STATUS(status);

                RtcEngineParameters rep(pEngine->m_engine);
                int result = rep.enableAudioVolumeIndication(interval, smooth);
                napi_set_int_result(args, result);
            } while (false);
            LOG_LEAVE;
        }

        NAPI_API_DEFINE(NodeRtcEngine, startAudioRecording)
        {
            LOG_ENTER;
            NodeString filePath;
            do {
                NodeRtcEngine *pEngine = nullptr;
                napi_status status = napi_ok;
                napi_get_native_this(args, pEngine);
                CHECK_NATIVE_THIS(pEngine);

                status = napi_get_value_nodestring_(args[0], filePath);
                CHECK_NAPI_STATUS(status);
                int quality;
                status = napi_get_value_int32_(args[1], quality);
                RtcEngineParameters rep(pEngine->m_engine);
                int result = rep.startAudioRecording(filePath, AUDIO_RECORDING_QUALITY_TYPE(quality));
                napi_set_int_result(args, result);
            } while (false);

            LOG_LEAVE;
        }

        NAPI_API_DEFINE(NodeRtcEngine, startAudioMixing)
        {
            LOG_ENTER;
            do {
                NodeRtcEngine *pEngine = nullptr;
                napi_status status = napi_ok;
                NodeString filepath;
                bool loopback, replace;
                int cycle;
                napi_get_native_this(args, pEngine);
                CHECK_NATIVE_THIS(pEngine);
                napi_get_param_4(args, nodestring, filepath, bool, loopback, bool, replace, int32, cycle);
                CHECK_NAPI_STATUS(status);
                RtcEngineParameters rep(pEngine->m_engine);
                int result = rep.startAudioMixing(filepath, loopback, replace, cycle);
                napi_set_int_result(args, result);
            } while (false);
            LOG_LEAVE;
        }

        NAPI_API_DEFINE(NodeRtcEngine, getAudioMixingDuration)
        {
            LOG_ENTER;
            do {
                NodeRtcEngine *pEngine = nullptr;
                napi_status status = napi_ok;
                napi_get_native_this(args, pEngine);
                CHECK_NATIVE_THIS(pEngine);

                RtcEngineParameters rep(pEngine->m_engine);
                int duration = rep.getAudioMixingDuration();
                napi_set_int_result(args, duration);
            } while (false);
            LOG_LEAVE;
        }

        NAPI_API_DEFINE(NodeRtcEngine, getAudioMixingCurrentPosition)
        {
            LOG_ENTER;
            do {
                NodeRtcEngine *pEngine = nullptr;
                napi_status status = napi_ok;
                napi_get_native_this(args, pEngine);
                CHECK_NATIVE_THIS(pEngine);

                RtcEngineParameters rep(pEngine->m_engine);
                int position = rep.getAudioMixingCurrentPosition();
                napi_set_int_result(args, position);
            } while (false);
            LOG_LEAVE;
        }

        NAPI_API_DEFINE(NodeRtcEngine, setRecordingAudioFrameParameters)
        {
            LOG_ENTER;
            do {
                NodeRtcEngine *pEngine = nullptr;
                napi_status status = napi_ok;
                napi_get_native_this(args, pEngine);
                CHECK_NATIVE_THIS(pEngine);
                int sampleRate, channel, mode, samplesPerCall;
                napi_get_param_4(args, int32, sampleRate, int32, channel, int32, mode, int32, samplesPerCall);
                CHECK_NAPI_STATUS(status);

                RtcEngineParameters rep(pEngine->m_engine);
                int result = rep.setRecordingAudioFrameParameters(sampleRate, channel, RAW_AUDIO_FRAME_OP_MODE_TYPE(mode), samplesPerCall);
                napi_set_int_result(args, result);
            } while (false);
            LOG_LEAVE;
        }

        NAPI_API_DEFINE(NodeRtcEngine, setPlaybackAudioFrameParameters)
        {
            LOG_ENTER;
            do {
                NodeRtcEngine *pEngine = nullptr;
                napi_status status = napi_ok;
                int sampleRate, channel, mode, samplesPerCall;
                napi_get_native_this(args, pEngine);
                CHECK_NATIVE_THIS(pEngine);
                napi_get_param_4(args, int32, sampleRate, int32, channel, int32, mode, int32, samplesPerCall);
                CHECK_NAPI_STATUS(status);

                RtcEngineParameters rep(pEngine->m_engine);
                int result = rep.setPlaybackAudioFrameParameters(sampleRate, channel, RAW_AUDIO_FRAME_OP_MODE_TYPE(mode), samplesPerCall);
                napi_set_int_result(args, result);
            } while (false);
            LOG_LEAVE;
        }

        NAPI_API_DEFINE(NodeRtcEngine, setMixedAudioFrameParameters)
        {
            LOG_ENTER;
            do {
                NodeRtcEngine *pEngine = nullptr;
                napi_status status = napi_ok;
                int sampleRate, samplesPerCall;
                napi_get_native_this(args, pEngine);
                CHECK_NATIVE_THIS(pEngine);
                napi_get_param_2(args, int32, sampleRate, int32, samplesPerCall);
                CHECK_NAPI_STATUS(status);

                RtcEngineParameters rep(pEngine->m_engine);
                int result = rep.setMixedAudioFrameParameters(sampleRate, samplesPerCall);
                napi_set_int_result(args, result);
            } while (false);
            LOG_LEAVE;
        }

        NAPI_API_DEFINE(NodeRtcEngine, setHighQualityAudioParameters)
        {
            LOG_ENTER;
            do {
                NodeRtcEngine *pEngine = nullptr;
                napi_status status = napi_ok;
                bool fullband, stereo, fullBitrate;
                napi_get_native_this(args, pEngine);
                CHECK_NATIVE_THIS(pEngine);
                napi_get_param_3(args, bool, fullband, bool, stereo, bool, fullBitrate);
                CHECK_NAPI_STATUS(status);

                RtcEngineParameters rep(pEngine->m_engine);
                int result = rep.setHighQualityAudioParameters(fullband, stereo, fullBitrate);
                napi_set_int_result(args, result);
            } while (false);
            LOG_LEAVE;
        }
#if defined(__APPLE__) || defined(_WIN32)
        NAPI_API_DEFINE(NodeRtcEngine, startScreenCapture)
        {
            LOG_ENTER;
            do {
                NodeRtcEngine *pEngine = nullptr;
                napi_status status = napi_ok;
                napi_get_native_this(args, pEngine);
                CHECK_NATIVE_THIS(pEngine);

                int captureFreq, bitrate;
                int top, left, bottom, right;

#if defined(__APPLE__)
                unsigned int windowId;
                status = napi_get_value_uint32_(args[0], windowId);
                CHECK_NAPI_STATUS(status);
#elif defined(_WIN32)
#if defined(_WIN64)
                int64_t wid;
                status = napi_get_value_int64_(args[0], wid);
#else
                int32_t wid;
                status = napi_get_value_int32_(args[0], wid);
#endif

                CHECK_NAPI_STATUS(status);
                HWND windowId = (HWND)wid;
#endif
                status = napi_get_value_int32_(args[1], captureFreq);
                CHECK_NAPI_STATUS(status);
                
                Local<Object> rect = args[2]->ToObject(args.GetIsolate());
                Local<Name> topKey = String::NewFromUtf8(args.GetIsolate(), "top", NewStringType::kInternalized).ToLocalChecked();
                Local<Value> topValue = rect->Get(args.GetIsolate()->GetCurrentContext(), topKey).ToLocalChecked();
                top = topValue->Int32Value();

                Local<Name> leftKey = String::NewFromUtf8(args.GetIsolate(), "left", NewStringType::kInternalized).ToLocalChecked();
                Local<Value> leftValue = rect->Get(args.GetIsolate()->GetCurrentContext(), leftKey).ToLocalChecked();
                left = leftValue->Int32Value();

                Local<Name> bottomKey = String::NewFromUtf8(args.GetIsolate(), "bottom", NewStringType::kInternalized).ToLocalChecked();
                Local<Value> bottomValue = rect->Get(args.GetIsolate()->GetCurrentContext(), bottomKey).ToLocalChecked();
                bottom = bottomValue->Int32Value();

                Local<Name> rightKey = String::NewFromUtf8(args.GetIsolate(), "right", NewStringType::kInternalized).ToLocalChecked();
                Local<Value> rightValue = rect->Get(args.GetIsolate()->GetCurrentContext(), rightKey).ToLocalChecked();
                right = rightValue->Int32Value();

                status = napi_get_value_int32_(args[3], bitrate);
                CHECK_NAPI_STATUS(status);

                Rect region(top, left, bottom, right);
                int result = pEngine->m_engine->startScreenCapture(windowId, captureFreq, &region, bitrate);
                napi_set_int_result(args, result);
            } while (false);
            LOG_LEAVE;
        }

        NAPI_API_DEFINE(NodeRtcEngine, updateScreenCaptureRegion)
        {
            LOG_ENTER;
            do {
                NodeRtcEngine *pEngine = nullptr;
                napi_status status = napi_ok;
                napi_get_native_this(args, pEngine);
                CHECK_NATIVE_THIS(pEngine);

                int top, left, bottom, right; 
                Local<Object> rect = args[0]->ToObject(args.GetIsolate());
                Local<Name> topKey = String::NewFromUtf8(args.GetIsolate(), "top", NewStringType::kInternalized).ToLocalChecked();
                Local<Value> topValue = rect->Get(args.GetIsolate()->GetCurrentContext(), topKey).ToLocalChecked();
                top = topValue->Int32Value();

                Local<Name> leftKey = String::NewFromUtf8(args.GetIsolate(), "left", NewStringType::kInternalized).ToLocalChecked();
                Local<Value> leftValue = rect->Get(args.GetIsolate()->GetCurrentContext(), leftKey).ToLocalChecked();
                left = leftValue->Int32Value();

                Local<Name> bottomKey = String::NewFromUtf8(args.GetIsolate(), "bottom", NewStringType::kInternalized).ToLocalChecked();
                Local<Value> bottomValue = rect->Get(args.GetIsolate()->GetCurrentContext(), bottomKey).ToLocalChecked();
                bottom = bottomValue->Int32Value();

                Local<Name> rightKey = String::NewFromUtf8(args.GetIsolate(), "right", NewStringType::kInternalized).ToLocalChecked();
                Local<Value> rightValue = rect->Get(args.GetIsolate()->GetCurrentContext(), rightKey).ToLocalChecked();
                right = rightValue->Int32Value();
                Rect region(top, left, bottom, right);
                pEngine->m_engine->updateScreenCaptureRegion(&region);                
            } while (false);
            LOG_LEAVE;
        }
#endif

        NAPI_API_DEFINE(NodeRtcEngine, onEvent)
        {
            LOG_ENTER;
            do {
                NodeRtcEngine *pEngine = nullptr;
                napi_status status = napi_ok;
                napi_get_native_this(args, pEngine);
                CHECK_NATIVE_THIS(pEngine);

                NodeString eventName;
                status = napi_get_value_nodestring_(args[0], eventName);
                CHECK_NAPI_STATUS(status);

                if (!args[1]->IsFunction()) {
                    LOG_ERROR("Function expected");
                    break;
                }

                Local<Function> callback = args[1].As<Function>();
                if (callback.IsEmpty()) {
                    LOG_ERROR("Function expected.");
                    break;
                }
                Persistent<Function> persist;
                persist.Reset(args.GetIsolate(), callback);
                Local<Object> obj = args.This();
                Persistent<Object> persistObj;
                persistObj.Reset(args.GetIsolate(), obj);
                pEngine->m_eventHandler->addEventHandler((char*)eventName, persistObj, persist);
            } while (false);
            LOG_LEAVE;
        }


        NAPI_API_DEFINE(NodeRtcEngine, getVideoDevices)
        {
            LOG_ENTER;
            do {
                NodeRtcEngine *pEngine = nullptr;
                napi_status status = napi_ok;
                napi_get_native_this(args, pEngine);
                CHECK_NATIVE_THIS(pEngine);
                AVideoDeviceManager vdm(pEngine->m_engine);
                auto vdc = vdm->enumerateVideoDevices();
                int count = vdc->getCount();
                Local<v8::Array> devices = v8::Array::New(args.GetIsolate(), count);
                char deviceName[MAX_DEVICE_ID_LENGTH] = { 0 };
                char deviceId[MAX_DEVICE_ID_LENGTH] = { 0 };
                for (int i = 0; i < count; i++) {
                    Local<v8::Object> dev = v8::Object::New(args.GetIsolate());
                    vdc->getDevice(i, deviceName, deviceId);
                    auto dn = v8::String::NewFromUtf8(args.GetIsolate(), deviceName, NewStringType::kInternalized).ToLocalChecked();
                    auto di = v8::String::NewFromUtf8(args.GetIsolate(), deviceId, NewStringType::kInternalized).ToLocalChecked();
                    dev->Set(v8::String::NewFromUtf8(args.GetIsolate(), "devicename", NewStringType::kInternalized).ToLocalChecked(), dn);
                    dev->Set(v8::String::NewFromUtf8(args.GetIsolate(), "deviceid", NewStringType::kInternalized).ToLocalChecked(), di);
                    devices->Set(i, dev);
                    deviceName[0] = '\0';
                    deviceId[0] = '\0';
                }
                args.GetReturnValue().Set(devices);
            } while (false);
            LOG_LEAVE;
        }

        NAPI_API_DEFINE(NodeRtcEngine, setVideoDevice)
        {
            LOG_ENTER;
            do {
                NodeRtcEngine *pEngine = nullptr;
                napi_status status = napi_ok;
                napi_get_native_this(args, pEngine);
                CHECK_NATIVE_THIS(pEngine);
                NodeString deviceId;
                status = napi_get_value_nodestring_(args[0], deviceId);
                CHECK_NAPI_STATUS(status);
                AVideoDeviceManager vdm(pEngine->m_engine);
                int result = vdm->setDevice(deviceId);
                napi_set_int_result(args, result);
            } while (false);
            LOG_LEAVE;
        }

        NAPI_API_DEFINE(NodeRtcEngine, getCurrentVideoDevice)
        {
            LOG_ENTER;
            do {
                NodeRtcEngine *pEngine = nullptr;
                napi_status status = napi_ok;
                napi_get_native_this(args, pEngine);
                CHECK_NATIVE_THIS(pEngine);
                char deviceId[MAX_DEVICE_ID_LENGTH] = { 0 };
                AVideoDeviceManager vdm(pEngine->m_engine);
                vdm->getDevice(deviceId);
                napi_set_string_result(args, deviceId);
            } while (false);
            LOG_LEAVE;
        }

        NAPI_API_DEFINE(NodeRtcEngine, startVideoDeviceTest)
        {
            LOG_ENTER;
            do {
                NodeRtcEngine *pEngine = nullptr;
                napi_status status = napi_ok;
                napi_get_native_this(args, pEngine);
                CHECK_NATIVE_THIS(pEngine);
                auto localChannel = pEngine->findChannelByUid(0);
                if (!localChannel) {
                    channel_context context;
                    std::shared_ptr<NodeVideoStreamChannel> pChannel = std::make_shared<NodeVideoStreamChannel>(*pEngine, context);
                    pEngine->m_channels.push_back(pChannel);
                    localChannel = pChannel.get();
                }
                AVideoDeviceManager vdm(pEngine->m_engine);
                vdm->startDeviceTest(localChannel);
            } while (false);
            LOG_LEAVE;
        }

        NAPI_API_DEFINE(NodeRtcEngine, stopVideoDeviceTest)
        {
            LOG_ENTER;
            do {
                NodeRtcEngine *pEngine = nullptr;
                napi_status status = napi_ok;
                napi_get_native_this(args, pEngine);
                CHECK_NATIVE_THIS(pEngine);
                AVideoDeviceManager vdm(pEngine->m_engine);
                vdm->stopDeviceTest();
            } while (false);
            LOG_LEAVE;
        }

        NAPI_API_DEFINE(NodeRtcEngine, getAudioPlaybackDevices)
        {
            LOG_ENTER;
            do {
                NodeRtcEngine *pEngine = nullptr;
                napi_status status = napi_ok;
                napi_get_native_this(args, pEngine);
                CHECK_NATIVE_THIS(pEngine);
                AAudioDeviceManager adm(pEngine->m_engine);
                auto pdc = adm->enumeratePlaybackDevices();
                int count = pdc->getCount();
                Local<v8::Array> devices = v8::Array::New(args.GetIsolate(), count);
                char deviceName[MAX_DEVICE_ID_LENGTH] = { 0 };
                char deviceId[MAX_DEVICE_ID_LENGTH] = { 0 };
                for (int i = 0; i < count; i++) {
                    Local<v8::Object> dev = v8::Object::New(args.GetIsolate());
                    pdc->getDevice(i, deviceName, deviceId);
                    auto dn = v8::String::NewFromUtf8(args.GetIsolate(), deviceName, NewStringType::kInternalized).ToLocalChecked();
                    auto di = v8::String::NewFromUtf8(args.GetIsolate(), deviceId, NewStringType::kInternalized).ToLocalChecked();
                    dev->Set(v8::String::NewFromUtf8(args.GetIsolate(), "devicename", NewStringType::kInternalized).ToLocalChecked(), dn);
                    dev->Set(v8::String::NewFromUtf8(args.GetIsolate(), "deviceid", NewStringType::kInternalized).ToLocalChecked(), di);
                    devices->Set(i, dev);
                    deviceName[0] = '\0';
                    deviceId[0] = '\0';
                }
                args.GetReturnValue().Set(devices);
            } while (false);
            LOG_LEAVE;
        }

        NAPI_API_DEFINE(NodeRtcEngine, setAudioPlaybackDevice)
        {
            LOG_ENTER;
            do {
                NodeRtcEngine *pEngine = nullptr;
                napi_status status = napi_ok;
                napi_get_native_this(args, pEngine);
                CHECK_NATIVE_THIS(pEngine);
                NodeString deviceId;
                status = napi_get_value_nodestring_(args[0], deviceId);
                CHECK_NAPI_STATUS(status);
                AAudioDeviceManager adm(pEngine->m_engine);
                int result = adm->setPlaybackDevice(deviceId);
                napi_set_int_result(args, result);
            } while (false);
            LOG_LEAVE;
        }

        NAPI_API_DEFINE(NodeRtcEngine, getCurrentAudioPlaybackDevice)
        {
            LOG_ENTER;
            do {
                NodeRtcEngine *pEngine = nullptr;
                napi_status status = napi_ok;
                napi_get_native_this(args, pEngine);
                CHECK_NATIVE_THIS(pEngine);
                char deviceId[MAX_DEVICE_ID_LENGTH] = { 0 };
                AAudioDeviceManager adm(pEngine->m_engine);
                adm->getPlaybackDevice(deviceId);
                napi_set_string_result(args, deviceId);
            } while (false);
            LOG_LEAVE;
        }

        NAPI_API_DEFINE(NodeRtcEngine, setAudioPlaybackVolume)
        {
            LOG_ENTER;
            do {
                NodeRtcEngine *pEngine = nullptr;
                napi_status status = napi_ok;
                napi_get_native_this(args, pEngine);
                CHECK_NATIVE_THIS(pEngine);
                int volume;
                status = napi_get_value_int32_(args[0], volume);
                CHECK_NAPI_STATUS(status);
                AAudioDeviceManager adm(pEngine->m_engine);
                int result = adm->setPlaybackDeviceVolume(volume);
                napi_set_int_result(args, result);
            } while (false);
            LOG_LEAVE;
        }

        NAPI_API_DEFINE(NodeRtcEngine, getAudioPlaybackVolume)
        {
            LOG_ENTER;
            do {
                NodeRtcEngine *pEngine = nullptr;
                napi_status status = napi_ok;
                napi_get_native_this(args, pEngine);
                CHECK_NATIVE_THIS(pEngine);
                int volume;
                AAudioDeviceManager adm(pEngine->m_engine);
                adm->getPlaybackDeviceVolume(&volume);
                napi_set_int_result(args, volume);
            } while (false);
            LOG_LEAVE;
        }

        NAPI_API_DEFINE(NodeRtcEngine, getAudioRecordingDevices)
        {
            LOG_ENTER;
            do {
                NodeRtcEngine *pEngine = nullptr;
                napi_status status = napi_ok;
                napi_get_native_this(args, pEngine);
                CHECK_NATIVE_THIS(pEngine);
                AAudioDeviceManager adm(pEngine->m_engine);
                auto pdc = adm->enumerateRecordingDevices();
                int count = pdc->getCount();
                Local<v8::Array> devices = v8::Array::New(args.GetIsolate(), count);
                char deviceName[MAX_DEVICE_ID_LENGTH] = { 0 };
                char deviceId[MAX_DEVICE_ID_LENGTH] = { 0 };
                for (int i = 0; i < count; i++) {
                    Local<v8::Object> dev = v8::Object::New(args.GetIsolate());
                    pdc->getDevice(i, deviceName, deviceId);
                    auto dn = v8::String::NewFromUtf8(args.GetIsolate(), deviceName, NewStringType::kInternalized).ToLocalChecked();
                    auto di = v8::String::NewFromUtf8(args.GetIsolate(), deviceId, NewStringType::kInternalized).ToLocalChecked();
                    dev->Set(v8::String::NewFromUtf8(args.GetIsolate(), "devicename", NewStringType::kInternalized).ToLocalChecked(), dn);
                    dev->Set(v8::String::NewFromUtf8(args.GetIsolate(), "deviceid", NewStringType::kInternalized).ToLocalChecked(), di);
                    devices->Set(i, dev);
                    deviceName[0] = '\0';
                    deviceId[0] = '\0';
                }
                args.GetReturnValue().Set(devices);
            } while (false);
            LOG_LEAVE;
        }

        NAPI_API_DEFINE(NodeRtcEngine, setAudioRecordingDevice)
        {
            LOG_ENTER;
            do {
                NodeRtcEngine *pEngine = nullptr;
                napi_status status = napi_ok;
                napi_get_native_this(args, pEngine);
                CHECK_NATIVE_THIS(pEngine);
                NodeString deviceId;
                status = napi_get_value_nodestring_(args[0], deviceId);
                CHECK_NAPI_STATUS(status);
                AAudioDeviceManager adm(pEngine->m_engine);
                int result = adm->setRecordingDevice(deviceId);
                napi_set_int_result(args, result);
            } while (false);
            LOG_LEAVE;
        }

        NAPI_API_DEFINE(NodeRtcEngine, getCurrentAudioRecordingDevice)
        {
            LOG_ENTER;
            do {
                NodeRtcEngine *pEngine = nullptr;
                napi_status status = napi_ok;
                napi_get_native_this(args, pEngine);
                CHECK_NATIVE_THIS(pEngine);
                char deviceId[MAX_DEVICE_ID_LENGTH] = { 0 };
                AAudioDeviceManager adm(pEngine->m_engine);
                adm->getRecordingDevice(deviceId);
                napi_set_string_result(args, deviceId);
            } while (false);
            LOG_LEAVE;
        }

        NAPI_API_DEFINE(NodeRtcEngine, getAudioRecordingVolume)
        {
            LOG_ENTER;
            do {
                NodeRtcEngine *pEngine = nullptr;
                napi_status status = napi_ok;
                napi_get_native_this(args, pEngine);
                CHECK_NATIVE_THIS(pEngine);
                int volume;
                AAudioDeviceManager adm(pEngine->m_engine);
                adm->getRecordingDeviceVolume(&volume);
                napi_set_int_result(args, volume);
            } while (false);
            LOG_LEAVE;
        }

        NAPI_API_DEFINE(NodeRtcEngine, setAudioRecordingVolume)
        {
            LOG_ENTER;
            do {
                NodeRtcEngine *pEngine = nullptr;
                napi_status status = napi_ok;
                napi_get_native_this(args, pEngine);
                CHECK_NATIVE_THIS(pEngine);
                int volume;
                status = napi_get_value_int32_(args[0], volume);
                CHECK_NAPI_STATUS(status);
                AAudioDeviceManager adm(pEngine->m_engine);
                int result = adm->setRecordingDeviceVolume(volume);
                napi_set_int_result(args, result);
            } while (false);
            LOG_LEAVE;
        }

        NAPI_API_DEFINE(NodeRtcEngine, startAudioPlaybackDeviceTest)
        {
            LOG_ENTER;
            do {
                NodeRtcEngine *pEngine = nullptr;
                napi_status status = napi_ok;
                napi_get_native_this(args, pEngine);
                CHECK_NATIVE_THIS(pEngine);
                NodeString filePath;
                status = napi_get_value_nodestring_(args[0], filePath);
                CHECK_NAPI_STATUS(status);
                AAudioDeviceManager adm(pEngine->m_engine);
                int result = adm->startPlaybackDeviceTest(filePath);
                napi_set_int_result(args, result);
            } while (false);
            LOG_LEAVE;
        }

        NAPI_API_DEFINE(NodeRtcEngine, stopAudioPlaybackDeviceTest)
        {
            LOG_ENTER;
            do {
                NodeRtcEngine *pEngine = nullptr;
                napi_status status = napi_ok;
                napi_get_native_this(args, pEngine);
                CHECK_NATIVE_THIS(pEngine);
                AAudioDeviceManager adm(pEngine->m_engine);
                int result = adm->stopPlaybackDeviceTest();
                napi_set_int_result(args, result);
            } while (false);
            LOG_LEAVE;
        }

        NAPI_API_DEFINE(NodeRtcEngine, startAudioRecordingDeviceTest)
        {
            LOG_ENTER;
            do {
                NodeRtcEngine *pEngine = nullptr;
                napi_status status = napi_ok;
                napi_get_native_this(args, pEngine);
                CHECK_NATIVE_THIS(pEngine);
                int indicateInterval;
                status = napi_get_value_int32_(args[0], indicateInterval);
                CHECK_NAPI_STATUS(status);
                AAudioDeviceManager adm(pEngine->m_engine);
                int result = adm->startRecordingDeviceTest(indicateInterval);
                napi_set_int_result(args, result);
            } while (false);
            LOG_LEAVE;
        }

        NAPI_API_DEFINE(NodeRtcEngine, stopAudioRecordingDeviceTest)
        {
            LOG_ENTER;
            do {
                NodeRtcEngine *pEngine = nullptr;
                napi_status status = napi_ok;
                napi_get_native_this(args, pEngine);
                CHECK_NATIVE_THIS(pEngine);
                AAudioDeviceManager adm(pEngine->m_engine);
                int result = adm->stopRecordingDeviceTest();
                napi_set_int_result(args, result);
            } while (false);
            LOG_LEAVE;
        }

        NAPI_API_DEFINE(NodeRtcEngine, getAudioPlaybackDeviceMute)
        {
            LOG_ENTER;
            do {
                NodeRtcEngine *pEngine = nullptr;
                napi_status status = napi_ok;
                napi_get_native_this(args, pEngine);
                CHECK_NATIVE_THIS(pEngine);
                bool mute;
                AAudioDeviceManager adm(pEngine->m_engine);
                adm->getPlaybackDeviceMute(&mute);
                napi_set_bool_result(args, mute);
            } while (false);
            LOG_LEAVE;
        }

            NAPI_API_DEFINE(NodeRtcEngine, setAudioPalybackDeviceMute)
            {
                LOG_ENTER;
                do {
                    NodeRtcEngine *pEngine = nullptr;
                    napi_status status = napi_ok;
                    napi_get_native_this(args, pEngine);
                    CHECK_NATIVE_THIS(pEngine);
                    bool mute;
                    status = napi_get_value_bool_(args[0], mute);
                    CHECK_NAPI_STATUS(status);
                    AAudioDeviceManager adm(pEngine->m_engine);
                    int result = adm->setPlaybackDeviceMute(mute);
                    napi_set_int_result(args, result);
                } while (false);
                LOG_LEAVE;
            }

            NAPI_API_DEFINE(NodeRtcEngine, getAudioRecordingDeviceMute)
            {
                LOG_ENTER;
                do {
                    NodeRtcEngine *pEngine = nullptr;
                    napi_status status = napi_ok;
                    napi_get_native_this(args, pEngine);
                    CHECK_NATIVE_THIS(pEngine);
                    bool mute;
                    AAudioDeviceManager adm(pEngine->m_engine);
                    adm->getRecordingDeviceMute(&mute);
                    napi_set_bool_result(args, mute);
                } while (false);
                LOG_LEAVE;
            }

            NAPI_API_DEFINE(NodeRtcEngine, setAudioRecordingDeviceMute)
            {
                LOG_ENTER;
                do {
                    NodeRtcEngine *pEngine = nullptr;
                    napi_status status = napi_ok;
                    napi_get_native_this(args, pEngine);
                    CHECK_NATIVE_THIS(pEngine);
                    bool mute;
                    status = napi_get_value_bool_(args[0], mute);
                    CHECK_NAPI_STATUS(status);
                    AAudioDeviceManager adm(pEngine->m_engine);
                    int result = adm->setRecordingDeviceMute(mute);
                    napi_set_int_result(args, result);
                } while (false);
                LOG_LEAVE;
            }

    }
}