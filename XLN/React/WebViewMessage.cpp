//
// Created by Rex Lien on 4/12/17.
//

#include "WebViewMessage.h"

using namespace XGf;

XLN_IMPL_BASE_NAMESPACED_MSG_CLASS(XGf, WebViewMessage)

WebViewMessage::WebViewMessage() {

}


WebViewMessage::WebViewMessage(const XCr::String& webViewID, Command command)
    :m_WebViewID(webViewID)
{
    m_Command = command;
}

const XCr::String& WebViewMessage::GetWebViewID() const {
    return m_WebViewID;
};

WebViewMessage::Command WebViewMessage::GetCommand() const {
    return m_Command;
}

