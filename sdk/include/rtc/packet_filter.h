//
//  Agora Media SDK
//
//  Created by Sting Feng in 2015-05.
//  Copyright (c) 2015 Agora IO. All rights reserved.
//
#pragma once
#include <util.h>
#include <base/base_type.h>

namespace agora {
	namespace commons {
		class event_loop;
	}
namespace rtc {

enum MEDIA_STREAM_TYPE {
    MEDIA_STREAM_TYPE_AUDIO = (1 << 0),
    MEDIA_STREAM_TYPE_VIDEO_LOW = (1 << 1),
    MEDIA_STREAM_TYPE_VIDEO_HIGH = (1 << 2),
    MEDIA_STREAM_TYPE_VIDEO = (MEDIA_STREAM_TYPE_VIDEO_LOW | MEDIA_STREAM_TYPE_VIDEO_HIGH),
};

struct rtc_packet_t {
	uid_t uid;
	uint32_t seq;
	uint16_t payload_length; // should be the same as payload.length()
	uint64_t sent_ts;
	uint64_t recv_ts;
	int link_id;
	std::string payload;
	rtc_packet_t()
		:uid(0)
		, seq(0)
		, payload_length(0)
		, sent_ts(0)
		, recv_ts(0)
		, link_id(-1)
	{}
    virtual ~rtc_packet_t() {}
	// rtc_packet_t(rtc_packet_t&& rhs) = default;
  //   :uid(rhs.uid)
	// 	, seq(rhs.seq)
	// 	, payload_length(rhs.payload_length)
	// 	, sent_ts(rhs.sent_ts)
	// 	, recv_ts(rhs.recv_ts)
	// 	, link_id(rhs.link_id)
	// 	, payload(std::move(rhs.payload))
	// {}
	// rtc_packet_t& operator=(rtc_packet_t&& rhs) = default;
  // {
  //   if (this != &rhs) {
	// 		uid = rhs.uid;
	// 		seq = rhs.seq;
	// 		payload_length = rhs.payload_length;
	// 		sent_ts = rhs.sent_ts;
	// 		recv_ts = rhs.recv_ts;
	// 		link_id = rhs.link_id;
	// 		payload = std::move(rhs.payload);
	// 	}
	// 	return *this;
	// }
//private:
//	rtc_packet_t(const rtc_packet_t& rhs) = delete;
//	rtc_packet_t& operator=(const rtc_packet_t& rhs) = delete;
};

struct broadcast_packet_t : public rtc_packet_t
{
    bool quit;
    bool rtcp;
    broadcast_packet_t()
        :quit(false)
        , rtcp(false)
    {}
};

struct audio_packet_t : public rtc_packet_t
{
	uint32_t ts;
	int8_t vad;
	uint8_t codec;
	int last_error;//error code set by last filter
  uint32_t reqMs; // for calculating RTT only
	audio_packet_t()
		:ts(0)
		, vad(0)
		, codec(0)
		, last_error(0)
    , reqMs(0)
	{}

// NOTE(liuyong): supporting move semantics in this plain object has no meaning.
  // audio_packet_t(audio_packet_t&& rhs) = default;

  // audio_packet_t& operator=(audio_packet_t&& rhs) = default;

// private:
//	audio_packet_t(const audio_packet_t& rhs) = delete;
//	audio_packet_t& operator=(const audio_packet_t& rhs) = delete;
};

struct video_packet_t : public rtc_packet_t
{
	enum VIDEO_STREAM_TYPE {
		VIDEO_STREAM_UNKNOWN = -1,
		VIDEO_STREAM_HIGH = 0,
		VIDEO_STREAM_LOW = 1,
		VIDEO_STREAM_MEDIUM = 2,
		VIDEO_STREAM_LIVE = 3,
		VIDEO_STREAM_MIN = VIDEO_STREAM_HIGH,
		VIDEO_STREAM_MAX = VIDEO_STREAM_LIVE,
	};

	enum VIDEO_FLAG_TYPE {
        //below is for video2 only, not used in video3
        VIDEO_FLAG_KEY_FRAME = 0x80,
        VIDEO_FLAG_FEC = 0x40,
        VIDEO_FLAG_LIVE = 0x20,
        VIDEO_FLAG_STD_CODEC = 0x8,//also for video3 to differentiate std stream and private stream
        VIDEO_FLAG_B_FRAME = 0x10,
        //below is for video3
        VIDEO_FLAG_HARDWARE_ENCODE = 0x4,
	};

  enum VIDEO_INTERNAL_FLAG_TYPE {
        VIDEO_FLAG_REXFERRED = 0x1,
        VIDEO_FLAG_VIDEO3 = 0x2,
        VIDEO_FLAG_TIMESTAMP_SET = 0x4,
        VIDEO_FLAG_FROM_VOS = 0x8,
  };

	enum VIDEO_FRAME_TYPE {
		KEY_FRAME = 0,
		DELTA_FRAME = 1,
		B_FRAME = 2,
	};
	enum VIDEO_CODEC_TYPE {
		VIDEO_CODEC_VP8 = 1, //std VP8
		VIDEO_CODEC_H264 = 2, //std H264
		VIDEO_CODEC_EVP = 3, //VP8 with BCM
		VIDEO_CODEC_E264 = 4, //H264 with BCM
	};

