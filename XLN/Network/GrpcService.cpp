#include "GrpcService.h"

using namespace XLN;


XLN_IMPL_SERVICE_CLASS(GrpcService, XCr::Service);
XLN_IMPLEMENT_CREATOR(GrpcService);

GrpcService::GrpcService()
{
    
   
}

bool XLN::GrpcService::OnInit()
{
    //currently only 1 thread to handle cq
    m_ClientThreadPool.AddThread(std::bind(&GrpcService::RunClient, this, std::placeholders::_1));
    return true;
}

bool XLN::GrpcService::OnDestroy()
{
    m_ClientCq.Shutdown();
    //we know we only have 1 thread
    m_ClientThreadPool.JoinStoppedThread(1);
    return true;
}

std::shared_ptr<grpc::Channel> XLN::GrpcService::CreateChannel(const std::string & name, const std::string & address)
{
    auto newChannel = grpc::CreateChannel(address , grpc::InsecureChannelCredentials());
    m_ChannelPool.insert_or_assign(name, newChannel);
    
    return newChannel;
}

std::shared_ptr<grpc::Channel> XLN::GrpcService::GetChannel(const std::string & name)
{

    return m_ChannelPool[name];
}

void GrpcService::RunClient(XCr::ThreadPool::ThreadPtr thread)
{
    void* got_tag;
    bool ok = false;

    // Block until the next result is available in the completion queue "cq".
    while (m_ClientCq.Next(&got_tag, &ok)) {
        // The tag in this example is the memory location of the call object
        GrpcCallContextBase* call = static_cast<GrpcCallContextBase*>(got_tag);

        // Verify that the request was completed successfully. Note that "ok"
        // corresponds solely to the request for updates introduced by Finish().
        GPR_ASSERT(ok);
        call->promise();

        // Once we're complete, deallocate the call object.
        delete call;
    }
    thread->AfterRun();
    m_ClientThreadPool.AddStoppingThread(thread);
}