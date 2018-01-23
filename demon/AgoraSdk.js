const EventEmitter = require("events");
const agora = require("./agora_node_ext");
require("./webgl-utils")
require("./AgoraRender2")

class AgoraRtcEngine extends EventEmitter {
    constructor() {
        super();
        this.rtcengine = new agora.NodeRtcEngine();
        this.initEventHandler();
        this.streams = {};
    }

    initEventHandler() {
        var self = this;
        this.rtcengine.onEvent("joinchannel", function (channel, uid, elapsed) {
            self.emit("joinedchannel", channel, uid, elapsed);
        });

        this.rtcengine.onEvent("rejoinchannel", function (channel, uid, elapsed) {
            self.emit("rejoinedchannel", channel, uid, elapsed);
        });

        this.rtcengine.onEvent("warning", function (warn, msg) {
            self.emit("warning", warn, msg);
        });

        this.rtcengine.onEvent("error", function (err, msg) {
            self.emit("error", err, msg);
        });

        this.rtcengine.onEvent("audioquality", function (uid, quality, delay, lost) {
            self.emit("audioquality", uid, quality, delay, lost);
        });

        this.rtcengine.onEvent("audiovolumeindication", function (uid, volume, speakerNumber, totalVolume) {
            self.emit("audiovolumeindication", uid, volume, speakerNumber, totalVolume);
        });

        this.rtcengine.onEvent("leavechannel", function () {
            self.emit("leavechannel");
        });

        this.rtcengine.onEvent("audiodevicestatechanged", function (deviceId, deviceType, deviceState) {
            self.emit("audiodevicestatechanged", deviceId, deviceType, deviceState);
        });

        this.rtcengine.onEvent("audiomixingfinished", function () {
            self.emit("audiomixingfinished");
        });

        this.rtcengine.onEvent("apicallexecuted", function (api, err) {
            self.emit("apicallexecuted", api, err);
        });

        this.rtcengine.onEvent("remoteaudiomixingbegin", function () {
            self.emit("remoteaudiomixingbegin");
        });

        this.rtcengine.onEvent("remoteaudiomixingend", function () {
            self.emit("remoteaudiomixingend");
        });

        this.rtcengine.onEvent("audioeffectfinished", function (soundId) {
            self.emit("audioeffectfinished", soundId);
        });

        this.rtcengine.onEvent("videodevicestatechanged", function (deviceId, deviceType, deviceState) {
            self.emit("videodevicestatechanged", deviceId, deviceType, deviceState);
        });

        this.rtcengine.onEvent("networkquality", function (uid, txquality, rxquality) {
            self.emit("networkquality", uid, txquality, rxquality);
        });

        this.rtcengine.onEvent("lastmilequality", function (quality) {
            self.emit("lastmilequality", quality);
        });

        this.rtcengine.onEvent("firstlocalvideoframe", function (width, height, elapsed) {
            self.emit("firstlocalvideoframe", width, height, elapsed);
        });

        this.rtcengine.onEvent("firstremotevideodecoded", function (uid, width, height, elapsed) {
            //self.emit("addstream", uid, width, height, elapsed);
        });

        this.rtcengine.onEvent("videosizechanged", function (uid, width, height, rotation) {
            self.emit("videosizechanged", uid, width, height, rotation);
        });

        this.rtcengine.onEvent("firstremotevideoframe", function (uid, width, height, elapsed) {
            self.emit("firstremotevideoframe", uid, width, height, elapsed);
        });

        this.rtcengine.onEvent("userjoined", function (uid, elapsed) {
            console.log("user : " + uid + " joined.");
            //self.emit("userjoined", uid, elapsed);
            self.emit("addstream", uid, elapsed);
        });

        this.rtcengine.onEvent("useroffline", function (uid, reason) {
          if(!self.streams){
            self.streams = {};
            console.log("Warning!!!!!!, streams is undefined.");
            return;
          }
            self.streams[uid] = undefined;
            self.rtcengine.unsubscribe(uid);
            self.emit("removestream", uid, reason);
        });

        this.rtcengine.onEvent("usermuteaudio", function (uid, muted) {
            self.emit("usermuteaudio", uid, muted);
        });

        this.rtcengine.onEvent("usermutevideo", function (uid, muted) {
            self.emit("usermutevideo", uid, muted);
        });

        this.rtcengine.onEvent("userenablevideo", function (uid, enabled) {
            self.emit("userenablevideo", uid, enabled);
        });

        this.rtcengine.onEvent("userenablelocalvideo", function (uid, enabled) {
            self.emit("userenablelocalvideo", uid, enabled);
        });

        this.rtcengine.onEvent("apicallexecuted", function (api, err) {
            self.emit("apicallexecuted", api, err);
        });

        this.rtcengine.onEvent("cameraready", function () {
            self.emit("cameraready");
        });

        this.rtcengine.onEvent("videostopped", function () {
            self.emit("videostopped");
        });

        this.rtcengine.onEvent("connectionlost", function () {
            self.emit("connectionlost");
        });

        this.rtcengine.onEvent("connectioninterrupted", function () {
            self.emit("connectioninterrupted");
        });

        this.rtcengine.onEvent("connectionbanned", function () {
            self.emit("connectionbanned");
        });

        this.rtcengine.onEvent("refreshrecordingservicestatus", function (status) {
            self.emit("refreshrecordingservicestatus", status);
        });

        this.rtcengine.onEvent("streammessage", function (uid, streamId, msg, len) {
            self.emit("streammessage", uid, streamId, msg, len);
        });

        this.rtcengine.onEvent("streammessageerror", function (uid, streamid, code, missed, cached) {
            self.emit("streammessageerror", uid, streamId, code, missed, cached);
        });

        this.rtcengine.onEvent("mediaenginestartcallsuccess", function () {
            self.emit("mediaenginestartcallsuccess");
        });

        this.rtcengine.onEvent("requestchannelkey", function () {
            self.emit("requestchannelkey");
        });

        this.rtcengine.onEvent("fristlocalaudioframe", function (elapsed) {
            self.emit("firstlocalaudioframe", elapsed);
        });

        this.rtcengine.onEvent("firstremoteaudioframe", function (uid, elapsed) {
            self.emit("firstremoteaudioframe", uid, elapsed);
        });

        this.rtcengine.onEvent("activespeaker", function (uid) {
            self.emit("activespeaker", uid);
        });

        this.rtcengine.onEvent("clientrolechanged", function (oldRole, newRole) {
            self.emit("clientrolechanged", oldRole, newRole);
        });

        this.rtcengine.onEvent("audiodevicevolumechanged", function (deviceType, volume, muted) {
            self.emit("audiodevicevolumechanged", deviceType, volume, muted);
        });

        this.rtcengine.registerDeliverFrame(function (uid, header, ydata, udata, vdata) {
            if (!header || !ydata || !udata || !vdata) {
                console.log("Invalid data param ： " + header + " " + ydata + " " + udata + " " + vdata);
                return;
            }
            var render = null;
            if (uid == 0) {
                render = self.streams["local"];
            } else {
                render = self.streams[uid];
            }
            if (!render) {
                console.log("Can't find render for uid : " + uid);
                return;
            }
            self.drawImage(render, header, ydata, udata, vdata);
        });
    }

