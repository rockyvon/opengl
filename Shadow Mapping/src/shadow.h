#pragma once
#include "shader.h"

class Shadow {
public:
	Shadow();
	~Shadow();
	void draw(vec3 position, vec3 direction, mat4& lsm, void(*callback)(ShaderManager*));
protected:
	virtual void init() = 0;
	GLuint fbo, texId;
	GLint shadowSize[2] = { 2048, 2048 };
	ShaderManager *shader;
};

class DShadow : public Shadow
{
public:
	DShadow();
	~DShadow();
	
protected:
	virtual void init();
private:
	
	char shader_shdow_path[3][19] = {
		"shader/shadow.vert",
		"",
		"shader/shadow.frag"
	};
};

class LShadow : public Shadow
{
public:
	LShadow();
	~LShadow();

protected:
	virtual void init();
private:

	char shader_shdow_path[3][19] = {
		"shader/shadow.vert",
		"",
		"shader/shadow.frag"
	};
};

