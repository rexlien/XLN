#pragma once

#include <XLN/Core/Config/Config.h>
#include <XLN/Core/Framework/Application.h>

namespace XLN
{
	class XLN_CLASS_DECL GameApplication : public XGf::Application
	{
	public:

	protected:
		virtual void OnInitService() override;

	};
}