#pragma once

#define _USE_MATH_DEFINES
#include <math.h>
#include <string>
#include <vector>
#include "KeyCode.h"
#include "Image.h"
#include "Sprite.h"
#include "Transform.h"
#include <random>
#include "ResourceManager.h"
#include "RenderManager.h"

struct bullet
{
	Transform trans;
	Sprite sprite;
	float angle = 0;
	bool collided = false;
	float timewrapped = 0;
};

struct FlyingSaucer
{
	public:
		Transform trans;
		Transform offset;
		Image image;
		Sprite sprite;
		float m_scale;
		bool m_left;
		bool m_right;
		bool m_forward;
		bool m_backward;
		bool bulletready = true;
		bool teleportready = true;
		float m_dx;
		float m_dy;
		float m_angle;
		float m_speed;
		std::random_device rd;

		FlyingSaucer(float height, float scale, ResourceManager& resourceManager);
		void Update(float dt);
		void Render(RenderManager& renderManager);
		void OnKeyDown(KeyCode key);
		void OnKeyUp(KeyCode key);
};