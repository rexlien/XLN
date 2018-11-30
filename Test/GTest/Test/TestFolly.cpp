#include <gtest/gtest.h>
#include <folly/futures/Future.h>
#include <folly/executors/GlobalExecutor.h>
#include <folly/Baton.h>
#include <folly/executors/ManualExecutor.h>

TEST(TestFolly, TestFollyFuture) {

    auto promise = std::make_shared<folly::Promise<int>>();
    folly::Future<int> future = promise->getFuture();
    folly::Baton<> baton;
    int result;
    auto cb = [&baton, &result](int s) mutable
    {
        EXPECT_EQ(1, s);
        result = s;
        baton.post();
    };

    std::shared_ptr<folly::ManualExecutor> executor = std::make_shared<folly::ManualExecutor>();

    future.via(folly::getIOExecutor().get()).then([](int s) mutable
                                                  {
                                                      EXPECT_EQ(1, s);
                                                      return s;
                                                  } )
            .via(executor.get()).then(cb);

    folly::getIOExecutor()->add([promise]()
            {
                promise->setValue(1);
            }
    );

    while(!baton.try_wait())
    {
        executor->run();
    }

    EXPECT_EQ(1, result);
}