    drawImage(render, header, yplanedata, uplanedata, vplanedata) {
        //console.log("try to drawImage");
        if (header.byteLength != 20) {  //
            console.error('invalid image header ' + header.byteLength);
            return;
        }

        if (yplanedata.byteLength === 20) {
            console.error('invalid image yplane ' + yplane.byteLength);
            return
        }

        if (uplanedata.byteLength === 20) {
            console.error('invalid image uplanedata ' + uplanedata.byteLength);
            return
        }

        if (yplanedata.byteLength != uplanedata.byteLength * 4
            || uplanedata.byteLength != vplanedata.byteLength
        ) {
            console.error('invalid image header ' + yplanedata.byteLength + ' ' + uplanedata.byteLength + ' ' + vplanedata.byteLength);
            return;
        }
        var headerLength = 20;
        var dv = new DataView(header);
        var format = dv.getUint8(0);
        var mirror = dv.getUint8(1);
        var width = dv.getUint16(2);
        var height = dv.getUint16(4);
        var left = dv.getUint16(6);
        var top = dv.getUint16(8);
        var right = dv.getUint16(10);
        var bottom = dv.getUint16(12);
        var rotation = dv.getUint16(14);
        var ts = dv.getUint32(16);

        var xWidth = width + left + right;
        var xHeight = height + top + bottom;
        var yLength = xWidth * xHeight;
        var yBegin = headerLength;
        var yEnd = yBegin + yLength;

        var uLength = yLength / 4;
        var uBegin = yEnd;
        var uEnd = uBegin + uLength;

        var vLength = yLength / 4;
        var vBegin = uEnd;
        var vEnd = vBegin + vLength;
        render.renderImage(
            {
                mirror: mirror
                , width: width
                , height: height
                , left: left
                , top: top
                , right: right
                , bottom: bottom
                , rotation: rotation
                , yplane: new Uint8Array(yplanedata)
                , uplane: new Uint8Array(uplanedata)
                , vplane: new Uint8Array(vplanedata)
            }
        );
        var now32 = (Date.now() & 0xFFFFFFFF) >>> 0;
        var latency = now32 - ts;
    }

