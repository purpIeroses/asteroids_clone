#include "FlyingSaucer.h"

FlyingSaucer::FlyingSaucer(float height, float scale, ResourceManager& resourceManager) : 
	m_scale(1), m_forward(false), m_backward(false), m_left(false), m_right(false), m_angle(0), m_dx(0.0f), m_dy(-1.0f), m_speed(0.01f)
{
	resourceManager.LoadImageFromFile(image, "../Assets/Sprites/ship.png");
	sprite.SetImage(image);
	sprite.SetSource(0, 0, 15, 22);
	m_scale = scale;
	trans.SetPosition(320.0f, 240.0f);
	trans.SetScale(m_scale, m_scale);
	offset.scale = trans.scale;
}

void FlyingSaucer::Update(float dt)
{
	if (m_left || m_right || m_forward || m_backward)
	{ 
		m_angle = (float)M_PI / 180.0f * trans.rotation; 
	}
	if (m_left) 
	{ 
		trans.rotation -= 1.5f;
	}
	if (m_right) 
	{ 
		trans.rotation += 1.5f;
	}
	if (m_forward)
	{
		m_dx += (float)sin(m_angle) * m_speed;
		m_dy += -(float)cos(m_angle) * m_speed;
	}
	if (m_backward)
	{
		m_dx -= (float)sin(m_angle) * m_speed;
		m_dy -= -(float)cos(m_angle) * m_speed;
	}
	trans.position.x += m_dx;
	trans.position.y += m_dy;
	offset.SetPosition(trans.position.x - ((float)image.width * m_scale * 0.5f), trans.position.y - ((float)image.width * m_scale * 0.5f));
	offset.rotation = trans.rotation;
}

void FlyingSaucer::Render(RenderManager& renderManager)
{
	renderManager.Render(sprite, offset);
}

void FlyingSaucer::OnKeyDown(KeyCode key)
{
	switch (key)
	{
	case KeyCode::LEFT_ARROW:
		m_left = true;
		return;
		break;

	case KeyCode::RIGHT_ARROW:
		m_right = true;
		return;
		break;

	case KeyCode::UP_ARROW:
		m_forward = true;
		return;
		break;

	case KeyCode::DOWN_ARROW:
		m_backward = true;
		return;
		break;

	case KeyCode::T:
		if (teleportready)
		{
			srand(rd());
			trans.SetPosition(static_cast<float>((rand() % 640) + sprite.source.w), static_cast<float>((rand() % 480) + sprite.source.h));
			teleportready = false;
		}
		break;

	default:
		break;
	}
}

void FlyingSaucer::OnKeyUp(KeyCode key)
{
	switch (key)
	{
	case KeyCode::LEFT_ARROW:
		m_left = false;
		return;
		break;

	case KeyCode::RIGHT_ARROW:
		m_right = false;
		return;
		break;

	case KeyCode::UP_ARROW:
		m_forward = false;
		return;
		break;

	case KeyCode::DOWN_ARROW:
		m_backward = false;
		return;
		break;

	case KeyCode::SPACE:
		bulletready = true;
		break;

	case KeyCode::T:
		teleportready = true;
		break;

	default:
		break;
	}
}
