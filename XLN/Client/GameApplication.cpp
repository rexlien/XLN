#include "GameApplication.h"
#include <XLN/Core/Foundation/ServiceMgr.h>
#include <XLN/Client/ClientExecutorService.h>

using namespace XLN;

void XLN::GameApplication::OnInitService()
{
	XCr::ServiceMgr::GetActiveServiceMgr()->RegisterService(XLN_OBJ_NEW XLN::ClientExecutorService());
}
