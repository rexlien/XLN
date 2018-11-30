#include <XLN/Core/Foundation/Message/IMessage.h>

using namespace XCr;

XLN_IMPL_BASE_NAMESPACED_MSG_CLASS(XCr, IMessage);

bool XCr::IMessage::FromStream( XCr::DataStream& stream )
{
    return true;
}

bool XCr::IMessage::ToStream( XCr::DataStream& stream )
{
    return true;
}

void XCr::IMessage::RegisterMessageCreator( const XCr::Util::UniqueID& id, MessageCeator cb )
{   
    GetMsgCreator().insert(std::make_pair(id, cb));
}

IMessage* XCr::IMessage::StreamToMsg( XCr::DataStream& stream )
{   
    XCr::Util::UniqueID id;
    stream.Read(id.GetData(), id.GetByteLength());
    std::map<XCr::Util::UniqueID, XCr::IMessage::MessageCeator>& creators = GetMsgCreator();
    
    std::map<XCr::Util::UniqueID, XCr::IMessage::MessageCeator>::iterator iter(creators.find(id));
    if(iter != creators.end())
    {
        IMessage* newMessage = iter->second();
        newMessage->FromStream(stream);
        return newMessage;
    }
    
    return 0;
    
}

void XCr::IMessage::MsgToStream( IMessage& msg, XCr::DataStream& stream )
{
    stream.Write((void*)msg.ObjClassID().GetData(), msg.ObjClassID().GetByteLength());
    msg.ToStream(stream);
}

std::map<XCr::Util::UniqueID, IMessage::MessageCeator>& XCr::IMessage::GetMsgCreator()
{   static std::map<XCr::Util::UniqueID, XCr::IMessage::MessageCeator> inst;
    return inst;
}

