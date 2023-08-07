#pragma once

#include <string>
#include "Image.h"
#include "Sprite.h"
#include "Transform.h"
#include "ResourceManager.h"
#include "RenderManager.h"

struct EnemyShip
{
	public:
		Transform trans;
		Transform offset;
		Image img;
		Sprite sprite;
		int bullettimer = 0;
		bool collided = false;
		bool active = false;
		float m_size;
		float m_dx;
		float m_dy;
		float m_angle;
		float m_speed;

		EnemyShip();
		EnemyShip(float size, float angle, float x, float y, ResourceManager& resourceManager);
		void Update(float dt);
		void Render(RenderManager& renderManager);
};