
#pragma once

#include <ionCore.h>
#include <ionMath.h>
#include <ionFramework.h>
#include <ionWindow.h>


namespace ion
{


	class CDefaultApplication : public IEventListener
	{

	public:

		struct SSettings
		{
			vec2i WindowSize = vec2i(1980, 1080);
			EWindowType WindowType = EWindowType::Windowed;
		};

		void LoadSettings();

	protected:

		SSettings ApplicationSettings;

	};

}
