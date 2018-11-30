//
// Created by Rex Lien on 4/11/17.
//

#include "BaseMessage.h"

using namespace XCr;

XLN_IMPL_BASE_NAMESPACED_STREMABLE_MSG_CLASS(XCr, KeyValuePairMessage);

XCr::KeyValuePairMessage::~KeyValuePairMessage()
{

}

XCr::KeyValuePairMessage::KeyValuePairMessage()
{

}

XCr::KeyValuePairMessage::KeyValuePairMessage( const XCr::String& key, const XCr::String& value )
{
    InsertValue(key, value);
}

void XCr::KeyValuePairMessage::InsertValue( const XCr::String& key, const XCr::String& value )
{
    m_KeyValuePair[key] = value;
}

const XCr::String* XCr::KeyValuePairMessage::GetValue( const XCr::String& key ) const
{
    std::map<XCr::String, XCr::String>::const_iterator iter(m_KeyValuePair.find(key));
    if(iter ==m_KeyValuePair.end())
    {    return 0;
    }
    return &iter->second;
}

const std::map<XCr::String, XCr::String>& XCr::KeyValuePairMessage::GetKeyValues() const
{
    return m_KeyValuePair;
}

bool XCr::KeyValuePairMessage::FromStream( XCr::DataStream& stream )
{
    XCr::UInt32 mapSize = 0;
    stream.Read(&mapSize, sizeof(mapSize));
    XCr::UInt32 strSize = 0;
    for(XCr::UInt32 i = 0; i < mapSize; i++)
    {
        stream.Read(&strSize, sizeof(strSize));
        XCr::Char* buf = XLN_NEW XCr::Char[strSize + 1];
        stream.Read(buf, strSize);
        buf[strSize] = 0;
        XCr::String key(buf);
        XLN_DELETE [] buf;

        stream.Read(&strSize, sizeof(strSize));
        buf = XLN_NEW XCr::Char[strSize + 1];
        stream.Read(buf, strSize);
        buf[strSize] = 0;
        XCr::String value(buf);
        XLN_DELETE [] buf;
        m_KeyValuePair.insert(std::make_pair(key, value));

    }
    return true;
}

bool XCr::KeyValuePairMessage::ToStream( XCr::DataStream& stream )
{
    std::map<XCr::String, XCr::String>::iterator iter(m_KeyValuePair.begin());
    XCr::UInt32 mapSize = static_cast<XCr::UInt32>(m_KeyValuePair.size());
    stream.Write(&mapSize, sizeof(mapSize));
    while(iter != m_KeyValuePair.end())
    {
        XCr::UInt32 size = static_cast<XCr::UInt32>(iter->first.size());
        stream.Write(&size, sizeof(size));
        const XCr::Char* data = iter->first.data();
        stream.Write(const_cast<XCr::Char*>(data), size);

        size = static_cast<XCr::UInt32>(iter->second.size());
        stream.Write(&size, sizeof(size));
        data = iter->second.data();
        stream.Write(const_cast<XCr::Char*>(data), size);

        ++iter;
    }
    return true;
}