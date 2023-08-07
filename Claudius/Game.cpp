#include "Game.h"
#include <assert.h>
#include <stdio.h>
#include "ResourceManager.h"
#include "RenderManager.h"
#include "CollisionManager.h"
#include "SoundManager.h"

Game::Game(ResourceManager& resourceManager, SoundManager& soundManager) : 
	resourceManager(resourceManager), soundManager(soundManager),
	scale(1.0f), level(1), playing(true), score(0), player(Height, scale, resourceManager),
	enemy(1.5f, 50.0f, static_cast<float>(rand() % ((int)Height + 50)), 0, resourceManager)
{
	srand(rd());
	resourceManager.LoadImageFromFile(bulletImg, "../Assets/Sprites/bullet.png");
	spawnAsteroidObjects();
	spawnHealth();
	resourceManager.LoadFontFromFile(font, "../Assets/Fonts/HyperspaceBold.ttf", 20);
	scoreTxt.color.SetColor(255, 255, 255, 0);
	scoreTxt.transform.SetPosition(10, 10);
	scoreTxt.text = "Score:" + std::to_string(score);
}

Game::~Game()
{
	for (bullet* b : bullets)
	{
		delete b;
	}
	for (bullet* b : enemyBullets)
	{
		delete b;
	}
	for (AsteroidObjects* a : asteroids)
	{
		delete a;
	}
	for (Health* h : player_health)
	{
		delete h;
	}
	enemyBullets.clear();
	bullets.clear();
	asteroids.clear();
	player_health.clear();
}

bool Game::Enter(int& width, int& height, std::string& title)
{
	width = (int)Width;
	height = (int)Height;
	title = "Asteroids (1979)";
	return true;
}

bool Game::Update(float dt)
{
	player.Update(dt);
	for (AsteroidObjects* a : asteroids)
	{
		a->Update(dt);
		screenWraparound(a->trans.position.x, a->trans.position.y, a->trans.position.x, a->trans.position.y);
	}
	if (enemy.active)
	{
		enemy.Update(dt);
		screenWraparound(enemy.trans.position.x, enemy.trans.position.y, enemy.trans.position.x, enemy.trans.position.y);
	}
	screenWraparound(player.trans.position.x, player.trans.position.y, player.trans.position.x, player.trans.position.y);
	wrapBullets(bullets);
	wrapBullets(enemyBullets);
	if (bullets.size() > 0)
	{
		for (int i = 0; i < bullets.size(); i++)
		{
			if (bullets[i]->timewrapped >= 30.0f || bullets[i]->collided)
			{
				delete bullets[i];
				bullets.erase(bullets.begin() + i);
			}
		}
	}
	if (enemyBullets.size() > 0)
	{
		for (int i = 0; i < enemyBullets.size(); i++)
		{
			if (enemyBullets[i]->timewrapped >= 30.0f || enemyBullets[i]->collided)
			{
				delete enemyBullets[i];
				enemyBullets.erase(enemyBullets.begin() + i);
			}
		}
	}
	if (score != 0 && (score % 150 == 0) && !enemy.active)
	{
		enemy.active = true;
	}
	if (enemy.active)
	{
		enemy.bullettimer++;
	}
	if (enemy.bullettimer == 60 && enemy.active)
	{
		enemyShoot();
	}
	bulletCollision(bullets);
	bulletCollision(enemyBullets);
	playerCollision();
	if (asteroids.size() <= 0)
	{
		level++;
		spawnAsteroidObjects();
	}
	return playing;
}

void Game::Render(RenderManager& renderManager)
{
	player.Render(renderManager);
	if (enemy.active == true)
	{
		enemy.Render(renderManager);
	}
	for (bullet* b : bullets)
	{
		renderManager.Render(b->sprite, b->trans);
	}
	for (bullet* b : enemyBullets)
	{
		renderManager.Render(b->sprite, b->trans);
	}
	for (AsteroidObjects* a : asteroids) 
	{ 
		a->Render(renderManager);
	}
	for (Health* h : player_health) 
	{ 
		renderManager.Render(h->sprite, h->trans);
	}
	renderManager.Render(font, scoreTxt);
}

