{
    'targets': [
      {
        'target_name': 'VideoSource',
		'type': 'executable',
		'defines': [
		'_USING_V110_SDK71_',
		'UNICODE'
		],
		'include_dirs': [
		'./sdk/include',
		'./common'
		],
		'library_dirs': [
			'./sdk/lib',
		],
		'sources': [
		'./common/ipc_shm.h',
		'./video_source/video_source.cpp',
		'./video_source/video_source.h',
		'./video_source/video_source_event_handler.cpp',
		'./video_source/video_source_event_handler.h',
		'./common/video_source_ipc.h',
		'./common/video_source_ipc.cpp',
		'./video_source/video_source_log.h',
		'./video_source/video_source_param_parser.cpp',
		'./video_source/video_source_param_parser.h',
		'./video_source/video_source_render.cpp',
		'./video_source/video_source_render.h',
		'./video_source/video_source_transporter.cpp',
		'./video_source/video_source_transporter.h'
		],
		'conditions': [
			[
			'OS=="win"',
			{
				"link_settings": {
                    "libraries": [
                        "-lagora_rtc_sdk.lib",
						"-lws2_32.lib"
                    ]
                },
				"link_settings!": [
                        "-liojs.lib",
                ],
				'defines!': [
				'_HAS_EXCEPTIONS=0'
				],
				'configurations': {
					'Release': {
						'msvs_settings': {
							'VCCLCompilerTool': {
								'ExceptionHandling': '0',
								'AdditionalOptions': [
									'/EHsc'
								]
							}
						}
					},
					'Debug': {
						'msvs_settings': {
							'VCCLCompilerTool': {
								'ExceptionHandling': '0',
								'AdditionalOptions': [
									'/EHsc'
								]
							}
						}
					}
				}
			}
			]
		]
      },
      {
        'target_name': 'agora_node_ext',
		'type': 'shared_library',
		'include_dirs': [
		'./sdk/include',
		'./common'
		],
		'library_dirs': [
			'./sdk/lib',
		],
		'sources': [
		'./agora_node_ext/agora_node_ext.cpp',
		'./agora_node_ext/agora_node_ext.h',
		'./agora_node_ext/agora_rtc_engine.cpp',
		'./agora_node_ext/agora_rtc_engine.h',
		'./agora_node_ext/agora_video_source.cpp',
		'./agora_node_ext/agora_video_source.h',
		'./common/ipc_shm.h',
		'./agora_node_ext/node_async_queue.cpp',
		'./agora_node_ext/node_async_queue.h',
		'./agora_node_ext/node_event_handler.cpp',
		'./agora_node_ext/node_event_handler.h',
		'./agora_node_ext/node_log.cpp',
		'./agora_node_ext/node_log.h',
		'./agora_node_ext/node_napi_api.cpp',
		'./agora_node_ext/node_napi_api.h',
		'./agora_node_ext/node_uid.cpp',
		'./agora_node_ext/node_uid.h',
		'./agora_node_ext/node_video_render.cpp',
		'./agora_node_ext/node_video_render.h',
		'./agora_node_ext/node_video_stream_channel.cpp',
		'./agora_node_ext/node_video_stream_channel.h',
		'./common/video_source_ipc.cpp',
		'./common/video_source_ipc.h'
		],
		'conditions': [
			[
			'OS=="win"',
			{
				"link_settings": {
                    "libraries": [
                        "-lagora_rtc_sdk.lib",
						"-lws2_32.lib",
						"-lRpcrt4.lib"
                    ]
                },
				'defines!': [
				'_HAS_EXCEPTIONS=0'
				],
				'configurations': {
					'Release': {
						'msvs_settings': {
							'VCCLCompilerTool': {
								'ExceptionHandling': '0',
								'AdditionalOptions': [
									'/EHsc'
								]
							}
						}
					},
					'Debug': {
						'msvs_settings': {
							'VCCLCompilerTool': {
								'ExceptionHandling': '0',
								'AdditionalOptions': [
									'/EHsc'
								]
							}
						}
					}
				}
			}
			]
		]
      },
    ]
  }