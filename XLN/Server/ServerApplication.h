//
// Created by Rex Lien on 18/08/2017.
//
#pragma once

#include <XLN/Core/Config/Config.h>
#include <folly/io/async/EventBase.h>
#include <XLN/Core/Framework/Application.h>

namespace XLN
{

class XLN_CLASS_DECL ServerApplication : public XGf::Application
{
public:

    virtual void Main(int argc, char** argv) override;
	
protected:
	virtual void OnInitService() override;
    virtual void StartServer(int argc, char** argv);
    virtual void StopServer();

private:

};

}