#pragma once

#include <string>
#include "Image.h"
#include "Sprite.h"
#include "Transform.h"
#include "ResourceManager.h"
#include "RenderManager.h"

struct AsteroidObjects
{
	public:
		Transform trans;
		Transform offset;
		Image img;
		Sprite sprite;
		float m_size;
		float m_dx;
		float m_dy;
		float m_angle;
		float m_speed;

		AsteroidObjects(float size, float angle, float x, float y, ResourceManager& resourceManager);
		void Update(float dt);
		void Render(RenderManager& renderManager);
};