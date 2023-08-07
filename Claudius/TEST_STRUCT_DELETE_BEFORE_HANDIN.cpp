#include "TEST_STRUCT_DELETE_BEFORE_HANDIN.h"
#include "RenderManager.h"

void TEST_STRUCT_DELETE_BEFORE_HANDIN::Update(float dt)
{
	transform.ChangeRotation(50 * dt);
	collider.SetSize(sprite.source.w * transform.scale.x, sprite.source.h * transform.scale.y);
	collider.SetPosition(static_cast<int>(transform.GetPosition().x), static_cast<int>(transform.GetPosition().y));
}
 
void TEST_STRUCT_DELETE_BEFORE_HANDIN::Render(RenderManager& renderManager)
{
	renderManager.Render(sprite, transform);
	renderManager.Render(collider.rect, color);
}

void TEST_STRUCT_DELETE_BEFORE_HANDIN::OnKeyDown(KeyCode key)
{
	if (key == KeyCode::A)
	{
		transform.SetRotation(90);
		transform.ChangePosition(-moveSpeed, 0);
	}
	if (key == KeyCode::D)
	{
		transform.ChangePosition(moveSpeed, 0);
	}
	if (key == KeyCode::W)
	{
		transform.ChangePosition(0,-moveSpeed);
	}
	if (key == KeyCode::S)
	{
		transform.ChangePosition(0,moveSpeed);
	}

	if (key == KeyCode::Q)
	{
		transform.ChangeScale(-0.05f, .0f);
	}
	if (key == KeyCode::E)
	{
		transform.ChangeScale(0.05f, .0f);
	}
	if (key == KeyCode::Z)
	{
		transform.ChangeScale(.0f, -0.05f);
	}
	if (key == KeyCode::C)
	{
		transform.ChangeScale(.0f, 0.05f);
	}
}
