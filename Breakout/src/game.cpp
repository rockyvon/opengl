#include "stdafx.h"
#include "game.h"

Game::Game()
	: state(GAME_ACTIVE)
{
}

Game::~Game()
{
}

void Game::init(ShaderManager* shader)
{
	this->shader = shader;

	TextureMng::addTexture("paddle", TextureMng::loadTexture(img_paddle_path));
	TextureMng::addTexture("ball", TextureMng::loadTexture(img_ball_path));
	TextureMng::addTexture("bg", TextureMng::loadTexture(img_bg_path));
	TextureMng::addTexture("block", TextureMng::loadTexture(img_block_path));
	TextureMng::addTexture("block_solide", TextureMng::loadTexture(img_blocksl_path));

	player = new GameObject(
		TextureMng::getTexture("paddle"),
		vec2((window_width - PLAYER_SIZE.x) / 2, window_height - PLAYER_SIZE.y - 10),
		PLAYER_SIZE,
		vec3(1.0f),
		true,
		vec2(PLAYER_VELOCITY)
	);

	ball = new Ball(TextureMng::getTexture("ball"),
		calculateBPos(),
		bradius,
		ballVelo);
	background = new GameObject(
		TextureMng::getTexture("bg"), 
		vec2(0.0f),
		vec2(window_width, window_height),
		vec3(1.0f),
		false,
		vec2(0.0f),
		true);
	levels.push_back(new Level(level1_cpath, window_width, 300));
	levels.push_back(new Level(level2_cpath, window_width, 300));
	levels.push_back(new Level(level3_cpath, window_width, 300));
	levels.push_back(new Level(level4_cpath, window_width, 300));
	level = 0;
}

void Game::update()
{
	/*sprite->setTransform(
		vec2(0.0f, 0.0f), 
		vec2(500.0f, 300.0f));*/
	ball->move(window_width);
	levels[level]->checkCollision(ball);
}

void Game::render()
{
	background->draw(shader);
	player->draw(shader);
	levels[level]->draw(shader);
	ball->draw(shader);
}

void Game::refreshWindow(int window_width, int window_height)
{
	this->window_width = window_width;
	this->window_height = window_height;

}

void Game::realign(float x, float y)
{
	for(Level* level : levels)
		level->realign(x, y);
	vec2 bgsize = background->getSize();
	background->realign(vec2(0.0f), vec2(bgsize.x * x, bgsize.y * y));
}

void Game::processButton(unsigned char button)
{
	
	if (!player)
		return;
	float v = 20.0f;
	vec2 right = vec2(3.0f, 0.0f);
	if (button == 'a')
	{
		if (player->getPosition().x - v >= 0) {
			player->otranslatex(-v);
		}
	}
	else if (button == 'd')
	{
		if (player->getPosition().x + player->getSize().x + v <= window_width) {
			player->otranslatex(v);
		}
	}
	if (ball->stuck) {
		ball->setPosition(calculateBPos());
	}

	if (button == 's') {
		ball->stuck = false;
	}
}

vec2 Game::calculateBPos()
{
	vec2 ppos = player->getPosition();
	vec2 psize = player->getSize();
	vec2 bpos = vec2(ppos.x + psize.x / 2 - bradius, ppos.y - bradius * 2);
	return bpos;
}
