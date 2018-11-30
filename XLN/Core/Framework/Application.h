//
// Created by Rex Lien on 3/16/17.
//

#include <XLN/Core/Config/Config.h>

#pragma once

namespace XGf
{


class Application : public XCr::MemObj
{


private:
    static XGf::Application* s_Applicaiton;

public:
    Application();
    virtual ~Application();
    virtual void Init();
    virtual void InitService();
    virtual void PostInitServices();

    virtual void Main(int argc, char** argv);
    static Application* GetAppication();

protected:
    virtual void OnInit();
    virtual void OnInitService();
    virtual void OnPostInitServices();


};

}