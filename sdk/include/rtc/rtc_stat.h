//
//  Agora Media SDK
//
//  Created by Sting Feng in 2015-05.
//  Copyright (c) 2015 Agora IO. All rights reserved.
//
#pragma once
#include <base/base_type.h>

namespace agora { namespace rtc {

	namespace che {
		struct LocalAudioStats {
			int recordingRate;
			int playoutRate;
			int frameRate;
			int packetRate;
			LocalAudioStats()
				:recordingRate(0)
				, playoutRate(0)
				, frameRate(0)
				, packetRate(0)
			{}
		};
		struct RemoteAudioStats {
			uint16_t fecSavedLossRate;
			uint16_t rxPackets;
			uint16_t rxFrames;
			uint16_t rxExpiredFrames;
			uint16_t decoderInFrames;
			uint16_t decoderOutFrames;
			int duration;
			RemoteAudioStats()
				:fecSavedLossRate(0)
				, rxPackets(0)
				, rxFrames(0)
				, rxExpiredFrames(0)
				, decoderInFrames(0)
				, decoderOutFrames(0)
				, duration(0)
			{}
			void update(const RemoteAudioStats& rhs) {
				fecSavedLossRate += rhs.fecSavedLossRate;
				rxPackets += rhs.rxPackets;
				rxFrames += rhs.rxFrames;
				rxExpiredFrames += rhs.rxExpiredFrames;
				decoderInFrames += rhs.decoderInFrames;
				decoderOutFrames += rhs.decoderOutFrames;
				duration += rhs.duration;
			}
			void normalize() {
				if (!duration) return;
				normalize_one(rxPackets);
				normalize_one(rxFrames);
				normalize_one(rxExpiredFrames);
				normalize_one(decoderInFrames);
				normalize_one(decoderOutFrames);
			}
			void normalize_one(uint16_t& value) {
				value = value * 1000 / duration;
			}
		};
		struct LocalVideoStreamStat
		{
			uint16_t txRate;
			uint16_t txFrames;
			uint16_t txPackets;
			LocalVideoStreamStat()
				:txRate(0)
				, txFrames(0)
				, txPackets(0)
			{}
			void update(const LocalVideoStreamStat& rhs) {
				txRate += rhs.txRate;
				txFrames += rhs.txFrames;
				txPackets += rhs.txPackets;
			}
			void normalize(int duration) {
				txRate = (uint16_t)((txRate)*8.0 / (duration));
				normalize_one(txFrames, duration);
				normalize_one(txPackets, duration);
			}
			void normalize_one(uint16_t& value, int duration) {
				value = value * 1000 / duration;
			}
		};

