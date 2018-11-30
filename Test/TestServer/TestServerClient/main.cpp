
#include "folly/futures/Future.h"
#include <XLN/Core/Foundation/Log.h>
#include <XLN/Core/Framework/System.h>
#include "TestServerClientSystemFactoryService.h"

//#include <boost/system/>

int main() {
    
    XGf::System::Init(XLN_OBJ_NEW TestServerClientSystemFactoryService());
    
    XGf::System::Main();
    //std::cout << "After Main" << std::endl;
    
}
