//
//  Agora Media SDK
//
//  Created by Sting Feng in 2015-05.
//  Copyright (c) 2015 Agora IO. All rights reserved.
//
#pragma once
#include <functional>
#include <string>
#include <IAgoraService.h>

namespace agora {

namespace base {
    struct AgoraServiceContextEx : public AgoraServiceContext
    {
        std::string deviceId;
        std::string configDir;
        std::string dataDir;
        std::string pluginDir;
        AgoraServiceContextEx(){}
        AgoraServiceContextEx(const AgoraServiceContext&){}
    };

    class BaseContext;

//full feature definition of rtc engine interface
class IAgoraServiceEx : public IAgoraService
{
public:
    static const char* getSdkVersion(int* build);
    static const char* getSourceVersion();
    virtual int initializeEx(const AgoraServiceContextEx& context) = 0;
    virtual int panic(void *exception) = 0;
};

}}
