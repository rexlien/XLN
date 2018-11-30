#include <XLN/Core/Config/Config.h>
#include <XLN/Network/gen-cpp2/ActorService.h>

namespace XLN
{

class ActorService : public XLN::Game::Thrift::cpp2::ActorServiceSvIf 
{

    virtual void sendMessage(std::unique_ptr< ::XLN::Game::Thrift::cpp2::Message> msg) override
    {

    }

    virtual void testString(std::string& ret, std::unique_ptr<std::string> str) override
    {

        ret = *str;
    }

        
};


}