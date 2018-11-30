#pragma once

#include <XLN/Core/Config/Config.h>
#include <XLN/Core/Framework/ExecutorService.h>
#include <XLN/Core/Foundation/Executor/IOThreadPoolExecutor.h>
#include <XLN/Core/Network/Fwd.h>
#include <folly/io/async/AsyncServerSocket.h>



namespace XLN
{
	class XLN_CLASS_DECL ServerExecutorService : public XGf::ExecutorService
	{
		public:

			struct ServerConfig
			{
			public:
				ServerConfig()
				{
					m_MaxAcceptor = 1;
					m_Port = 10000;
					m_UseFastOpen = false;
					m_AcceptBacklog = 32;
				}
				int m_MaxAcceptor;
				int m_Port;
				int m_AcceptBacklog;
				bool m_UseFastOpen;
				//folly::IPAddress m_Address;
			};

			ServerExecutorService(ServerConfig config);
			ServerExecutorService();
			virtual ~ServerExecutorService();

			void StartAccpet();

		protected:

			XCr::IOThreadPoolExecutor m_AcceptorExectuor;
            std::unique_ptr<XCr::Acceptor> m_Acceptor;
            ServerConfig m_Config;
			folly::Synchronized<std::list<XCr::SharedPtr<folly::AsyncServerSocket> >> m_AcceptorSocket;
            
			virtual bool OnDestroy() override;

	};
}