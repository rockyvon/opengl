#pragma once

#include "opengl.h"
#include "shader.h"

struct Vertex
{
	vec3 position;
	vec3 normal;
	vec4 color;
	vec2 texCoords;
	vec3 tangent;
	vec3 bitangent;
};

struct Texture {
	int id;
	int type;
	string path;
};

class Mesh {
public:

	Mesh(vector<Vertex> vertices, vector<GLuint> indices, vector<Texture> textures);
	void init();
	void draw(ShaderManager * shader);
private:
	GLuint vao, vbo, ebo;
	vector<Vertex> vertices;
	vector<GLuint> indices;
	vector<Texture> textures;
};
