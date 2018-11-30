#include "ServerClientExecutorService.h"

#include <folly/SocketAddress.h>
#include <folly/io/async/EventBaseManager.h>

using namespace XLN;

XLN::ServerClientExecutorService::ServerClientExecutorService()
{

}

XLN::ServerClientExecutorService::~ServerClientExecutorService()
{


}


bool XLN::ServerClientExecutorService::OnDestroy()
{
	ExecutorService::OnDestroy();
	
	return true;
}
