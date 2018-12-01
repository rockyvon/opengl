#include "stdafx.h"
#include "sprite.h"


Sprite::Sprite()
{
	initData();
}

Sprite::Sprite(Texture2D* tex)
{
	initData();
	setTexture(tex);
}


Sprite::~Sprite()
{
}

void Sprite::setTexture(Texture2D* tex)
{
	this->texture = tex;
}

void Sprite::draw(ShaderManager * shader,
	vec2& position,
	vec2& size,
	float& rotation,
	vec3& color)
{
	shader->use();
	mat4 model = mat4(1.0f);

	model = translate(model, vec3(position, 0.0f));

	model = translate(model, vec3(0.5 * size.x, 0.5 * size.y, 0.0f));
	model = rotate(model, radians(rotation), vec3(0.0f, 0.0f, 1.0f));
	model = translate(model, vec3(-0.5 * size.x, -0.5 * size.y, 0.0f));

	model = scale(model, vec3(size, 1.0));

	shader->setUniformMatrix4fv("model", 1, value_ptr(model));
	shader->setUniform3fv("color", 1, value_ptr(color));

	glActiveTexture(GL_TEXTURE0);
	texture->bind();
	shader->setUniform1i("image", 0);

	glBindVertexArray(vao);
	glDrawArrays(GL_POLYGON, 0, 4);
	glBindVertexArray(0);
}

void Sprite::initData()
{
	float vertices[] = {
		// Î»ÖÃ     // ÎÆÀí
		0.0f, 0.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		0.0f, 1.0f, 0.0f, 1.0f,
	};
	GLuint vbo;
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindVertexArray(this->vao);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)(2 * sizeof(GLfloat)));

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	
}
