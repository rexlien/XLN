#include <memory>
#include <chrono>

#include <gtest/gtest.h>
#include "gen-cpp2/TestService.h"
#include "gen-cpp2/FutureService.h"
#include <boost/lexical_cast.hpp>

#include <thrift/lib/cpp2/server/ThriftServer.h>
#include <thrift/lib/cpp2/async/StubSaslServer.h>
#include <thrift/lib/cpp2/async/HeaderClientChannel.h>
#include <thrift/lib/cpp2/test/util/TestServerFactory.h>
#include <thrift/lib/cpp/util/ScopedServerThread.h>
#include <thrift/lib/cpp/async/TAsyncSocket.h>
#include <thrift/lib/cpp2/async/RequestChannel.h>
#include <thrift/lib/cpp/transport/TTransportException.h>


#include <XLN/Server/Thrift/ThriftMuxService.h>

namespace apache {
    namespace thrift {

        template<typename Interface>
        struct TestThriftServerFactory : public TestServerFactory {
        public:
            std::shared_ptr<BaseThriftServer> create() override {
                auto server = std::make_shared<apache::thrift::ThriftServer>();
                server->setNumIOWorkerThreads(1);
                if (useSimpleThreadManager_) {
                    auto threadFactory =
                        std::make_shared<apache::thrift::concurrency::PosixThreadFactory>();
                    auto threadManager =
                        apache::thrift::concurrency::ThreadManager::newSimpleThreadManager(
                            1, 5, false, 50);
                    threadManager->threadFactory(threadFactory);
                    threadManager->start();
                    server->setThreadManager(threadManager);
                }
                else if (exe_) {
                    server->setThreadManager(exe_);
                }

                server->setPort(0);
                server->setSaslEnabled(true);
                if (useStubSaslServer_) {
                    server->setSaslServerFactory([](folly::EventBase* evb) {
                        return std::unique_ptr<apache::thrift::SaslServer>(
                            new apache::thrift::StubSaslServer(evb));
                    });
                }

                if (idleTimeoutMs_ != 0) {
                    server->setIdleTimeout(std::chrono::milliseconds(idleTimeoutMs_));
                }

                if (duplex_) {
                    server->setDuplex(true);
                }

                if (serverEventHandler_) {
                    server->setServerEventHandler(serverEventHandler_);
                }

                server->setMinCompressBytes(minCompressBytes_);
                if (transId_) {
                    std::vector<uint16_t> transforms{ transId_ };
                    server->setDefaultWriteTransforms(transforms);
                }
                server->setInterface(std::unique_ptr<Interface>(new Interface));
                return server;
            }

            
            TestThriftServerFactory& useSimpleThreadManager(bool use) override {
                useSimpleThreadManager_ = use;
                return *this;
            }

            TestThriftServerFactory& useThreadManager(
                std::shared_ptr<apache::thrift::concurrency::ThreadManager> exe)
                override {
                exe_ = exe;
                return *this;
            }

            TestThriftServerFactory& useStubSaslServer(bool use) {
                useStubSaslServer_ = use;
                return *this;
            }

            TestThriftServerFactory& idleTimeoutMs(uint32_t idle) {
                idleTimeoutMs_ = idle;
                return *this;
            }

            TestThriftServerFactory& duplex(bool duplex) {
                duplex_ = duplex;
                return *this;
            }

            TestThriftServerFactory& minCompressBytes(uint32_t minCompressBytes) {
                minCompressBytes_ = minCompressBytes;
                return *this;
            }

            TestThriftServerFactory& defaultWriteTransform(uint16_t transId) {
                transId_ = transId;
                return *this;
            }


        private:
            bool useSimpleThreadManager_{ true };
            std::shared_ptr<apache::thrift::concurrency::ThreadManager> exe_{ nullptr };
            bool useStubSaslServer_{ true };
            uint32_t idleTimeoutMs_{ 0 };
            bool duplex_{ false };
            uint32_t minCompressBytes_{ 0 };
            uint16_t transId_{ 0 };
        };

    }
} // apache::thrift

using namespace apache::thrift::test::cpp2;
using namespace apache::thrift;
using namespace apache::thrift::async;
using namespace apache::thrift::transport;


class TestThrift : public ::testing::Test
{

protected:
    
    TestThrift()
    {
        m_DefRpcOptions.setTimeout(std::chrono::milliseconds(3000));
        m_DefRpcOptions.setChunkTimeout(std::chrono::milliseconds(3000));
        m_DefRpcOptions.setQueueTimeout(std::chrono::milliseconds(3000));
    }

    virtual void SetUp() override {
        
    };

    virtual void TearDown() override {};


    apache::thrift::RpcOptions m_DefRpcOptions;
    
};


