#pragma once

#include <string>
#include "Transform.h"
#include "Color.h"

struct Text
{
	std::string text;
	Transform transform;
	Color color;
};