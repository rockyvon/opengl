#ifndef TEXTURE_H
#define TEXTURE_H

#include "opengl.h"

class Texture2D
{
public:
	GLuint id;
	GLuint width, height;
	GLuint imgFormat, intFormat;
	Texture2D();
	~Texture2D();
	void process(GLuint width, GLuint height, GLint comp, unsigned char* data);
	void bind() const;
private:
	static void processFormat(GLint& imgFormat, GLint& intFormat, GLint& comp);
};

#endif // !TEXTURE_H

