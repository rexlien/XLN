#include <XLN/Core/Config/Config.h>
#include <XLN/Core/Framework/System.h>

#include "GTestApplication.h"

int main(int argc, char** argv) {
    
    XGf::System::Init("GTestSystemFactoryService");    
    XGf::System::Main(argc, argv);

    
}

