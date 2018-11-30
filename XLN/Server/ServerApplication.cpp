//
// Created by Rex Lien on 18/08/2017.
//

#include "ServerApplication.h"
#include <folly/io/async/EventBaseManager.h>
#include <folly/io/async/AsyncServerSocket.h>
#include <XLN/Core/Framework/System.h>
#include <XLN/Core/Foundation/ServiceMgr.h>
#include <XLN/Server/ServerExecutorService.h>
#undef signal_set
#include <boost/asio.hpp>

//#include <fbthrift/

using namespace XLN;
using namespace folly;

void ServerApplication::Main(int argc, char** argv)
{
	XGf::Application::Main(argc, argv);
	StartServer(argc, argv);
	
	//handle input signal
	boost::asio::io_service io_service;
#if (XLN_PLATFORM == XLN_PLATFORM_WIN32)
    boost::asio::signal_set termSignal(io_service, SIGINT, SIGTERM, SIGBREAK);
#else
	boost::asio::signal_set termSignal(io_service, SIGINT, SIGTERM);
#endif
	termSignal.async_wait(
		[this, &io_service](
			const boost::system::error_code& error,
			int signal_number)
	{
		std::cout << "Got signal " << signal_number << "; "
			"stopping io_service." << std::endl;
		this->StopServer();
		XGf::System::Shutdown();
		io_service.stop();
	});
	io_service.run();
	
}

void ServerApplication::OnInitService()
{
	
}

void ServerApplication::StartServer(int argc, char** argv)
{
    auto executorService = XLN_OBJ_NEW ServerExecutorService();
    XCr::ServiceMgr::GetActiveServiceMgr()->RegisterService(executorService);
    executorService->StartAccpet();
}

void ServerApplication::StopServer()
{

}

