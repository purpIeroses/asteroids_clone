#include "Image.h"

Image::Image() : id(-1), width(0), height(0)
{
}

Image::Image(const Image& rhs) : id(rhs.id), width(rhs.width), height(rhs.height)
{
}

unsigned int Image::GetID()
{
	return id;
}

int Image::GetWidth()
{
	return width;
}

int Image::GetHeight()
{
	return height;
}