void Game::OnKeyDown(KeyCode key)
{
	player.OnKeyDown(key);
	switch (key)
	{
		case KeyCode::SPACE:
			
			if (player.bulletready)
			{
				bullet* b = new bullet();
				b->sprite.SetImage(bulletImg);
				b->sprite.SetSource(0, 0, 2, 2);
				b->trans.SetPosition(player.trans.position.x, player.trans.position.y);
				b->angle = player.m_angle;
				b->trans.scale = player.trans.scale;
				bullets.push_back(b);
				player.bulletready = false;
			}
			break;

		default:
			break;
	}
}

void Game::OnKeyUp(KeyCode key)
{
	player.OnKeyUp(key);
}

void Game::screenWraparound(float inx, float iny, float& outx, float& outy)
{
	outx = inx;
	outy = iny;
	if (inx < 0.0f) 
	{ outx = inx + Width; }
	if (inx >= Width) 
	{ outx = inx - Width; }
	if (iny < 0.0f) 
	{ outy = iny + Height; }
	if (iny >= Height) 
	{ outy = iny - Height; }
}

void Game::screenWraparound(float inx, float iny, float& outx, float& outy, bullet* b)
{
	outx = inx;
	outy = iny;
	if (inx < 0.0f)
	{
		outx = inx + Width; 
		b->timewrapped++;
	}
	if (inx >= Width)
	{ 
		outx = inx - Width; 
		b->timewrapped++;
	}
	if (iny < 0.0f)
	{ 
		outy = iny + Height;
		b->timewrapped++;
	}
	if (iny >= Height)
	{ 
		outy = iny - Height; 
		b->timewrapped++;
	}
}

void Game::spawnAsteroidObjects()
{
	for (int i = 0; i < 3 * level; i++)
	{
		int x;
		int y;

		if (rand() % 2 == 0)
		{
			if (rand() % 2 == 0)
			{
				x = 0;
				y = rand() % ((int)Height + 1);
			}
			else
			{
				x = (int)Width;
				y = rand() % ((int)Height + 1);
			}
		}
		else
		{
			if (rand() % 2 == 0)
			{
				y = 0;
				x = rand() % ((int)Height + 1);
			}
			else
			{
				y = (int)Height;
				x = rand() % ((int)Height + 1);
			}
		}
		AsteroidObjects* a = new AsteroidObjects(1.5f, static_cast<float>(x), static_cast<float>(y), (rand() % 360) * ((float)M_PI / 180.0f), resourceManager);
		asteroids.push_back(a);
	}
}

void Game::spawnEnemy()
{
	int x = (rand() % (int)Width) + 150;
	int y = (rand() % (int)Height) + 150;
	enemy.trans.SetPosition(static_cast<float>(x), static_cast<float>(y));
	enemy.active = true;
}

void Game::spawnHealth()
{
	for (int i = 0; i < 4; i++)
	{
		Health* tempHealth = new Health();
		tempHealth->sprite = player.sprite;
		tempHealth->trans.SetPosition((float)i * player.image.width * 1.0f + 10.0f, 35);
		tempHealth->trans.SetScale(scale, scale);
		player_health.push_back(tempHealth);
	}
}

void Game::playerCollision()
{
	if (asteroids.size() > 0)
	{
		for (AsteroidObjects* a : asteroids)
		{
			if (sqrt(
				pow(a->trans.position.x - player.trans.position.x, 2) +
				pow(a->trans.position.y - player.trans.position.y, 2))
				< ((float)a->img.width * a->m_size / 2.0f) + (player.image.width * player.m_scale / 4.0f))
			{
				delete player_health[player_health.size() - 1];
				player_health.pop_back();
				player.trans.SetPosition(Width / 2.0f, Height / 2.0f);

				if (player_health.size() == 0)
				{
					playing = false;
				}
			}
		}
	}
	if (enemyBullets.size() > 0)
	{
		for (int i = 0; i < enemyBullets.size(); i++)
		{
			if (sqrt(
				pow(player.trans.position.x - enemyBullets[i]->trans.position.x, 2)
				+ pow(player.trans.position.y - enemyBullets[i]->trans.position.y, 2))
				< ((float)player.image.width * 1.5f / 2.0f))
			{
				player_health.pop_back();
				player.trans.SetPosition(Width / 2.0f, Height / 2.0f);

				if (player_health.size() == 0)
				{
					playing = false;
				}
				enemyBullets[i]->collided = true;
			}
		}
	}
}

