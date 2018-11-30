#include "ServerExecutorService.h"

#include <XLN/Core/Network/Acceptor/Acceptor.h>
#include <folly/SocketAddress.h>
#include <folly/io/async/EventBaseManager.h>
#include <boost/thread/latch.hpp>

using namespace XLN;

static const int IO_WORKER_COUNT = 4;

XLN::ServerExecutorService::ServerExecutorService(ServerConfig config)
	:m_Config(config)
{
    m_IOTasksExecutor->SetMaxThread(IO_WORKER_COUNT);
}

XLN::ServerExecutorService::ServerExecutorService()
    :m_Acceptor(XLN_OBJ_NEW XCr::Acceptor())
{
    m_IOTasksExecutor->SetMaxThread(IO_WORKER_COUNT);
}

XLN::ServerExecutorService::~ServerExecutorService()
{
	

}

void XLN::ServerExecutorService::StartAccpet()
{

    m_AcceptorExectuor.SetMaxThread(1);
	folly::SocketAddress address;
	address.setFromLocalPort(m_Config.m_Port);

	auto newSocket = folly::AsyncServerSocket::newSocket(0);
	m_AcceptorSocket.wlock()->push_back(newSocket);

	auto startFunc = [&](std::shared_ptr<folly::Baton<>> barrier)
	{
		newSocket->setReusePortEnabled(false);
		//newSocket->setKeepAliveEnabled(true);
		newSocket->attachEventBase(folly::EventBaseManager::get()->getEventBase());
		newSocket->bind(address);
		newSocket->listen(m_Config.m_AcceptBacklog);
		newSocket->startAccepting();
		barrier->post();
	};

	std::shared_ptr<folly::Baton<>> barrier = std::make_shared<folly::Baton<>>();
    m_AcceptorExectuor.add(std::bind(startFunc, barrier));
	barrier->wait();
    
    auto ioExeuctor = m_IOTasksExecutor.get();
    auto acceptor = m_Acceptor.get();
    boost::latch latch(IO_WORKER_COUNT);
    m_IOTasksExecutor->addAll(
        [ioExeuctor, acceptor, newSocket, &latch]()
        {
            auto ioThread = ioExeuctor->GetCurrentIOThread();
            auto ioEventBase = ioThread->m_EventBase;       
            newSocket->getEventBase()->runImmediatelyOrRunInEventBaseThreadAndWait(
                [newSocket, acceptor, ioEventBase, &latch]()
                {
                    newSocket->addAcceptCallback(acceptor, ioEventBase);
                    latch.count_down();
                }
                
            );
        }
    );
    latch.wait();
}

bool XLN::ServerExecutorService::OnDestroy()
{
	ExecutorService::OnDestroy();
	auto destroyFunc = [&](std::shared_ptr<folly::Baton<>> barrier)
	{
		
		m_AcceptorSocket.wlock()->clear();
		barrier->post();
	};

	std::shared_ptr<folly::Baton<>> barrier = std::make_shared<folly::Baton<>>();
    m_AcceptorExectuor.add(std::bind(destroyFunc, barrier));
	barrier->wait();
    m_IOTasksExecutor->StopAllThread();
    m_AcceptorExectuor.StopAllThread();
	return true;
}
