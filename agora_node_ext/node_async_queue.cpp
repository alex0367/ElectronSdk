/*
* Copyright (c) 2017 Agora.io
* All rights reserved.
* Proprietry and Confidential -- Agora.io
*/

/*
*  Created by Wang Yongli, 2017
*/

#include "node_async_queue.h"
#include <uv.h>

namespace agora {
    namespace rtc {
        node_async_call* node_async_call::s_instance = new node_async_call();

        node_async_call::node_async_call()
        {
            m_nodeQueue.reset(new async_queue<task_type>(uv_default_loop(), std::move(node_async_call::runTask)));
            //m_loop = (uv_loop_t*)malloc(sizeof(uv_loop_t));
            //uv_loop_init(m_loop);
            //uv_run(m_loop, UV_RUN_DEFAULT);
        }

        node_async_call::~node_async_call()
        {
            //m_nodeQueue->clear();
            //uv_stop(m_loop);
            //free(m_loop);
        }
    }
}