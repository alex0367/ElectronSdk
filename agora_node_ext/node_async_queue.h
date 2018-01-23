/*
* Copyright (c) 2017 Agora.io
* All rights reserved.
* Proprietary and Confidential -- Agora.io
*/

/*
*  Created by Wang Yongli, 2017
*/
#ifndef AGORA_NODE_ASYNC_QUEUE_H
#define AGORA_NODE_ASYNC_QUEUE_H

#include <cstdint>
#include <atomic>
#include <mutex>
#include <condition_variable>
#include <future>
#include <chrono>
#include <queue>
#include <functional>
#include <string.h>
#include <util.h>
#include "uv.h"
namespace agora{
    namespace rtc {

        template <class T>
        class unlock_guard {
        public:
            unlock_guard(T& mutex) : mutex_(mutex) {
                mutex_.unlock();
            }

            ~unlock_guard() {
                mutex_.lock();
            }
        private:
            unlock_guard(const unlock_guard&) = delete;
            unlock_guard& operator=(const unlock_guard&) = delete;
        private:
            T& mutex_;
        };
        template<typename T1, typename T2 = int>
        class async_queue_base {
        public:
            typedef std::function<void(T1&)> callback_type;
            virtual ~async_queue_base() {}
            virtual int async_call(T1&& e, uint64_t ts = 0) = 0;
            virtual int await_async_call(T1&& req, T2* res, int timeout) { return -1; }
            virtual void set_priority(int prio) = 0;
            virtual void set_capacity(size_t capacity) = 0;
            virtual void clear() = 0;
            virtual uint64_t last_pop_ts() const { return 0; }
            virtual void set_promise_result(T2&& result) {}
        };

        template <typename Elem, typename T2 = int, typename Lck = std::mutex>
        class async_queue
        {
            async_queue(const async_queue&) = delete;
            async_queue& operator=(const async_queue&) = delete;
        public:
            using callback_type = typename async_queue_base<Elem, T2>::callback_type;
            async_queue(uv_loop_t *loop, callback_type&& cb)
                : h_((uv_async_t*)malloc(sizeof(uv_async_t)))
                , closed_(false)
                , cb_(std::move(cb))
                , capacity_(0)
            {
                int r = ::uv_async_init(loop, h_, async_callback);
                h_->data = this;
            }

            ~async_queue() {
                uv_close((uv_handle_t*)h_, [](uv_handle_t *handle) {
                    free(handle);
                });
            }
            int async_call(Elem&& e, uint64_t ts = 0){
                if (closed_) {
                    return -1;
                }

        {
            std::lock_guard<Lck> guard(lock_);
            if (capacity_&& q_.size() > capacity_)
            {
                q_.pop();
            }
            q_.push(std::move(e));
        }

        return !uv_async_send(h_) ? 0 : -1;
            }
            size_t size() const {
                std::lock_guard<Lck> guard(lock_);
                return q_.size();
            }
            bool empty() const {
                return size() == 0;
            }
            void close() {
                if (!empty()) {
                    log(LOG_WARN, " You should close this queue after taking all the elements!");
                }
                closed_ = true;
            }
            bool closed() const {
                return closed_;
            }
            void set_priority(int prio){}
            void set_capacity(size_t capacity){
                capacity_ = capacity;
            }
            void clear(){
                std::lock_guard<Lck> guard(lock_);
                std::queue<Elem> empty;
                std::swap(q_, empty);
            }
            uint64_t last_pop_ts() const{
                return tick_ms();
            }
        private:
            static void async_callback(uv_async_t* handle) {
                reinterpret_cast<async_queue*>(handle->data)->on_event();
            }
            void on_event() {
                std::lock_guard<Lck> guard(lock_);
                while (!q_.empty()) {
                    Elem e(std::move(q_.front()));
                    q_.pop();
                    unlock_guard<Lck> unguard(lock_);
                    cb_(e);
                }
            }
        private:
            uv_async_t* h_;
            std::atomic<bool> closed_;
            mutable Lck lock_;
            std::queue<Elem> q_;
            callback_type cb_;
            size_t capacity_;
        };
        class node_async_call
        {
        public:
            using task_type = std::function<void(void)>;
            using node_queue_type = async_queue<task_type>;
            static void asyncCall(task_type cb) {
                node_async_call::getInstance()->m_nodeQueue->async_call(std::move(cb));
            }

        private:
            node_async_call();
            ~node_async_call();
            static node_async_call* getInstance() { return s_instance; }
            static void runTask(task_type& task) {
                task();
            }
            std::unique_ptr<node_queue_type> m_nodeQueue;
            static node_async_call* s_instance;
            uv_loop_t *m_loop;
        };
    }
}

#endif