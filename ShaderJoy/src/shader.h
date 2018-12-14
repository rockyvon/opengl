#pragma once

#include "stdafx.h"
#include "opengl.h"

class ShaderManager
{
public:
	~ShaderManager();
	void load(const char* vertex_path, const char* geometry_path, const char* fragment_path);
	void load(std::string& vertex_path, std::string& geometry_path, std::string& fragment_path);
	void use();
	void disable();
	static void checkGLError();
	void setUniform1i(const GLchar * name, GLint value);
	void setUniform1i(std::string name, GLint value);
	void setUniform1f(const GLchar * name, GLfloat value);
	void setUniform1f(std::string name, GLfloat value);
	void setUniform3fv(const GLchar * name, GLint count, GLfloat * values);
	void setUniform3fv(std::string name, GLint count, GLfloat * values);
	void setUniform4fv(const GLchar * name, GLint count, GLfloat * values);
	void setUniform4fv(std::string name, GLint count, GLfloat * values);
	void setUniformMatrix4fv(const GLchar * name, GLint count, GLfloat * values);
	void setUniformMatrix4fv(std::string name, GLint count, GLfloat * values);

private:
	std::string loadFile(const char* path);
	GLuint vertex_shader;
	GLuint geometry_shader;
	GLuint fragment_shader;
	GLint program;
	static const int BUFFER_SIZE = 512;
};

