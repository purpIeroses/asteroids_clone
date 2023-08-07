#include "AsteroidObjects.h"

AsteroidObjects::AsteroidObjects(float size, float angle, float x, float y, ResourceManager& resourceManager) : 
	m_angle(90), m_dx(0), m_dy(0), m_speed(1.0f)
{
	m_size = size;
	resourceManager.LoadImageFromFile(img, "../Assets/Sprites/asteroid1_L.png");
	sprite.SetImage(img);
	sprite.SetSource(0, 0, 50, 50);
	m_speed = 0.5f / size;
	m_angle = angle;
	trans.SetPosition((float)x, (float)y);
	m_dx += (float)sin(angle) * m_speed;
	m_dy += (float)-cos(angle) * m_speed;
	trans.SetScale(m_size, m_size);
	offset.scale = trans.scale;
}

void AsteroidObjects::Update(float dt)
{
	trans.position.x += m_dx;
	trans.position.y += m_dy;
	offset.SetPosition(trans.position.x - ((float)img.width * m_size * 0.5f), trans.position.y - ((float)img.width * m_size * 0.5f));
	offset.rotation = trans.rotation;
}

void AsteroidObjects::Render(RenderManager& renderManager)
{
	renderManager.Render(sprite, offset);
}