void Game::bulletCollision(std::vector<bullet*> bulletvector)
{
	if (bulletvector.size() > 0)
	{
		for (int i = 0; i < bulletvector.size(); i++)
		{
			bulletvector[i]->trans.position.x += (float)sin(bulletvector[i]->angle) * 5.0f;
			bulletvector[i]->trans.position.y += -(float)cos(bulletvector[i]->angle) * 5.0f;
			if (bulletvector == bullets)
			{
				splitAsteroids(i);
				if (enemy.active)
				{
					enemyCollision(i);
				}
			}
		}
	}
}

void Game::enemyShoot()
{
	bullet* b = new bullet();
	b->sprite.SetImage(bulletImg);
	b->sprite.SetSource(0, 0, 2, 2);
	b->trans.SetPosition(enemy.trans.position.x, enemy.trans.position.y);
	b->angle = static_cast<float>(rand() % 360);
	b->trans.scale = enemy.trans.scale;
	enemyBullets.push_back(b);
	enemy.bullettimer = 0;
}

void Game::enemyCollision(int i)
{
	if (sqrt(
		pow(enemy.trans.position.x - bullets[i]->trans.position.x, 2)
		+ pow(enemy.trans.position.y - bullets[i]->trans.position.y, 2))
		< (float)enemy.img.GetWidth() * enemy.m_size / 1.5f)
	{
		score += 20 * level;
		scoreTxt.text = "Score:" + std::to_string(score);
		int x = (rand() % (int)Width) + 150;
		int y = (rand() % (int)Height) + 150;
		enemy.trans.SetPosition(static_cast<float>(x), static_cast<float>(y));
		enemy.active = false;
		bullets[i]->collided = true;
	}
}

void Game::wrapBullets(std::vector<bullet*> bulletvector)
{
	for (bullet* b : bulletvector)
	{
		if (b->timewrapped > 0)
		{
			b->timewrapped++;
		}
		screenWraparound(b->trans.position.x, b->trans.position.y, b->trans.position.x, b->trans.position.y, b);
	}
}

void Game::splitAsteroids(int i)
{
	if (asteroids.size() > 0)
	{
		for (int j = 0; j < asteroids.size(); j++)
		{
			if (sqrt(
				pow(asteroids[j]->trans.position.x - bullets[i]->trans.position.x, 2)
				+ pow(asteroids[j]->trans.position.y - bullets[i]->trans.position.y, 2))
				< (float)asteroids[j]->img.GetWidth() * asteroids[j]->m_size / 2.0f)
			{
				score += 10 * level;
				scoreTxt.text = "Score:" + std::to_string(score);
				if (asteroids[j]->m_size >= 0.75f)
				{
					AsteroidObjects* ast1 = new AsteroidObjects(asteroids[j]->m_size * 0.5f, (rand() % 360) * ((float)M_PI / 180.0f),
						asteroids[j]->trans.position.x, asteroids[j]->trans.position.y, resourceManager);
					asteroids.push_back(ast1);
					AsteroidObjects* ast2 = new AsteroidObjects(asteroids[j]->m_size * 0.5f, (rand() % 360) * ((float)M_PI / 180.0f),
						asteroids[j]->trans.position.x, asteroids[j]->trans.position.y, resourceManager);
					asteroids.push_back(ast2);

				}
				delete asteroids[j];
				asteroids.erase(asteroids.begin() + j);
				bullets[i]->collided = true;
			}
		}
	}
}