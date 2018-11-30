#pragma once

#include <XLN/Core/Config/Config.h>
#include <folly/io/async/EventBase.h>
#include <XLN/Core/Framework/Application.h>

namespace XLN
{
	class XLN_CLASS_DECL ServerClientApplication : public XGf::Application
	{
	public:
		virtual void Main(int argc, char** argv) override;
	protected:
		virtual void OnInitService() override;

	

	};
}