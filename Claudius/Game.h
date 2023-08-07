#pragma once

#include <random>
#include <string>
#include <vector>

#include "Collider.h"
#include "KeyCode.h"
#include "Font.h"
#include "Text.h"
#include "Image.h"

#include "FlyingSaucer.h"
#include "AsteroidObjects.h"
#include "EnemyShip.h"

struct RenderManager;
struct ResourceManager;
struct SoundManager;

struct Health
{
	Sprite sprite;
	Transform trans;
};

class Game
{
	ResourceManager& resourceManager;
	SoundManager& soundManager;
public:
	
	// references to a resource and sound manager sent in the constructor
	Game(ResourceManager& resourceManager, SoundManager& soundManager);
	~Game();

	// References sent to the game from the base to set the window size and name
	bool Enter(int& width, int& height, std::string& title); 

	// If Games update returns false the program will exit, dt is milliseconds passed since last frame
	bool Update(float dt);

	// reference to the rendermanager sent in the function call, make sure to separate update logic from render logic
	void Render(RenderManager& rendererManager);
	void OnKeyDown(KeyCode key);
	void OnKeyUp(KeyCode key);
	void screenWraparound(float inx, float iny, float& outx, float& outy);
	void screenWraparound(float inx, float iny, float& outx, float& outy, bullet* b);
	void playerCollision();
	void enemyCollision(int i);
	void bulletCollision(std::vector<bullet*> bulletvector);
	void spawnAsteroidObjects();
	void spawnEnemy();
	void enemyShoot();
	void wrapBullets(std::vector<bullet*> bulletvector);
	void spawnHealth();
	void splitAsteroids(int i);

	float Width = 640;
	float Height = 480;
	float scale;
	int level;
	bool playing;
	int score = 0;
	int randNum;

	Image bulletImg;
	Text scoreTxt;
	Font font;
	FlyingSaucer player;
	EnemyShip enemy;

	std::vector<AsteroidObjects*> asteroids;
	std::random_device rd;
	std::vector<bullet*> bullets;
	std::vector<bullet*> enemyBullets;
	std::vector<Health*>  player_health;
};