		struct LocalVideoStats {
			uint16_t captureWidth;
			uint16_t captureHeight;
			uint16_t captureFrames;
			uint16_t encoderInFrames;
			uint16_t encoderOutFrames;
			uint16_t encoderFailedFrames;
			uint16_t encoderSkipFrames;
			uint16_t encodeTimeMs;
			LocalVideoStreamStat lowStream;
			LocalVideoStreamStat highStream;
			int duration;
			LocalVideoStats()
				:captureWidth(0)
				, captureHeight(0)
				, captureFrames(0)
				, encoderInFrames(0)
				, encoderOutFrames(0)
				, encoderFailedFrames(0)
				, encoderSkipFrames(0)
				, encodeTimeMs(0)
				, duration(0)
			{}
			void update(const LocalVideoStats& rhs) {
				captureWidth = rhs.captureWidth;
				captureHeight = rhs.captureHeight;
				captureFrames += rhs.captureFrames;
				encoderInFrames += rhs.encoderInFrames;
				encoderOutFrames += rhs.encoderOutFrames;
				encoderFailedFrames += rhs.encoderFailedFrames;
				encoderSkipFrames += rhs.encoderSkipFrames;
				encodeTimeMs += rhs.encodeTimeMs;
				lowStream.update(rhs.lowStream);
				highStream.update(rhs.highStream);
				duration += rhs.duration;
			}
			void normalize() {
				if (!duration) return;
				normalize_one(encodeTimeMs, encoderOutFrames);
				normalize_one(captureFrames);
				normalize_one(encoderInFrames);
				normalize_one(encoderOutFrames);
				normalize_one(encoderFailedFrames);
				normalize_one(encoderSkipFrames);
				highStream.normalize(duration);
				lowStream.normalize(duration);
			}
			void normalize_one(uint16_t& value) {
				value = value * 1000 / duration;
			}
			void normalize_one(uint16_t& value, uint16_t frames) {
				if (frames)
					value = value * 50 / frames; //per 50 frames
			}
		};
		struct RemoteVideoStats {
			uint16_t rxPackets; //high+low
			uint16_t decoderFailedFrames;
			uint16_t decoderOutFrames;
			uint16_t rendererInFrames;
			uint16_t rendererOutFrames;
			uint16_t decodeTimeMs;
			bool isHardwareDecoding;
			int duration;
			RemoteVideoStats()
				:rxPackets(0)
				, decoderFailedFrames(0)
				, decoderOutFrames(0)
				, rendererInFrames(0)
				, rendererOutFrames(0)
				, decodeTimeMs(0)
				, duration(0)
                , isHardwareDecoding(false)
			{}
			void update(const RemoteVideoStats& rhs) {
				rxPackets += rhs.rxPackets;
				decoderOutFrames += rhs.decoderOutFrames;
				decoderFailedFrames += rhs.decoderFailedFrames;
				rendererInFrames += rhs.rendererInFrames;
				rendererOutFrames += rhs.rendererOutFrames;
				decodeTimeMs += rhs.decodeTimeMs;
				duration += rhs.duration;
                isHardwareDecoding = rhs.isHardwareDecoding;
			}
			void normalize() {
				if (!duration) return;
				normalize_one(decodeTimeMs, decoderOutFrames);
				normalize_one(rxPackets);
				normalize_one(decoderOutFrames);
				normalize_one(decoderFailedFrames);
				normalize_one(rendererInFrames);
				normalize_one(rendererOutFrames);
			}
			void normalize_one(uint16_t& value) {
				value = value * 1000 / duration;
			}
			void normalize_one(uint16_t& value, uint16_t frames) {
				if (frames)
					value = value *50 / frames; //per 50 frames
			}
		};
	}
	namespace sdk {
		struct AudioStats {
			uint16_t txPackets;
			uint16_t txDtxPackets;
			uint16_t rxPackets;
			uint16_t txDroppedPackets;
			uint16_t rxDroppedPackets;
			AudioStats()
				:txPackets(0)
				, txDtxPackets(0)
				, rxPackets(0)
				, txDroppedPackets(0)
				, rxDroppedPackets(0)
			{}
			void normalize(int duration) {
				if (!duration) return;
				txPackets = txPackets * 1000 / duration;
				txDtxPackets = txDtxPackets * 1000 / duration;
				rxPackets = rxPackets * 1000 / duration;
			}
		};
		struct VideoStats {
			int txPackets;
			int rxPackets;
			int txDroppedPackets;
			int rxDroppedPackets;
			VideoStats()
				:txPackets(0)
				, rxPackets(0)
				, txDroppedPackets(0)
				, rxDroppedPackets(0)
			{}
			void normalize(int duration) {
				if (!duration) return;
				txPackets = txPackets * 1000 / duration;
				rxPackets = rxPackets * 1000 / duration;
			}
		};
	}

struct TraceStats {
	struct {
		che::LocalAudioStats localAudioStats;
		che::RemoteAudioStats remoteAudioStats;
		che::LocalVideoStats localVideoStats;
		che::RemoteVideoStats remoteVideoStats;
	}che;
	struct {
		sdk::AudioStats audioStats;
		sdk::VideoStats videoStats;
	}sdk;
};

struct RtcEngineStats {
	uint64_t startTs;
	uint32_t duration;
	uint32_t txBytes;
	uint32_t rxBytes;
	uint16_t txKBitRate;
	uint16_t rxKBitRate;

	uint32_t txPackets;
	uint32_t rxPackets;
	uint16_t txPacketRate;
	uint16_t rxPacketRate;

	uint32_t txBroadcastBytes;
	uint32_t rxBroadcastBytes;
	uint32_t txReportBytes;
	uint16_t txBroadcastKBitRate;
	uint16_t rxBroadcastKBitRate;
	uint16_t txReportKBitRate;

	uint32_t txAudioBytes;
	uint32_t rxAudioBytes;
	uint16_t txAudioKBitRate;
	uint16_t rxAudioKBitRate;

    uint32_t txVideoBytes;
    uint32_t rxVideoBytes;
    uint16_t txVideoKBitRate;
    uint16_t rxVideoKBitRate;

	// p2p
	uint32_t p2pTxBytes;
	uint16_t p2pTxBitRate;
};


}}
