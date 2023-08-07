#pragma once

#include "Rectangle.h"
#include "Color.h"

struct Image;

struct Sprite
{
	enum class Flip
	{
		None,
		Horizontal,
		Vertical
	};

	Sprite();
	
	void SetImage(Image& image);
	void SetSource(int x, int y, int w, int h);
	void SetFlip(Flip flip);

	Rectangle source;
	Flip flip;
	const Image* image;
};