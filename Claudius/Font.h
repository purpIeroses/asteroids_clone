#pragma once

struct Font
{
	Font();
	Font(const Font& rhs);

	unsigned int id;
	unsigned int size;
	unsigned int GetID();
	unsigned int GetSize();
};
