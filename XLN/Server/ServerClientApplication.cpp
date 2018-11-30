#include "ServerClientApplication.h"
#include <folly/io/async/EventBaseManager.h>
#include <folly/io/async/AsyncServerSocket.h>
#include <XLN/Core/Framework/System.h>
#include <XLN/Core/Foundation/ServiceMgr.h>
#include <XLN/Server/ServerClientExecutorService.h>
#undef signal_set
#include <boost/asio.hpp>

using namespace XLN;

void ServerClientApplication::Main(int argc, char** argv)
{
	XGf::Application::Main(argc, argv);
	
	boost::asio::io_service io_service;
#if (XLN_PLATFORM == XLN_PLATFORM_WIN32)
    boost::asio::signal_set termSignal(io_service, SIGINT, SIGTERM, SIGBREAK);
#else
    boost::asio::signal_set termSignal(io_service, SIGINT, SIGTERM);
#endif
	termSignal.async_wait(
		[&io_service](
			const boost::system::error_code& error,
			int signal_number)
	{
		std::cout << "Got signal " << signal_number << "; "
			"stopping io_service." << std::endl;
		XGf::System::Shutdown();
		io_service.stop();
	});
	io_service.run();
	

}

void ServerClientApplication::OnInitService()
{
	auto executorService = XLN_OBJ_NEW ServerClientExecutorService();
	XCr::ServiceMgr::GetActiveServiceMgr()->RegisterService(executorService);

}

