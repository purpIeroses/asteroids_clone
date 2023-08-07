#include "Sprite.h"
Sprite::Sprite() : source(0, 0, 0, 0), flip(Flip::None), image(nullptr)
{
}

void Sprite::SetImage(Image& image)
{
	this->image = &image;
}

void Sprite::SetSource(int x, int y, int w, int h)
{
	this->source.SetBounds(x, y, w, h);
}

void Sprite::SetFlip(Flip flip)
{
	this->flip = flip;
}
