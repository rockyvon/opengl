#ifndef MAINCFG_H
#define MAINCFG_H

#include "opengl.h"
#include "shader.h"
#include "log.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

float vertices[] = {
	// Œª÷√     // Œ∆¿Ì
	0.0f, 0.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 1.0f, 0.0f,
	1.0f, 1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f, 1.0f,
	0.0f, 1.0f, 0.0f, 1.0f,
	0.0f, 0.0f, 0.0f, 0.0f,
};

GLuint vao, tex1, tex2, tex3;

bool rendering = true;

int win_width = 800, win_height = 600;
vec3 iResolution = vec3(1.0f);
vec4 iMouse = vec4(1.0f);
GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;
GLuint curFrame = 0;
mat4 projection;

std::string shader_scene1_path[] = {"shader/backpaint.vert", "", "shader/raymathing1.frag"};
std::string shader_scene2_path[] = { "shader/backpaint.vert", "", "shader/seascape1.frag" };
std::string shader_scene3_path[] = { "shader/backpaint.vert", "", "shader/snail.frag" };
std::string shader_scene4_path[] = { "shader/backpaint.vert", "", "shader/cloud1.frag" };

std::string img1_path = "texture/snail1.png";
std::string img2_path = "texture/snail2.png";
std::string img3_path = "texture/snail3.png";

std::string* shader_path = shader_scene4_path;

ShaderManager shader;

#endif // !MAINCFG_H

