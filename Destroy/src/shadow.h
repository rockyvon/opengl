#ifndef SHADOW_H
#define SHADOW_H

#include "opengl.h"

class Shadow {
public:
	
	void init(LIGHT_TYPE light_type, vec3 eye, vec3 center = vec3(0.0f), vec3 up = vec3(0.0f, 1.0f, 0.0f));
	void bakeMap(ShaderManager * shader);
	void renderShadow();
	void endOffscreen();
	void bindShadowTexture(ShaderManager * shader);
	mat4 *getView();
	mat4 *getProjection();
	GLint* getResolution();
	GLfloat* getSize();
private:
	GLuint fbo, depth_texture;
	LIGHT_TYPE light_type;
	vec3 eye, center, up;
	mat4 view, projection;
	GLint shadow_map_resolution[2] = { 2048, 2048 };
	GLfloat shadow_size[2] = { 20.0f, 20.0f };
};

#endif