#include "Color.h"

Color::Color() : r(0), g(0), b(0), a(0) 
{
}

Color::Color(const Color& rhs) : r(rhs.r), g(rhs.g), b(rhs.b), a(rhs.a)
{
}

Color::Color(const unsigned char r, const unsigned char g, const unsigned char b, const unsigned char a) : r(r), g(g), b(b), a(a)
{
}

void Color::SetColor(const unsigned char r, const unsigned char g, const unsigned char b, const unsigned char a)
{
	this->r = r; this->g = g;
	this->b = b; this->a = a;
}
