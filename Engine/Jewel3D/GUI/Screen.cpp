// Copyright (c) 2020 Emilian Cioca
#include "Jewel3D/Precompiled.h"
#include "Screen.h"
#include "Jewel3D/Application/HierarchicalEvent.h"
#include "Jewel3D/Entity/Hierarchy.h"
#include "Jewel3D/Input/Input.h"

namespace Jwl
{
	Screen::Screen(Entity& _owner)
		: Widget(_owner)
	{
		owner.Require<Hierarchy>();
		ASSERT(owner.Get<Hierarchy>().IsRoot(), "A Screen widget must be a root.");

		owner.Require<HierarchicalDispatcher<MouseMoved>>();
		owner.Require<HierarchicalDispatcher<MouseScrolled>>();
		owner.Require<HierarchicalDispatcher<KeyPressed>>();
		owner.Require<HierarchicalDispatcher<KeyReleased>>();

		top.offset   = static_cast<float>(Application.GetScreenHeight());
		right.offset = static_cast<float>(Application.GetScreenWidth());

		onResize = [this](const Resize& e) {
			top.offset   = static_cast<float>(e.height);
			right.offset = static_cast<float>(e.width);
		};
	}
}
