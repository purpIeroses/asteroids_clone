#pragma once

#include <vector>
#include <string>

struct Sprite;
struct Transform;
struct Rectangle;
struct Color;
struct Font;
struct Text;
struct RenderImpl;

struct RenderManager
{
	RenderManager(RenderImpl& resourceImpl);
	~RenderManager();
	void Render(const Sprite& sprite, const Transform& transform); // renders a sprite on a specific transform
	void Render(const Rectangle& rect, const Color& color); // renders a rectangle of a color 
	void Render(const Font& font, const Text& text); // renders a text with a specific font, text is rendered with color and transform

	RenderImpl& impl;
};