TEST_F(TestThrift, TestConnection)
{
    class TestService : public TestServiceSvIf
    {
    public:
        using ProcessorType = TestServiceAsyncProcessor;

        virtual void echoRequest(std::string& _return, std::unique_ptr<std::string> req) override
        {

            _return = "response";

        }

    };

    class ConnCallback : public TAsyncSocket::ConnectCallback {
    public:
        void connectSuccess() noexcept override 
        {
        }

        void connectError(
            const transport::TTransportException& ex) noexcept override {
            LOG(ERROR) << ex.what() << std::endl;
            EXPECT_EQ(ex.getType(), TTransportException::NOT_OPEN);
             
        }

       
    };

    TestThriftServerFactory<TestService> factory2;
    util::ScopedServerThread st(factory2.create());


    folly::EventBase base;


    //connection should fail since port is not listening.
    folly::SocketAddress badClientAddr("::1", 65535); 
    std::shared_ptr<TAsyncSocket> badSocket(
        TAsyncSocket::newSocket(&base));
    ConnCallback cb;
    badSocket->connect(&cb, badClientAddr, 3000);
    base.loop();
    
    

    //timeout should happens when rpc sent since connect to a http service that should ignore our request
    folly::SocketAddress clientAddr("8.8.8.8", 80);
    std::shared_ptr<TAsyncSocket> socket(
            TAsyncSocket::newSocket(&base, clientAddr, 3000));
  
    std::shared_ptr<apache::thrift::RequestChannel> headerClientChannel = HeaderClientChannel::newChannel(socket);
    TestServiceAsyncClient client(headerClientChannel);

    try
    {
        std::string ret;
        client.sync_echoRequest(m_DefRpcOptions, ret, "request");

    }
    catch (TTransportException& ex)
    {   
        LOG(ERROR) << ex.what() << std::endl;
        EXPECT_EQ(ex.getType(), TTransportException::TIMED_OUT);
    }
    
    
   

}


TEST_F(TestThrift, TestTimeout)
{
    class TestService : public TestServiceSvIf
    {
    public:
        using ProcessorType = TestServiceAsyncProcessor;

        virtual void echoRequest(std::string& _return, std::unique_ptr<std::string> req) override
        {

            EXPECT_EQ(*req, "request");
            _return = "response";
            sleep(5);
        }

        virtual void sendResponse(std::string& _return, int64_t size) override
        {

            _return = "response";
           
        }

    };

    TestThriftServerFactory<TestService> factory2;
    util::ScopedServerThread st(factory2.create());
    {
        folly::EventBase base;
        folly::SocketAddress clientAddr("::1", st.getAddress()->getPort());

        std::shared_ptr<TAsyncSocket> socket(
            TAsyncSocket::newSocket(&base, clientAddr));

        std::shared_ptr<apache::thrift::RequestChannel> headerClientChannel = HeaderClientChannel::newChannel(socket);

        TestServiceAsyncClient client(headerClientChannel);
       
        try
        {
           
            std::string ret;
            client.sync_echoRequest(m_DefRpcOptions, ret, "request");
            

        }
        catch (TTransportException& ex)
        {
            LOG(ERROR) << ex.what() << std::endl;
            EXPECT_EQ(ex.getType(), TTransportException::TIMED_OUT);
           
        }
        
        auto f1 = client.future_echoRequest(m_DefRpcOptions, "request").onError([](TTransportException & ex)
                    {   EXPECT_EQ(ex.getType(), TTransportException::TIMED_OUT); 
                return std::string("");
                }).waitVia(&base);

           
         auto f2 = client.future_echoRequest(m_DefRpcOptions, "request");
         auto f3 = client.future_sendResponse(1000);
         auto resVec = folly::collectAll(f2, f3).then([](const std::tuple<folly::Try<std::string>, folly::Try<std::string>>& tup) 
            {
                //f2 must have been timed out
                bool hasException = std::get<0>(tup).hasException();
                EXPECT_TRUE(hasException);
                try
                {
                    
                    std::string res1 = std::get<0>(tup).value();
                    std::string res2 = std::get<1>(tup).value();
                }
                catch (TTransportException& ex)
                {
                    
                    EXPECT_EQ(ex.getType(), TTransportException::TIMED_OUT);
                }
            }
         ).waitVia(&base);
       
    }

}

TEST_F(TestThrift, TestMultipleService)
{
    class TestService : public TestServiceSvIf
    {   
    public:
        using ProcessorType = TestServiceAsyncProcessor;
       
        virtual void echoRequest(std::string& _return, std::unique_ptr<std::string> req) override
        {

            EXPECT_EQ(*req, "request");
            _return = "response";
            
        }

    };

    class FutureTestService : public FutureServiceSvIf // , public FutureServiceSvIf{
    {
    public:
        using ProcessorType = FutureServiceAsyncProcessor;
        virtual void sendMuxRequest(int64_t request)override
        {

            EXPECT_EQ(request, 1000);
            

        }
    };
    TestThriftServerFactory<XLN::ThriftMuxServiceTemplate<TestService, FutureTestService>> factory2;
    util::ScopedServerThread st(factory2.create());
    {
        folly::EventBase base;
        //NOTE: have to explicit set localhost if in parallel desktop VM environ,ent
        folly::SocketAddress clientAddr("::1", st.getAddress()->getPort());
       
        std::shared_ptr<TAsyncSocket> socket(
            TAsyncSocket::newSocket(&base, clientAddr));

        std::shared_ptr<apache::thrift::RequestChannel> headerClientChannel = HeaderClientChannel::newChannel(socket);

        TestServiceAsyncClient client(headerClientChannel);
        FutureServiceAsyncClient futureClient(headerClientChannel);
        try
        {
         
            std::string ret;
            client.sync_echoRequest(ret, "request");
            EXPECT_EQ(ret, "response");

            
            auto f1 = client.future_echoRequest("request");
            auto f2 = futureClient.future_sendMuxRequest(1000);
            auto resVec = folly::collectAll(f1, f2).waitVia(&base);
            
            EXPECT_EQ(std::get<0>(resVec.get()).value(), "response");


        }
        catch (TException& ex)
        {
            LOG(ERROR) << ex.what() << std::endl;
        }

       
    }

}


