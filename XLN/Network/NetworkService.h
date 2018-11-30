#pragma once

#include <XLN/Core/Config/Config.h>
#include "gen-cpp2/NetworkService.h"

namespace XLN
{
    class NetworkService : public XLN::Game::Thrift::cpp2::NetworkServiceSvIf
    {

    public:

        virtual int64_t heartbeat(int64_t localtime) override;


    };


}