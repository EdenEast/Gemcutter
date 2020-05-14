// Copyright (c) 2020 Emilian Cioca
#pragma once
#include "Jewel3D/Application/Application.h"
#include "Jewel3D/Application/Event.h"
#include "Jewel3D/GUI/Widget.h"

namespace Jwl
{
	// A root UI widget which automatically resizes to the size of the application's screen.
	// This is intended to be the root widget for a standard UI setup.
	class Screen : public Widget
	{
	public:
		Screen(Entity& owner);

	private:
		Listener<Resize> onResize;
	};
}
