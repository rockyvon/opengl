#pragma once
#include "opengl.h"
#include "shader.h"

struct Vertex {
	vec3 position;
	vec4 color;
	vec3 normal;
	vec2 texCoords;
};


struct Face {
	vector<GLuint> indices;
	GLenum type;
};

struct Material {
	GLuint difTexId;
	GLuint speTexId;
	vec4 emission;
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
	GLfloat shininess;
	GLboolean hasColor = false;
	GLboolean hasTexture = false;
	GLboolean wireframe = false;
	GLboolean cullFace = false;
};

class Mesh
{
public:
	Mesh(vector<Vertex> vertices, vector<Face> faces, Material material);
	~Mesh();
	void init();
	void draw(ShaderManager* shader);
private:
	GLuint vbo;
	vector<GLuint> vaos;
	vector<GLuint> ibos;
	vector<Vertex> vertices;
	vector<Face> faces;
	Material material;
};

