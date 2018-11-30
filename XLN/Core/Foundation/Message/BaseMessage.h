//
// Created by Rex Lien on 4/11/17.
//

#pragma once

#include <XLN/Core/Config/Config.h>
#include "IMessage.h"

namespace XCr
{

class XLN_CLASS_DECL KeyValuePairMessage : public XCr::IMessage
{

    public:
        XLN_DECLARE_BASE_STREMABLE_MSG_CLASS(KeyValuePairMessage);
        KeyValuePairMessage();
        KeyValuePairMessage(const XCr::String& key, const XCr::String& value);
        virtual ~KeyValuePairMessage();
        virtual bool FromStream(XCr::DataStream& stream);
        virtual bool ToStream(XCr::DataStream& stream);
        void InsertValue(const XCr::String& key, const XCr::String& value);
        const XCr::String* GetValue(const XCr::String& key) const;
        const std::map<XCr::String, XCr::String>& GetKeyValues() const;

    private:
        std::map<XCr::String, XCr::String> m_KeyValuePair;
};


}