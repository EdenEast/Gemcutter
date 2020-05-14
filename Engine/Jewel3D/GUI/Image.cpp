// Copyright (c) 2020 Emilian Cioca
#include "Jewel3D/Precompiled.h"
#include "Image.h"
#include "Jewel3D/Rendering/Sprite.h"
#include "Jewel3D/Resource/Material.h"

namespace Jwl
{
	Image::Image(Entity& _owner)
		: Widget(_owner)
	{
		auto material = Material::MakeNew();
		material->depthMode = DepthFunc::None;
		material->shader = Load<Shader>("Shaders/Default/Sprite");

		auto& sprite = owner.Require<Sprite>();
		sprite.SetAlignment(Alignment::Center);
		sprite.SetMaterial(std::move(material));
	}

	Image::Image(Entity& owner, Texture::Ptr tex)
		: Image(owner)
	{
		SetTexture(std::move(tex));
	}

	void Image::SetTexture(Texture::Ptr tex)
	{
		texture = std::move(tex);

		auto material = owner.Get<Renderable>().GetMaterial();
		material->textures.Clear();
		material->textures.Add(texture);
	}

	Texture::Ptr Image::GetTexture()
	{
		return texture;
	}

	void Image::UpdateContent()
	{
		const Rectangle& bounds = GetAbsoluteBounds();

		owner.scale = vec3(bounds.width, bounds.height, 1.0f);
	}
}
