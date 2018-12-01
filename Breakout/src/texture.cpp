#include "stdafx.h"
#include "texture.h"


Texture2D::Texture2D()
	: width(0), height(0), imgFormat(GL_RGB), intFormat(GL_RGB)
{
	glGenTextures(1, &id);
}


Texture2D::~Texture2D()
{
}

void Texture2D::process(GLuint width, GLuint height, GLint comp, unsigned char * data)
{
	this->width = width;
	this->height = height;
	GLint imgFor, intFor;
	Texture2D::processFormat(imgFor, intFor, comp);
	this->imgFormat = imgFor;
	this->intFormat = intFor;
	glBindTexture(GL_TEXTURE_2D, id);
	glTexImage2D(GL_TEXTURE_2D, 0, intFormat, width, height, 0, imgFormat, GL_UNSIGNED_BYTE, data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture2D::bind() const
{
	glBindTexture(GL_TEXTURE_2D, id);
}

void Texture2D::processFormat(GLint & imgFormat, GLint & intFormat, GLint & comp)
{

	switch (comp)
	{
	case 1:
		imgFormat = GL_RED;
		intFormat = GL_RED;
		break;
	case 3:
		imgFormat = GL_RGB;
		intFormat = GL_RGB;
		break;
	case 4:
		imgFormat = GL_RGBA;
		intFormat = GL_RGBA;
		break;
	default:
		imgFormat = GL_RGB;
		intFormat = GL_RGB;
		break;
	}
}
