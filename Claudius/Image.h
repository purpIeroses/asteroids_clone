#pragma once

struct Image
{
	Image();
	Image(const Image& rhs);

	unsigned int id;
	int width;
	int height;

	unsigned int GetID();
	int GetWidth();
	int GetHeight();
};