TEST_F(TestThrift, TestSyncEcho)
{
    class EchoService : public TestServiceSvIf 
    {
        virtual void echoRequest(std::string& _return, std::unique_ptr<std::string> req) override
        {
            
            EXPECT_EQ(*req, "request");
            _return = "response";
            
        }
        
    };

    TestThriftServerFactory<EchoService> factory2;
    util::ScopedServerThread st(factory2.create());

    {
        folly::EventBase base;
        //NOTE: have to explicit set localhost if in parallel desktop VM environ,ent
        folly::SocketAddress clientAddr("::1", st.getAddress()->getPort());

        std::shared_ptr<TAsyncSocket> socket(
            TAsyncSocket::newSocket(&base, clientAddr));

        TestServiceAsyncClient client(HeaderClientChannel::newChannel(socket));
        try
        {
            std::string ret;
            client.sync_echoRequest(ret, "request");
            EXPECT_EQ(ret, "response");

            auto f1 = client.future_echoRequest("request");
            f1.waitVia(&base);
            EXPECT_EQ(f1.get(), "response");

        }
        catch (TException& ex)
        {
            LOG(ERROR) << ex.what() << std::endl;
        }
    }
}

TEST_F(TestThrift, TestSimpleServer)
{
    static std::atomic<bool> done(false);
    class OnewayTestInterface : public TestServiceSvIf {
        void noResponse(int64_t size) override {
            
            usleep(size);
            done = true;
        }
    };

   
    TestThriftServerFactory<OnewayTestInterface> factory2;
    util::ScopedServerThread st(factory2.create());
    
    {
        folly::EventBase base;
        //NOTE: have to explicit set localhost if in parallel desktop VM environ,ent
        folly::SocketAddress clientAddr("::1", st.getAddress()->getPort());

        std::shared_ptr<TAsyncSocket> socket(
            TAsyncSocket::newSocket(&base, clientAddr));
        
        TestServiceAsyncClient client(HeaderClientChannel::newChannel(socket));
        try
        {
            //auto future = client.future_noResponse(1);
            //future.waitVia(&base);
            client.sync_noResponse(10000);
        }
        catch (folly::AsyncSocketException& ex)
        {
            LOG(ERROR) << ex.what() << std::endl;
        }
        catch (TException& tx)
        {
            LOG(ERROR) << tx.what() << std::endl;
        }
       
    } 
    usleep(1000000);
    
    //not necessarily true;
    EXPECT_TRUE(done);

}

class MyFutureSServer : public FutureServiceSvIf
{
public:
    folly::Future<std::unique_ptr<std::string>> future_sendResponse(int64_t size) override
    {
        
        folly::Promise<std::unique_ptr<std::string>> p;
        auto f = p.getFuture();

        auto func = [p = std::move(p), size]() mutable
        {
            std::unique_ptr<std::string> ret(new std::string("test" + boost::lexical_cast<std::string>(size)));
            p.setValue(std::move(ret));
        };

        folly::RequestEventBase::get()->runInEventBaseThread(
            [func = std::move(func), size]() mutable
        {
            folly::RequestEventBase::get()->tryRunAfterDelay(std::move(func), size);
        }
        );
        return f;
    };

    folly::Future<int> future_throwing() override {
        folly::Promise<int> p;
        auto f = p.getFuture();

        Xception x;
        x.errorCode = 32;
        x.message = "test";

        p.setException(x);

        return f;
    }
    
};

TEST_F(TestThrift, TestFutureThrift)
{
    apache::thrift::TestThriftServerFactory<MyFutureSServer> factory;
    util::ScopedServerThread st(factory.create());
    
    folly::EventBase base;
    folly::SocketAddress clientAddr("::1", st.getAddress()->getPort());
    std::shared_ptr<TAsyncSocket> socket(
        TAsyncSocket::newSocket(&base, clientAddr));

    FutureServiceAsyncClient client(HeaderClientChannel::newChannel(socket));
   
    auto errorCodeF = client.future_throwing().onError([](const Xception& ex) {

        EXPECT_EQ(ex.get_errorCode(), 32);
        return -1;
    }).waitVia(&base);

    auto exceptionF = client.future_throwing().waitVia(&base);
    EXPECT_THROW(exceptionF.value(), Xception);

    auto reponseF = client.future_sendResponse(1000).waitVia(&base);
    EXPECT_EQ(reponseF.get(), "test1000");

    
}
