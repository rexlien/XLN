#pragma once

#include <XLN/Core/Config/Config.h>
#include <XLN/Core/Framework/ExecutorService.h>
#include <XLN/Core/Foundation/Executor/IOThreadPoolExecutor.h>

namespace XLN
{
	class XLN_CLASS_DECL ServerClientExecutorService : public XGf::ExecutorService
	{
	public:

		ServerClientExecutorService();
		virtual ~ServerClientExecutorService();

	protected:

		XCr::IOThreadPoolExecutor m_IOExecutor;
		
		virtual bool OnDestroy() override;

	};
}