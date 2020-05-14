// Copyright (c) 2020 Emilian Cioca
#pragma once
#include "Jewel3D/GUI/Widget.h"
#include "Jewel3D/Resource/Texture.h"

namespace Jwl
{
	//
	class Image : public Widget
	{
	public:
		Image(Entity& owner);
		Image(Entity& owner, Texture::Ptr tex);

		void SetTexture(Texture::Ptr tex);
		Texture::Ptr GetTexture();

	private:
		void UpdateContent() override;

		Texture::Ptr texture;
	};
}
