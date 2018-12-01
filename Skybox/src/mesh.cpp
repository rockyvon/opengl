#include "stdafx.h"
#include "mesh.h"

Mesh::Mesh(vector<Vertex> vertices, vector<Face> faces, Material material)
{
	this->vertices = vertices;
	this->faces = faces;
	this->material = material;
	init();
}

Mesh::~Mesh()
{
}

void Mesh::init()
{
	unsigned int i = 0;
	for (i = 0; i < faces.size(); i++)
	{
		vaos.push_back(0);
		ibos.push_back(0);
	}
		
	glGenVertexArrays(vaos.size(), &vaos[0]);
	glGenBuffers(1, &vbo);
	glGenBuffers(ibos.size(), &ibos[0]);


	for (i = 0; i < ibos.size(); i++) {
		glBindVertexArray(vaos[i]);

		
		if (i == 0) {
			glBindBuffer(GL_ARRAY_BUFFER, vbo);
			glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);
		}
			

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibos[i]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, faces[i].indices.size() * sizeof(GL_UNSIGNED_INT), &(faces[i].indices[0]), GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, position));
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, color));
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, normal));
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, texCoords));
	}

	
}

void Mesh::draw(ShaderManager * shader)
{
	unsigned int i = 0;

	glPolygonMode(GL_FRONT_AND_BACK, material.wireframe ? GL_LINE : GL_FILL);
	if (material.cullFace)
		glEnable(GL_CULL_FACE);
	else
		glDisable(GL_CULL_FACE);
	shader->setUniform4fv("material.emission", 1, value_ptr(material.emission));
	shader->setUniform4fv("material.ambient", 1, value_ptr(material.ambient));
	shader->setUniform4fv("material.diffuse", 1, value_ptr(material.diffuse));
	shader->setUniform4fv("material.specular", 1, value_ptr(material.specular));
	shader->setUniform1f("material.shininess", material.shininess);
	shader->setUniform1i("material.hasColor", material.hasColor);
	shader->setUniform1i("material.hasTexture", material.hasTexture);
	
	glActiveTexture(GL_TEXTURE0);
	shader->setUniform1i(("material.difTex"), 0);
	glBindTexture(GL_TEXTURE_2D, material.difTexId);

	glActiveTexture(GL_TEXTURE1);
	shader->setUniform1i(("material.speTex"), 1);
	glBindTexture(GL_TEXTURE_2D, material.speTexId);

	//根据face数量循环绘制
	for (i = 0; i < ibos.size(); i++) {
		glBindVertexArray(vaos[i]);
		glDrawElements(faces[i].type, faces[i].indices.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}
	
}
