
#pragma once

#include <ionCore.h>
#include <ionMath.h>
#include <ionFramework.h>


class CWindow;

struct SWindowResizedEvent : public IEvent
{
	vec2i Size;
	CWindow * Window;

	SWindowResizedEvent()
		: Window()
	{}
};
