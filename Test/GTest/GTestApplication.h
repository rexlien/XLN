#include <XLN/Core/Config/Config.h>
#include <XLN/Core/Framework/SystemFactoryService.h>
#include <XLN/Core/Framework/Application.h>


namespace XLN
{

    class XLN_CLASS_DECL GTestSystemFactoryService : public XGf::SystemFactoryService
    {
        XLN_DECLARE_SERVICE_CLASS(GTestSystemFactoryService)
        XLN_DECLARE_CREATOR(GTestSystemFactoryService);

    public:

        GTestSystemFactoryService();
        virtual ~GTestSystemFactoryService();
        virtual XGf::Application *CreateApplication() override;


    };

    class XLN_CLASS_DECL GTestApplication : public XGf::Application
    {
        public:
            virtual void Main(int argc, char** argv) override;
           
        protected:
            //virtual void OnInitService() override;
          
        private:

    };
}