	enum VIDEO_EXTRA_FLAG_TYPE {
    // marks if the |req_ms| field of PVideoRexferRes_v4 is set
		VIDEO_EXTRA_FLAG_TIMESTAMP_SET = 0x1,
	};

	uint32_t frameSeq;
	uint8_t frameType;
	uint8_t streamType;
	uint8_t packets;
	uint8_t subseq;
  uint8_t fecPkgNum;
	uint8_t codec;
	uint8_t flags;
	uint8_t protocolVersion;
  uint8_t internalFlags; // If this packet is rexferred.
  uint32_t reqMs; // for calculating RTT only

	video_packet_t()
		:frameSeq(0)
		, frameType(0)
		, streamType(0)
	  , packets(0)
		, subseq(0)
		, codec(0)
		, flags(0)
		, protocolVersion(0)
    , internalFlags(0)
    , reqMs(0)
	{}

  // NOTE(liuyong): supporting move semantics in this plain object has no meaning.
	// video_packet_t(video_packet_t&& rhs) = default;
  //   :rtc_packet_t(std::move(rhs))
	// 	, frameSeq(rhs.frameSeq)
	// 	, frameType(rhs.frameType)
	// 	, streamType(rhs.streamType)
	// 	, packets(rhs.packets)
	// 	, subseq(rhs.subseq)
	// 	, codec(rhs.codec)
	// 	, flags(rhs.flags)
	// 	, protocolVersion(rhs.protocolVersion)
	// {}

	// video_packet_t& operator=(video_packet_t&& rhs) = default;
  // {
	// 	if (this != &rhs) {
	// 		rtc_packet_t::operator = (std::move(rhs));
	// 		frameSeq = rhs.frameSeq;
	// 		frameType = rhs.frameType;
	// 		streamType = rhs.streamType;
	// 		packets = rhs.packets;
	// 		subseq = rhs.subseq;
	// 		codec = rhs.codec;
	// 		flags = rhs.flags;
	// 		protocolVersion = rhs.protocolVersion;
	// 	}
	// 	return *this;
	// }

	union video3_flags {
		struct {
      uint8_t stream_type : 4;
		  uint8_t frame_type : 4;
		};
		uint8_t video_type;
	};

	void fromVideType(uint8_t f) {
		video3_flags t;
		t.video_type = f;
		streamType = t.stream_type;
		frameType = t.frame_type;
	}