    initRender(view) {
        var render = new AgoraRender2();
        render.start(view, function () {
            console.log("render start fail.");
        });
        return render;
    }

    initialize(appid, onSuccess, onFailed) {
        return this.rtcengine.initialize(appid);
    };

    getVersion() {
        return this.rtcengine.getVersion();
    }

    getErrorDescription(errorCode) {
        return this.rtcengine.getErrorDescription();
    }

    joinChannel(key, name, chan_info, uid) {
        return this.rtcengine.joinChannel(key, name, chan_info, uid);
    }

    leaveChannel() {
        var localRender = this.streams["local"];
        this.streams = {};
        this.streams["local"] = localRender;
        return this.rtcengine.leaveChannel();
    }

    subscribe(uid, view) {
        this.streams[uid] = this.initRender(view);
        return this.rtcengine.subscribe(uid);
    }

    setupLocalVideo(view) {
        this.streams["local"] = this.initRender(view);
        return this.rtcengine.setupLocalVideo();
    }

    renewChannelKey(newKey) {
        return this.rtcengine.renewChannelKey(newKey);
    }

    setChannelProfile(profile) {
        return this.rtcengine.setChannelProfile(profile);
    }

    setClientRole(role, permissionKey) {
        return this.rtcengine.setClientRole(role, permissionKey);
    }

    startEchoTest() {
        return this.rtcengine.startEchoTest();
    }

    stopEchoTest() {
        return this.rtcengine.stopEchoTest();
    }

    enableLastmileTest() {
        return this.rtcengine.enableLastmileTest();
    }

    disableLastmileTest() {
        return this.rtcengine.disableLastmileTest();
    }

    enableVideo() {
        return this.rtcengine.enableVideo();
    }

    disableVideo() {
        return this.rtcengine.disableVideo();
    }

    startPreview() {
        return this.rtcengine.startPreview();
    }

    stopPreview() {
        return this.rtcengine.stopPreview();
    }

    setVideoProfile(profile, swapWidthAndHeight) {
        return this.rtcengine.setVideoProfile(profile, swapWidthAndHeight);
    }

    enableAudio() {
        return this.rtcengine.enableAudio();
    }

    disableAudio() {
        return this.rtcengine.disableAudio();
    }

    setAudioProfile(profile, scenario) {
        return this.rtcengine.setAudioProfile(profile, scenario);
    }

    getCallId() {
        return this.rtcengine.getCallId();
    }

    rate(callid, rating, desc) {
        return this.rtcengine.rate(callid, rating, desc);
    }

    complain(callid, desc) {
        return this.rtcengine.complain(callid, desc);
    }

    setEncryptionSecret(secret) {
        return this.rtcengine.setEncryptionSecret(secret);
    }

