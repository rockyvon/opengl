#include "stdafx.h"
#include "skybox.h"

#include "stb_image.h"

/*#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#endif // !STB_IMAGE_IMPLEMENTATION*/

void Skybox::init(vector<string>& faces)
{
	initVertex();
	loadCubeTexture(faces);
}

void Skybox::draw(ShaderManager * shader)
{
	
	glDepthFunc(GL_LEQUAL);
	glBindVertexArray(vao);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texId);
	glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_INT, (void *)(0 * sizeof(GLuint)));
	glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_INT, (void *)(4 * sizeof(GLuint)));
	glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_INT, (void *)(8 * sizeof(GLuint)));
	glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_INT, (void *)(12 * sizeof(GLuint)));
	glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_INT, (void *)(16 * sizeof(GLuint)));
	glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_INT, (void *)(20 * sizeof(GLuint)));
	glDepthFunc(GL_LESS);
	
}

void Skybox::initVertex()
{
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ibo);

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW
	);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(0);
}

void Skybox::loadCubeTexture(vector<string> &faces)
{
	glGenTextures(1, &texId);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texId);

	int width, height, nrChannels;
	for (unsigned int i = 0; i < faces.size(); i++)
	{
		unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
				0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
			);
			stbi_image_free(data);
		}
		else
		{
			std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
			stbi_image_free(data);
		}
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

}
