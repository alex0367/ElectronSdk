const AgoraRtcEngine = require("./AgoraSdk");
//const addon = require("./nodeaddon")
const appid = "0c0b4b61adf94de1befd7cdd78a50444";
rtcengine = null;
output = null;
function loadSdk() {
    rtcengine = new AgoraRtcEngine();
    rtcengine.initialize(appid, function () {
        document.write("Agora SDK load success.");
    }, function () {
        document.write("Agora SDK load failed.");
        });
}

function resetImageData() {
    header = undefined;
    yplane = undefined;
    uplane = undefined;
    vplane = undefined;
}

// Private methods
function getReso(w, h) {
    return { width: w, height: h };
}

var supportedVideoList = {
    'true': true,
    'unspecified': true,
    '120p': getReso(160, 120),
    '240p': getReso(320, 240),
    '360p': getReso(640, 360),
    '480p': getReso(640, 480),
    '720p': getReso(1280, 720),
    '1080p': getReso(1920, 1080),
    '4k': getReso(3840, 2160)
};


//loadSdk();