	uint8_t toVideoType() const {
		video3_flags t;
		t.stream_type = streamType;
		t.frame_type = frameType;
		return t.video_type;
	}
//private:
//	video_packet_t(const video_packet_t& rhs) = delete;
//	video_packet_t& operator=(const video_packet_t& rhs) = delete;
};

struct data_stream_packet_t : public rtc_packet_t
{
    stream_id_t stream_id;
    data_stream_packet_t()
        :stream_id(0)
    {}
};

struct data_stream_rexferred_packet_t : public rtc_packet_t
{
    stream_id_t stream_id;
    uint32_t request_ts;
    data_stream_rexferred_packet_t()
        : stream_id(0)
        , request_ts(0)
    {}
};

struct data_stream_sync_packet_t : public rtc_packet_t
{
    stream_id_t stream_id;
    uint32_t last_seq;
    data_stream_sync_packet_t()
        : stream_id(0)
        , last_seq(0)
    {}    
};

template<class T>
inline void to_rtc_packet(T&& from, rtc_packet_t& to, int linkId, uint64_t recv_ts)
{
	to.uid = from.uid;
	to.seq = from.seq;
	to.sent_ts = from.sent_ts;
	to.recv_ts = recv_ts ? recv_ts : commons::tick_ms();
	to.payload_length = (decltype(to.payload_length))from.payload.length();
	to.payload = std::move(from.payload);
	to.link_id = linkId;
}

template<class T>
inline void to_audio_packet(T&& from, audio_packet_t& to, int linkId, uint64_t recv_ts, int8_t vad)
{
	to_rtc_packet(std::move(from), to, linkId, recv_ts);
	to.ts = from.ts;
	to.codec = from.codec;
	to.vad = vad;
	to.last_error = 0;
}

template<class T>
inline void to_video_packet(T&& from, video_packet_t& to, int linkId, uint64_t recv_ts, uint8_t flags, uint8_t streamType, uint32_t frames)
{
	to_rtc_packet(std::move(from), to, linkId, recv_ts);
	to.frameSeq = frames;
	to.streamType = streamType;
	if (flags & video_packet_t::VIDEO_FLAG_KEY_FRAME)
		to.frameType = video_packet_t::KEY_FRAME;
	else if (flags & video_packet_t::VIDEO_FLAG_B_FRAME)
		to.frameType = video_packet_t::B_FRAME;
	else
		to.frameType = 0;
	to.packets = 0;
	to.subseq = 0;
	//old sdk only support VP8 and EVP
	if (flags & video_packet_t::VIDEO_FLAG_STD_CODEC)
		to.codec = video_packet_t::VIDEO_CODEC_VP8;
	else
		to.codec = video_packet_t::VIDEO_CODEC_EVP;
	to.flags = 0;
	to.protocolVersion = 0;//0 is for old version
}

template<class T>
inline void to_video_packet3(T&& from, video_packet_t& to, int linkId, uint64_t recv_ts)
{
	to_rtc_packet(std::move(from), to, linkId, recv_ts);
	to.fromVideType(from.video_type);
	to.frameSeq = from.frame_seq;
	to.packets = from.packets;
	to.subseq = from.subseq;
	to.codec = from.codec;
	to.protocolVersion = from.protocol_version;
  to.flags = from.flags;
}

template<class T>
inline void to_data_stream_packet(T&& from, data_stream_packet_t& to, int linkId, uint64_t recv_ts)
{
    to_rtc_packet(std::move(from), to, linkId, recv_ts);
    to.stream_id = from.stream_id;
}

template<class T>
inline void to_data_stream_rexferred_packet_t(T&& from, data_stream_rexferred_packet_t& to, int linkId, uint64_t recv_ts)
{
    to_rtc_packet(std::move(from), to, linkId, recv_ts);
    to.stream_id = from.stream_id;
    to.request_ts = from.request_ts;
}

template<class T>
inline void to_video_packet4(T&& from, video_packet_t& to, int linkId, uint64_t recv_ts)
{
    to_rtc_packet(std::forward<T>(from), to, linkId, recv_ts);
    to.fromVideType(from.header.video_type);
    to.frameSeq = from.header.frame_seq;
    to.packets = from.header.packet_cnt;
    to.subseq = from.header.subseq;
    to.codec = from.header.codec;
    to.protocolVersion = from.header.protocol_version;
    to.flags = from.header.flags;
}

enum FilterReturnValue {
	FILTER_CONTINUE = 0,
	FILTER_ABORT = 1,
    FILTER_MORE = 2,
};

class AudioPacketFilter
{
public:
	virtual ~AudioPacketFilter() {}
	// return true to continue processing, false to stop processing
	virtual int onFilterAudioPacket(audio_packet_t& p) = 0;
};

class VideoPacketFilter
{
public:
	virtual ~VideoPacketFilter() {}
	// return true to continue processing, false to stop processing
	virtual int onFilterVideoPacket(video_packet_t& p) = 0;
};

class DataStreamPacketFilter
{
public:
    virtual ~DataStreamPacketFilter() {}
	// return true to continue processing, false to stop processing
    virtual int onFilterDataStreamPacket(data_stream_packet_t& p) = 0;
    virtual int onFilterDataStreamRexferredPacket(data_stream_rexferred_packet_t& p) = 0;
};

class BroadcastPacketFilter
{
public:
	virtual ~BroadcastPacketFilter() {}
	// return true to continue processing, false to stop processing
    virtual int onFilterBroadcastPacket(broadcast_packet_t& p) = 0;
};

class CallReporter;
class IPacketObserverFilter : public AudioPacketFilter, public VideoPacketFilter
{
	struct FilterStats {
		unsigned int count;
		unsigned int accumulatedTime;
		unsigned int drops;
		FilterStats()
			:count(0)
			, accumulatedTime(0)
			, drops(0)
		{}
		unsigned int normalizedTime() {
			unsigned int v = count ? accumulatedTime * 50 / count : 0;
			count = 0;
			accumulatedTime = 0;
			return v;
		}
		unsigned int getDroppedPackets() {
			unsigned int v = drops;
			drops = 0;
			return v;
		}
	};
	FilterStats m_audioSend;
	FilterStats m_audioRecv;
	FilterStats m_videoSend;
	FilterStats m_videoRecv;
	static unsigned int elapsed(uint64_t ts) {
		return (unsigned int)(commons::tick_ms() - ts);
	}
public:
	void onAudioStats(int filterResult, audio_packet_t& p, uint64_t ts) {
		if (filterResult == FILTER_CONTINUE) {
			if (!p.uid) {
				m_audioSend.count++;
				m_audioSend.accumulatedTime += elapsed(ts);
			}
			else {
				m_audioRecv.count++;
				m_audioRecv.accumulatedTime += elapsed(ts);
			}
		}
		else {
			if (!p.uid) {
				m_audioSend.drops++;
			}
			else {
				m_audioRecv.drops++;
			}
		}
	}
	void onVideoStats(int filterResult, video_packet_t& p, uint64_t ts) {
		if (filterResult == FILTER_CONTINUE) {
			if (!p.uid) {
				m_videoSend.count++;
				m_videoSend.accumulatedTime += elapsed(ts);
			}
			else {
				m_videoRecv.count++;
				m_videoRecv.accumulatedTime += elapsed(ts);
			}
		}
		else {
			if (!p.uid) {
				m_videoSend.drops++;
			}
			else {
				m_videoRecv.drops++;
			}
		}
	}
	void reportStats(CallReporter* p);
};

}}
