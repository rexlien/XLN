#include "GTestApplication.h"
#include <gtest/gtest.h>

using namespace XLN;

XLN_IMPL_SERVICE_CLASS(GTestSystemFactoryService, XGf::SystemFactoryService);
XLN_IMPLEMENT_CREATOR(GTestSystemFactoryService);

#ifdef  __ANDROID__

#include <android/log.h>
#include <pthread.h>
#include <unistd.h>

static int pfd[2];
static pthread_t loggingThread;

static void *loggingFunction(void*) {
    ssize_t readSize;
    char buf[128];

    while((readSize = read(pfd[0], buf, sizeof buf - 1)) > 0) {
        if(buf[readSize - 1] == '\n') {
            --readSize;
        }

        buf[readSize] = 0;  // add null-terminator

        __android_log_print(ANDROID_LOG_DEBUG, "", "%s", buf); // Set any log level you want
        fflush(stdout);
    }

    return 0;
}

static int runLoggingThread() { // run this function to redirect your output to android log
    setvbuf(stdout, 0, _IOLBF, 0); // make stdout line-buffered
    setvbuf(stderr, 0, _IONBF, 0); // make stderr unbuffered

    /* create the pipe and redirect stdout and stderr */
    pipe(pfd);
    dup2(pfd[1], 1);
    dup2(pfd[1], 2);

    /* spawn the logging thread */
    if(pthread_create(&loggingThread, 0, loggingFunction, 0) == -1) {
        return -1;
    }

    pthread_detach(loggingThread);

    return 0;
}
#endif

GTestSystemFactoryService::GTestSystemFactoryService() {

}

GTestSystemFactoryService::~GTestSystemFactoryService() {

}


XGf::Application *GTestSystemFactoryService::CreateApplication() {
   
    return XLN_OBJ_NEW GTestApplication();

}

void GTestApplication::Main(int argc, char** argv)
{
    XGf::Application::Main(argc, argv);
#ifdef __ANDROID__
    //workaround
    runLoggingThread();
#endif
    //int argc = 1;
    //char* argv = const_cast<char*>("XLNGTest");
    testing::InitGoogleTest(&argc, argv);

    RUN_ALL_TESTS();
}


