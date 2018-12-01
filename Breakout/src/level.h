#ifndef LEVEL_H
#define LEVEL_H
#include "stdafx.h"
#include "object.h"
#include "resource.h"
#include <sstream>

class Level
{
public:
	Level();
	Level(string fpath, int width, int height);
	~Level();
	void load(string fpath, int width, int height);
	void draw(ShaderManager* shader);
	bool isCompleted();
	void realign(float x, float y);
	void checkCollision(GameObject* ball);
private:
	vector<GameObject> bricks;
	void init(vector<vector<unsigned int>> data, int winWidth, int winHeight);
	const unsigned int
		BRICK_SOLIDE = 1,
		BRICK_COLOR_1 = 2,
		BRICK_COLOR_2 = 3,
		BRICK_COLOR_3 = 4,
		BRICK_COLOR_4 = 5;

};

#endif // !LEVEL_H

