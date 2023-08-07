#include "Collider.h"

Collider::Collider() : rect(0,0,0,0)
{
}

Collider::Collider(const Collider& rhs) : rect(rhs.rect)
{
}

Collider::Collider(int x, int y, int w, int h) : rect(x,y,w,h)
{
}

void Collider::SetPosition(int x, int y)
{
	rect.x = x;
	rect.y = y;
}

void Collider::SetSize(int w, int h)
{
	rect.w = w;
	rect.h = h;
}
