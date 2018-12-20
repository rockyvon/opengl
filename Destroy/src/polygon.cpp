#include "stdafx.h"
#include "polygon.h"

#include "stb_image.h"

void Polygon::init()
{
	
}

void Polygon::initTexture(const char* path, GLuint *texture, bool gamma)
{
	glGenTextures(1, texture);
	glBindTexture(GL_TEXTURE_2D, *texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int width, height, nrChannels;

	unsigned char *data = stbi_load(path, &width, &height, &nrChannels, 0);
	if (data)
	{
		GLenum format_img;
		GLenum format_buf;
		if (nrChannels == 1) {
			format_img = GL_RED;
			format_buf = GL_RED;
		}


		else if (nrChannels == 3) {
			format_img = gamma ? GL_SRGB : GL_RGB;
			format_buf = GL_RGB;
		}


		else if (nrChannels == 4) {
			format_img = gamma ? GL_SRGB_ALPHA : GL_RGBA;
			format_buf = GL_RGBA;
		}
		glTexImage2D(GL_TEXTURE_2D, 0, format_img, width, height, 0, format_buf, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture: " << path << std::endl;
	}
	stbi_image_free(data);
}

void Cube::init(float length = 1.0f)
{
	this->length = length;
	initCubeVertex();
}

void Cube::initCubeVertex()
{
	float vertices[32] = {
		-0.5f, -0.5f, 0.0f, -1.0f, -1.0f, 1.0f, 0.0f, 0.0f,//左下角
		 0.5f, -0.5f, 0.0f,  1.0f, -1.0f, 1.0f, 1.0f, 0.0f,//右下角
		 0.5f,  0.5f, 0.0f,  1.0f,  1.0f, 1.0f, 1.0f, 1.0f,//右上角
		-0.5f,  0.5f, 0.0f, -1.0f,  1.0f, 1.0f, 0.0f, 1.0f//左上角
	};
	vertices[0] = vertices[1] = vertices[9] = vertices[24] = -length / 2;
	vertices[8] = vertices[16] = vertices[17] = vertices[25] = length / 2;
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void *)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void *)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void *)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);
}

void Cube::initCubeTexture(const char * dif_path, const char * spe_path, bool gamma)
{
	initTexture(dif_path, &texture_dif, gamma);
	initTexture(spe_path, &texture_spe, gamma);
}

void Cube::draw(ShaderManager * shader, mat4 model)
{
	glBindVertexArray(vao);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture_dif);
	shader->setUniform1i("material.diffuse0", 0);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture_spe);
	shader->setUniform1i("material.specular0", 1);
	shader->setUniform1i("material.shininess", 8);

	//前面
	mat4 model_local = translate(model, vec3(0.0f, 0.0f, length/2));
	shader->setUniformMatrix4fv("model", 1, value_ptr(model_local));
	glDrawArrays(GL_POLYGON, 0, 4);
	//后面
	model_local = rotate(model_local, radians(180.0f), vec3(0.0f, 1.0f, 0.0f));
	model_local = translate(model_local, vec3(0.0f, 0.0f, length));
	shader->setUniformMatrix4fv("model", 1, value_ptr(model_local));
	glDrawArrays(GL_POLYGON, 0, 4);
	//右面
	model_local = rotate(model, radians(90.0f), vec3(0.0f, 1.0f, 0.0f));
	model_local = translate(model_local, vec3(0.0f, 0.0f, length / 2));
	shader->setUniformMatrix4fv("model", 1, value_ptr(model_local));
	glDrawArrays(GL_POLYGON, 0, 4);
	//左面
	model_local = rotate(model_local, radians(180.0f), vec3(0.0f, 1.0f, 0.0f));
	model_local = translate(model_local, vec3(0.0f, 0.0f, length));
	shader->setUniformMatrix4fv("model", 1, value_ptr(model_local));
	glDrawArrays(GL_POLYGON, 0, 4);
	//下面
	model_local = rotate(model, radians(90.0f), vec3(2.0f, 0.0f, 0.0f));
	model_local = translate(model_local, vec3(0.0f, 0.0f, length / 2));
	shader->setUniformMatrix4fv("model", 1, value_ptr(model_local));
	glDrawArrays(GL_POLYGON, 0, 4);
	//上面
	model_local = rotate(model_local, radians(180.0f), vec3(0.0f, 1.0f, 0.0f));
	model_local = translate(model_local, vec3(0.0f, 0.0f, length));
	shader->setUniformMatrix4fv("model", 1, value_ptr(model_local));
	glDrawArrays(GL_POLYGON, 0, 4);
}

void Panel::init(float width, float height)
{
	this->width = width;
	this->height = height;
	initPanelVertex();
}

void Panel::draw(ShaderManager * shader, mat4 model)
{
	glBindVertexArray(vao);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture_dif);
	shader->setUniform1i("material.diffuse0", 0);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture_spe);
	shader->setUniform1i("material.specular0", 1);
	shader->setUniform1i("material.shininess", 8);

	shader->setUniformMatrix4fv("model", 1, value_ptr(model));
	glDrawArrays(GL_POLYGON, 0, 4);
}

void Panel::initPanelTexture(const char * dif_path, const char * spe_path, bool gamma)
{
	initTexture(dif_path, &texture_dif, gamma);
	initTexture(spe_path, &texture_spe, gamma);
}

void Panel::initPanelVertex()
{
	float vertices[32] = 
	{
		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,//左下角
		 0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,//右下角
		 0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,//右上角
		-0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f//左上角
	};

	vertices[0] = vertices[24] = -width / 2;
	vertices[1] = vertices[9] = -height / 2;
	vertices[8] = vertices[16] = width / 2;
	vertices[17] = vertices[25] = height / 2;

	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void *)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void *)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void *)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);
}
