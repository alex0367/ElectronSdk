/*
 *  Copyright (c) 2004 The WebRTC project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

// Common definition for video, including fourcc and VideoFormat.

#ifndef MEDIA_BASE_VIDEOCOMMON_H_  // NOLINT
#define MEDIA_BASE_VIDEOCOMMON_H_

#include <stdint.h>

#include <string>

namespace agora {

enum VIDEO_FRAME_TYPE {
  kUnknown = 0,
  kI420 = 1,
  kBGRA = 2,
  kNV21 = 3,
  kRGBA = 4,
  kIMC2 = 5,
  kABGR = 6,
  kARGB = 7,
  kNV12 = 8,
  kRGB24 = 9,
  kAndroidTexture2D = 10,
  kAndroidTextureOES = 11,
  kIOSTexture = 13,  //to do
  kIOSCVPixelBuffer     = 20,
  kIOSCVPixelBufferI420 = 21,
  kIOSCVPixelBufferBGRA = 22,
  kIOSCVPixelBufferNV12 = 23,
  kI420NativeFrame = 100  //reserved
};

static bool isCVPixelBuffer(VIDEO_FRAME_TYPE type)
  { return type >= kIOSCVPixelBuffer && type <= kIOSCVPixelBufferNV12; }
static bool isAndroidTexture(VIDEO_FRAME_TYPE type)
  { return type >= kAndroidTexture2D && type <= kAndroidTextureOES; }

//////////////////////////////////////////////////////////////////////////////
// Definition of VideoFormat.
//////////////////////////////////////////////////////////////////////////////

struct VideoFormat {
  static const int kMaxfps = 1000;  // 1k fps.

  int width;  // Number of pixels.
  int height;  // Number of pixels.
  int fps;  // Nanoseconds.
  VIDEO_FRAME_TYPE type;  // Color space. FOURCC_ANY means that any color space is OK.

  VideoFormat() : width(0), height(0), fps(0), type(kUnknown) {}

  VideoFormat(int w, int h, int f, VIDEO_FRAME_TYPE t) : width(w),
                                                  height(h), 
                                                  fps(f), 
                                                  type(t) {}

  bool operator==(const VideoFormat& format) const {
    return width == format.width && height == format.height &&
        fps == format.fps && type == format.type;
  }

  bool operator!=(const VideoFormat& format) const {
    return !(*this == format);
  }

  // Check if both width and height are 0.
  bool IsSize0x0() const { return 0 == width && 0 == height; }

  // Get a string presentation in the form of "fourcc width x height x fps"
  //std::string ToString() const;
};

//////////////////////////////////////////////////////////////////////////////
// Definition of RenderFormat.
//////////////////////////////////////////////////////////////////////////////

struct RenderFormat {
  uint32_t uid;
  uint32_t stream_id;
  float left;
  float right;
  float top;
  float bottom;
  int z_order;
  int render_mode;

  RenderFormat() : uid(0), stream_id(0), left(0), right(0), top(0), bottom(0),
                   z_order(0), render_mode(0) {}
};

//////////////////////////////////////////////////////////////////////////////
// Definition of VideoFrame.
//////////////////////////////////////////////////////////////////////////////

struct VideoFrame {
  VIDEO_FRAME_TYPE type;
  int width;  //width of video frame
  int height;  //height of video frame
  int crop_left;
  int crop_right;
  int crop_top;
  int crop_bottom;
  void* buffer;  //data buffer
  int buffer_size;
  int rotation; // rotation of this frame (0, 90, 180, 270)
  int64_t ts;   //time stamp
  //Android
  void* context;
  int texture_id;
  float matrix[16];
  //end android
  void* native_handle;

  VideoFrame() : type(kUnknown), width(0), height(0),
    crop_left(0), crop_right(0), crop_top(0), crop_bottom(0),
    buffer(NULL), buffer_size(0), rotation(0), ts(0),
    context(NULL), texture_id(0), native_handle(NULL) {}
};

}  // namespace agora

#endif  // MEDIA_BASE_VIDEOCOMMON_H_  // NOLINT
