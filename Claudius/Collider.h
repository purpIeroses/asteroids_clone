#pragma once

#include "Rectangle.h"

struct Collider
{
	Collider();
	Collider(const Collider& rhs);
	Collider(int x, int y, int w, int h);

	void SetPosition(int x, int y);
	void SetSize(int w, int h);

	Rectangle rect;
};