#pragma once

struct Color
{
	unsigned char r, g, b, a;

	Color();
	Color(const Color& rhs);
	Color(const unsigned char r, 
		  const unsigned char g, 
		  const unsigned char b, 
		  const unsigned char a);

	void SetColor(const unsigned char r, 
				  const unsigned char g, 
				  const unsigned char b, 
				  const unsigned char a);
};