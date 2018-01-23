//
//  Agora Media SDK
//
//  Created by Sting Feng in 2015-05.
//  Copyright (c) 2015 Agora IO. All rights reserved.
//
#pragma once
#include <mutex>
#include <list>
#include <util.h>
#include <base/base_type.h>

namespace agora {
namespace rtc {

    class DataStreamManager {
        enum CONSTANTS {
            DATA_STREAM_RELIABLE = (1 << 15),
            DATA_STREAM_ORDERED = (1 << 14),
            MAX_STREAM_COUNT = 5
        };
        struct DataHistoryItem {
            uint64_t ts;
            uint16_t length;
            DataHistoryItem(uint64_t t0, uint16_t l0)
                :ts(t0)
                , length(l0)
            {}
        };
        struct DataHistory {
            uint32_t totalLength;
            std::list<DataHistoryItem> history;
            DataHistory()
                :totalLength(0)
            {}
            void clearExpiredItems(uint64_t ts)
            {
                while (!history.empty())
                {
                    DataHistoryItem& item = history.front();
                    if (ts - item.ts < 1000)
                        break;
                    totalLength -= item.length;
                    history.pop_front();
                }
            }
            int onSend(size_t length, unsigned int packetRate, unsigned int bitrate)
            {
                auto ts = commons::tick_ms();
                clearExpiredItems(ts);
                if (packetRate > 0 && history.size() >= packetRate)
                    return -ERR_TOO_OFTEN;
                if (bitrate > 0 && totalLength + length > bitrate)
                    return -ERR_BITRATE_LIMIT;
                totalLength += length;
                history.emplace_back(ts, length);
                return 0;
            }
        };
        struct DataStream {
            uint32_t seq;
            DataHistory history;
            uint32_t lastSync;
            uint32_t syncSeq;
            DataStream()
                : seq(0)
                , lastSync(0)
                , syncSeq(0)
            {}

            uint32_t nextSyncSeq()
            {
                return ++syncSeq;
            }
        };
    public:
        DataStreamManager()
            :m_lastStreamId(0)
        {}
        int allocStreamId(bool reliable, bool ordered) {
            std::lock_guard<std::mutex> lock(m_mutex);
            if (m_streams.size() >= MAX_STREAM_COUNT)
                return 0;
            uint16_t streamId = ++m_lastStreamId;
            if (reliable)
                streamId |= DATA_STREAM_RELIABLE;
            if (ordered)
                streamId |= DATA_STREAM_ORDERED;
            m_streams.emplace(streamId, DataStream());
            return (int)streamId;
        }
        int onSendStreamMessage(stream_id_t streamId, size_t length, uint32_t& seq) {
            if (!isValidStream(streamId))
                return -ERR_INVALID_ARGUMENT;

            std::lock_guard<std::mutex> lock(m_mutex);
            
            auto it = m_streams.find(streamId);
            if (it != m_streams.end())
            {
                //bitrate control: 6kB/sec for all channels
                int r = m_totalHistory.onSend(length, 0, 6*1024);
                if (r) return r;

                auto& item = it->second;
                //packet rate control: 30 packets per channel
                r = item.history.onSend(length, 30, 0);
                if (!r)
                    seq = ++item.seq;
                return r;
            }
            return -ERR_INVALID_ARGUMENT;
        }
        std::unordered_map<stream_id_t, DataStream>& streams()
        {
            return m_streams;
        }
        void reset() {
            std::lock_guard<std::mutex> lock(m_mutex);
            m_lastStreamId = 0;
            m_streams.clear();
        }
        static stream_id_t toInternalStreamId(int streamId) {
            return (stream_id_t)streamId;
        }
        static bool isValidStream(stream_id_t streamId) {
            stream_id_t sid = getStreamId(streamId);
            return sid > 0 && sid <= MAX_STREAM_COUNT;
        }
        static bool isStreamReliable(stream_id_t streamId) {
            return streamId & DATA_STREAM_RELIABLE ? true : false;
        }
        static bool isStreamOrdered(stream_id_t streamId) {
            return streamId & DATA_STREAM_ORDERED ? true : false;
        }
        static stream_id_t getFirstStreamId() {
            return 1;
        }
        static stream_id_t getStreamId(stream_id_t streamId) {
            return streamId & (~(DATA_STREAM_RELIABLE | DATA_STREAM_ORDERED));
        }
        static bool isStreamEnd(const std::string& payload) {
            return payload.empty();
        }
        
    private:
        stream_id_t m_lastStreamId;
        std::mutex m_mutex;
        std::unordered_map<stream_id_t, DataStream> m_streams;
        DataHistory m_totalHistory;
    };

}}
