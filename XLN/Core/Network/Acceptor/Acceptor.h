#pragma once

#include <XLN/Core/Config/Config.h>
#include <folly/io/async/AsyncServerSocket.h>

namespace XCr
{

	class XLN_CLASS_DECL Acceptor : public folly::AsyncServerSocket::AcceptCallback, public XCr::MemObj
	{
	public:
		
		Acceptor()
		{

		}

		virtual ~Acceptor()
		{

		}

		void Init(folly::AsyncServerSocket* socket, folly::EventBase* eventBase)
		{
			socket->addAcceptCallback(this, eventBase);
		}
		

		virtual void connectionAccepted(int fd, const folly::SocketAddress& clientAddr) noexcept override;
		virtual void acceptError(const std::exception& ex) noexcept override;
		virtual void acceptStarted() noexcept override;
		virtual void acceptStopped() noexcept override;

	protected:
	};

}