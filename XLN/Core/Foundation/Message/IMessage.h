#pragma once

#include <XLN/Core/Config/Config.h>
#include <XLN/Core/Utility/ClassInfo.h>
#include <XLN/Core/Foundation/DataStream.h>

namespace XCr
{



class XLN_CLASS_DECL IMessage : public MemObj
{   
    XLN_DECLARE_BASE_MSG_CLASS(IMessage);
    public:
        typedef IMessage* (*MessageCeator)();
        
        IMessage() {};
        virtual ~IMessage() {};
        virtual bool FromStream(XCr::DataStream& stream);
        virtual bool ToStream(XCr::DataStream& stream);
        
        template<class T>
        static XCr::SharedPtr<T> AcquireMessage()
        {
            return XLN_OBJ_NEW T();
        }
        
        template<class T, class Type1>
        static XCr::SharedPtr<XCr::IMessage> AcquireMessage(Type1 param1)
        {
            return XLN_OBJ_NEW T(param1);
        }
        
        template<class T, class Type1, class Type2>
        static XCr::SharedPtr<XCr::IMessage> AcquireMessage(Type1 param1, Type2 param2)
        {
            return XLN_OBJ_NEW T(param1, param2);
        }
        
        template<class T, class Type1, class Type2, class Type3>
        static XCr::SharedPtr<XCr::IMessage> AcquireMessage(Type1 param1, Type2 param2, Type3 param3)
        {
            return XLN_OBJ_NEW T(param1, param2, param3);
        }

        
        static IMessage* StreamToMsg(XCr::DataStream& stream);
        static void MsgToStream(IMessage& msg, XCr::DataStream& stream);
        
        static void RegisterMessageCreator(const XCr::Util::UniqueID& id, MessageCeator cb);
        
    protected:
        static std::map<XCr::Util::UniqueID, MessageCeator>& GetMsgCreator();
        
};

template<class T>
class MessageCretorRegister
{
public:
    MessageCretorRegister()
    {
        XCr::IMessage::RegisterMessageCreator(T::ClassID(), T::Create);
    }
};

}