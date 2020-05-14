// Copyright (c) 2020 Emilian Cioca
#pragma once
#include "Jewel3D/GUI/Widget.h"
#include "Jewel3D/Resource/Sound.h"
#include "Jewel3D/Resource/Texture.h"
#include "Jewel3D/Resource/Font.h"

#include <functional>
#include <string_view>
#include <string>

namespace Jwl
{
	//
	class Button : public Widget
	{
	public:
		Button(Entity& owner);
		Button(Entity& owner, Font::Ptr font, std::string_view text);
		Button(Entity& owner, Texture::Ptr idle, Texture::Ptr hover, Texture::Ptr pressed);
		Button(Entity& owner, Font::Ptr font, std::string_view text, Texture::Ptr idle, Texture::Ptr hover, Texture::Ptr pressed);

		enum State
		{
			Idle,
			Hover,
			Pressed
		};

		State GetState() const { return state; }

		void Update();

		void SetFont(Font::Ptr font);
		Font::Ptr GetFont() const;

		void SetText(std::string_view string);
		const std::string& GetText() const;

		Sound::Ptr pressedSound;
		Texture::Ptr idleTexture;
		Texture::Ptr hoverTexture;
		Texture::Ptr pressedTexture;
		float textScale = 1.0f;

		std::function<void()> onClick;

	private:
		void UpdateContent() override;

		void SetState(State);
		void SetTexture(State);

		State state = State::Idle;
		Entity::Ptr imageEntity;
		Entity::Ptr textEntity;
		float textWidth = 0.0f;
	};
}
