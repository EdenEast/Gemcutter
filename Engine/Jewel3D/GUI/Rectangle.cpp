// Copyright (c) 2020 Emilian Cioca
#include "Jewel3D/Precompiled.h"
#include "Rectangle.h"

namespace Jwl
{
	Rectangle::Rectangle(float _x, float _y, float _width, float _height)
		: x(_x)
		, y(_y)
		, width(_width)
		, height(_height)
	{
	}

	float Rectangle::GetAspectRatio() const
	{
		return width / height;
	}

	bool Rectangle::IsPointInside(float _x, float _y) const
	{
		return _x >= x &&
			_y >= y &&
			_x <= x + width &&
			_y <= y + height;
	}
}
