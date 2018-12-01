#include "stdafx.h"
#include "level.h"


Level::Level()
{
}

Level::Level(string fpath, int width, int height)
{
	load(fpath, width, height);
}


Level::~Level()
{
}

void Level::load(string fpath, int width, int height)
{
	ifstream infile;
	infile.open(fpath.data());   //将文件流对象与文件连接起来 
	assert(infile.is_open());   //若失败,则输出错误消息,并终止程序运行 

	string line;
	vector<vector<unsigned int>> result;
	while (getline(infile, line))
	{
		vector<unsigned int> row;
		std::istringstream sstream(line);
		unsigned int code;
		while (sstream >> code) // 读取被空格分隔的每个数字
			row.push_back(code);
		result.push_back(row);
		
	}
	infile.close();
	if (result.size() > 0)
		init(result, width, height);
	else
		cout << "关卡配置文件不能为空！" << endl;
}

void Level::draw(ShaderManager * shader)
{
	for (GameObject &tile : bricks)
		if (!tile.isDestoryed())
			tile.draw(shader);
}

bool Level::isCompleted()
{
	for (GameObject &tile : bricks)
		if (!tile.isSolid() && !tile.isDestoryed())
			return GL_FALSE;
	return GL_TRUE;
}

void Level::realign(float x, float y)
{
	for (GameObject &tile : bricks) {
		vec2 osize = tile.getSize();
		vec2 oposition = tile.getPosition();
		vec2 nsize = vec2(osize.x * x, osize.y * y);
		float rc = oposition.x / osize.x;
		float cc = oposition.y / osize.y;
		vec2 nposition = vec2(rc * nsize.x, cc * nsize.y);
		tile.realign(nposition, nsize);
	}
}

void Level::checkCollision(GameObject * ball)
{
	for (GameObject& brick : bricks) {
		if (!brick.isDestoryed()) {
			if(ball->checkCollision(brick))
				if (!brick.isSolid()) {
					brick.destory();
				}
		}
	}
}

void Level::init(vector<vector<unsigned int>> data, int winWidth, int winHeight)
{
	unsigned int i = 0, j = 0;
	unsigned int rowCount = data.size();
	unsigned int colCount = data[0].size();
	float unitWidth = float(winWidth) / colCount;
	float unitHeight = float(winHeight) / rowCount;
	for (i = 0; i < rowCount; i++)
	{
		for (j = 0; j < colCount; j++)
		{
			vec2 position(unitWidth * j, unitHeight * i);
			vec2 size(unitWidth, unitHeight);

			if (data[i][j] == BRICK_SOLIDE)
			{
				GameObject obj(TextureMng::getTexture("block_solide"), position, size, vec3(1.0f), true);
				bricks.push_back(obj);
			}
			else if (data[i][j] > 1)
			{
				vec3 color = vec3(1.0f); // 默认为白色
				if (data[i][j] == BRICK_COLOR_1)
					color = vec3(0.2f, 0.6f, 1.0f);
				else if (data[i][j] == BRICK_COLOR_2)
					color = vec3(0.0f, 0.7f, 0.0f);
				else if (data[i][j] == BRICK_COLOR_3)
					color = vec3(0.8f, 0.8f, 0.4f);
				else if (data[i][j] == BRICK_COLOR_4)
					color = vec3(1.0f, 0.5f, 0.0f);

				bricks.push_back(
					GameObject(TextureMng::getTexture("block"), position, size, color)
				);
			}
		}
	}
}