    createDataStream(reliable, ordered) {
        return this.rtcengine.createDataStream(reliable, ordered);
    }

    sendStreamMessage(streamId, msg) {
        return this.rtcengine.sendStreamMessage(streamId, msg);
    }

    muteLocalAudioStream(mute) {
        return this.rtcengine.muteLocalAudioStream(mute);
    }

    muteAllRemoteAudioStreams(mute) {
        return this.rtcengine.muteAllRemoteAudioStreams(mute);
    }

    muteRemoteAudioStream(uid, mute) {
        return this.rtcengine.muteRemoteAudioStream(uid, mute);
    }

    muteLocalVideoStream(mute) {
        return this.rtcengine.muteLocalVideoStream(mute);
    }

    enableLocalVideo(enable) {
        return this.rtcengine.enableLocalVideo(enable);
    }

    muteAllRemoteVideoStreams(mute) {
        return this.rtcengine.muteAllRemoteVideoStreams(mute);
    }

    muteRemoteVideoStream(uid, mute) {
        return this.rtcengine.muteRemoteVideoStream(uid, mute);
    }

    setRemoteVideoStreamType(uid, streamType) {
        return this.rtcengine.setRemoteVideoStreamType(uid, streamType);
    }

    setRemoteDefaultVideoStreamType(streamType) {
        return this.rtcengine.setRemoteDefaultVideoStreamType(streamType);
    }

    startAudioRecording(filePath) {
        return this.rtcengine.startAudioRecording(filePath);
    }

    stopAudioRecording() {
        return this.rtcengine.stopAudioRecording();
    }

    startAudioMixing(filepath, loopback, replace, cycle) {
        return this.rtcengine.startAudioMixing(filepath, loopback, replace, cycle);
    }

    stopAudioMixing() {
        return this.rtcengine.stopAudioMixing();
    }

    pauseAudioMixing() {
        return this.rtcengine.pauseAudioMixing();
    }

    resumeAudioMixing() {
        return this.rtcengine.resumeAudioMixing();
    }

    adjustAudioMixingVolume(volume) {
        return this.rtcengine.adjustAudioMixingVolume(volume);
    }

    getAudioMixingDuration() {
        return this.rtcengine.getAudioMixingDuration();
    }

    getAudioMixingCurrentPosistion() {
        return this.rtcengine.getAudioMixingCurrentPosistion();
    }

    setAudioMixingPosition(position) {
        return this.rtcengine.setAudioMixingPosition(position);
    }

    setLocalVoicePitch(pitch) {
        return this.rtcengine.setLocalVoicePitch(pitch);
    }

    setInEarMonitoringVolume(volume) {
        return this.rtcengine.setInEarMonitoringVolume(volume);
    }

    pauseAudio() {
        return this.rtcengine.pauseAudio();
    }

    resumeAudio() {
        return this.rtcengine.resumeAudio();
    }

    stopScreenCapture() {
        return this.rtcengine.stopScreenCapture();
    }

    setLogFile(filepath) {
        return this.rtcengine.setLogFile(filepath);
    }

    setLogFilter(filter) {
        return this.rtcengine.setLogFilter(filter);
    }

    startRecordingService(recordingKey) {
        return this.rtcengine.startRecordingService(recordingKey);
    }

    stopRecordingService(recordingKey) {
        return this.rtcengine.stopRecordingService(recordingKey);
    }

    refreshRecrodingServiceStatus() {
        return this.rtcengine.refreshRecrodingServiceStatus();
    }

    enableDualStreamMode(enable) {
        return this.rtcengine.enableDualStreamMode(enable);
    }

    setRecordingAudioFrameParameters(sampleRate, channel, mode, samplesPerCall) {
        return this.rtcengine.setRecordingAudioFrameParameters(sampleRate, channel, mode, samplesPerCall);
    }

    setPlaybackAudioFrameParameters(sampleRate, channel, mode, sampelsPerCall) {
        return this.rtcengine.setPlaybackAudioFrameParameters(sampleRate, channel, mode, samplesPerCall);
    }

