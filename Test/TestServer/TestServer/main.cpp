
#include "folly/futures/Future.h"
#include <XLN/Core/Foundation/Log.h>
#include <XLN/Core/Framework/System.h>
#include "TestServerSystemFactoryService.h"

//#include <boost/system/>

int main(int argc, char** argv) {
    
    XGf::System::Init(XLN_OBJ_NEW TestServerSystemFactoryService());
    
    XGf::System::Main(argc, argv);
   
    
}
