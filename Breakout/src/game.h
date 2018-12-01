#ifndef GAME_H
#define GAME_H

#include "opengl.h"
#include "level.h"
#include "resource.h"
#include "gameconfig.h"

class Game
{
public:
	GameState state;

	Game();
	~Game();
	void init(ShaderManager* shader);
	void update();
	void render();
	void refreshWindow(int window_width, int window_height);
	void realign(float x, float y);
	void processButton(unsigned char button);

private:
	int window_width, window_height;
	ShaderManager* shader;
	const vec2 PLAYER_SIZE = vec2(100, 20);
	const float PLAYER_VELOCITY = 500.0f;
	string img_paddle_path = "texture/paddle.png";
	string img_ball_path = "texture/ball.png";
	string img_bg_path = "texture/girls.jpg";
	string img_block_path = "texture/block.png";
	string img_blocksl_path = "texture/block_solid.png";
	string level1_cpath = "config/level1.cfg";
	string level2_cpath = "config/level2.cfg";
	string level3_cpath = "config/level3.cfg";
	string level4_cpath = "config/level4.cfg";
	vector<Level*> levels;
	unsigned int level;
	GameObject* background;
	GameObject* player;
	Ball* ball;
	float bradius = 20.0f;
	vec2 ballVelo = vec2(0.5f, -0.5f);
	vec2 calculateBPos();
};

#endif // !GAME_H