    setMixedAudioFrameParameters(sampleRate, samplesPerCall) {
        return this.rtcengine.setMixedAudioFrameParameters(sampleRate, samplesPerCall);
    }

    adjustRecordingSignalVolume(volume) {
        return this.rtcengine.adjustRecordingSignalVolume(volume);
    }

    adjustPlaybackSignalVolume(volume) {
        return this.rtcengine.adjustPlaybackSignalVolume(volume);
    }

    setHighQualityAudioParameters(fullband, stereo, fullBitrate) {
        return this.rtcengine.setHighQualityAudioParameters(fullband, stereo, fullBitrate);
    }

    enableWebSdkInteroprability(enable) {
        return this.rtcengine.enableWebSdkInteroprability(enable);
    }

    setVideoQualityParameters(preferFrameRateOverImageQuality) {
        return this.rtcengine.setVideoQualityParameters(preferFrameRateOverImageQuality);
    }

    startScreenCapture(windowId, captureFreq, rect, bitrate) {
        return this.rtcengine.startScreenCapture(windowId, captureFreq, rect, bitrate);
    }

    updateScreenCaptureRegion(rect) {
        return this.rtcengine.updateScreenCaptureRegion(rect);
    }

    getVideoDevices() {
        return this.rtcengine.getVideoDevices();
    }

    setVideoDevice(deviceid) {
        return this.rtcengine.setVideoDevice(deviceid);
    }

    getCurrentVideoDevice() {
        return this.rtcengine.getCurrentVideoDevice();
    }

    startVideoDeviceTest() {
        return this.rtcengine.startVideoDeviceTest();
    }

    stopVideoDeviceTest() {
        return this.rtcengine.stopVideoDeviceTest();
    }

    getAudioPlaybackDevices() {
        return this.rtcengine.getAudioPlaybackDevices();
    }

    setAudioPlaybackDevice(deviceid) {
        return this.rtcengine.setAudioPlaybackDevice(deviceid);
    }

    getCurrentAudioPlaybackDevice() {
        return this.rtcengine.getCurrentAudioPlaybackDevice();
    }

    setAudioPlaybackVolume(volume) {
        return this.rtcengine.setAudioPlaybackVolume(volume);
    }

    getAudioPlaybackVolume() {
        return this.rtcengine.getAudioPlaybackVolume();
    }

    getAudioRecordingDevices() {
        return this.rtcengine.getAudioRecordingDevices();
    }

    setAudioRecordingDevice(deviceid) {
        return this.rtcengine.setAudioRecordingDevice(deviceid);
    }

    getCurrentAudioRecordingDevice() {
        return this.rtcengine.getCurrentAudioRecordingDevice();
    }

    getAudioRecordingVolume() {
        return this.rtcengine.getAudioRecordingVolume();
    }

    setAudioRecordingVolume(volume) {
        return this.rtcengine.setAudioRecordingVolume(volume);
    }

    startAudioPlaybackDeviceTest(filepath) {
        return this.rtcengine.startAudioPlaybackDeviceTest(filepath);
    }

    stopAudioPlaybackDeviceTest() {
        return this.rtcengine.stopAudioPlaybackDeviceTest();
    }

    startAudioRecordingDeviceTest(indicateInterval) {
        return this.rtcengine.startAudioRecordingDeviceTest(indicateInterval);
    }

    stopAudioRecordingDeviceTest() {
        return this.rtcengine.stopAudioRecordingDeviceTest();
    }

    getAudioPlaybackDeviceMute() {
        return this.rtcengine.getAudioPlaybackDeviceMute();
    }

    setAudioPlaybackDeviceMute(mute) {
        return this.rtcengine.setAudioPlaybackDeviceMute(mute);
    }

    getAudioRecordingDeviceMute() {
        return this.rtcengine.getAudioRecordingDeviceMute();
    }

    setAudioRecordingDeviceMute(mute) {
        return this.rtcengine.setAudioRecordingDeviceMute(mute);
    }
};

module.exports = AgoraRtcEngine;
