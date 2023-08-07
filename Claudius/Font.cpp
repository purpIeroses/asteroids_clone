#include "Font.h"

Font::Font() : id(-1), size(-1)
{
}

Font::Font(const Font& rhs) : id(rhs.id), size(rhs.size)
{
}

unsigned int Font::GetID()
{
	return id;
}

unsigned int Font::GetSize()
{
	return size;
}
