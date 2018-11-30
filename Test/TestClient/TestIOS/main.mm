#import <UIKit/UIKit.h>

#include "folly/futures/Future.h"
#include <Core/Foundation/Log.h>
#include <Core/Framework/System.h>

using namespace folly;
int main(int argc, char *argv[]) {
    
    
    XGf::System::Init("test");
    
    XLN_LOGD("Hello, %d", 123);
    
    //google::InitGoogleLogging(argv[0]);
    Future<Unit> fu = makeFuture(Unit{}).unit();
    Unit unit = fu.value();    //LOG(INFO) << "Test log ";
    NSAutoreleasePool * pool = [[NSAutoreleasePool alloc] init];
    //int retVal = UIApplicationMain(argc, argv, nil, @"AppController");
    [pool release];
    
    
    XGf::System::Shutdown();
    return 0;
}
