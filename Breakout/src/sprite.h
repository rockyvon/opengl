#ifndef SPRITE_H
#define SPRITE_H

#include "opengl.h"
#include "shader.h"
#include "texture.h"

class Sprite
{
public:
	Sprite();
	Sprite(Texture2D* tex);
	~Sprite();
	void setTexture(Texture2D* tex);
	void draw(ShaderManager* shader,
		vec2& position,
		vec2& size, 
		float& rotation, 
		vec3& color);
private:
	Texture2D* texture;
	GLuint vao;
	void initData();
};

#endif // !SPRITE_H

