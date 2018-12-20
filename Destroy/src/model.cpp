#include "stdafx.h"
#include "model.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

ModelLoader* ModelManager::createLoader(ModelType type)
{
	return new AssimpModel();
}

void ModelLoader::load_model(const string path, bool gamma)
{
	load_model(path.c_str(), gamma);
}

void ModelLoader::draw(ShaderManager * shader)
{
	for (unsigned int i = 0; i < meshes.size(); i++) {
		meshes[i].draw(shader);
	}
}

bool AssimpModel::init()
{
	return true;
}

void AssimpModel::load_model(const char * path, bool gamma)
{
	Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_FlipUVs);
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		cout << "ERROR::ASSIMP::" << importer.GetErrorString() << endl;
		return;
	}

	string path_string = path;
	directory = path_string.substr(0, path_string.find_last_of('/'));
	processNode(scene->mRootNode, scene, gamma);
}

void AssimpModel::load_model(const string path, bool gamma)
{
	ModelLoader::load_model(path);
}

void AssimpModel::draw(ShaderManager * shader)
{
	ModelLoader::draw(shader);
}

int AssimpModel::TextureLoaded(string tex_name)
{
	for (unsigned int i = 0; i < textures_loaded.size(); i++)
	{
		if (textures_loaded[i].path == tex_name) {
			return i;
		}
	}
	return -1;
}

void AssimpModel::processNode(aiNode * node, const aiScene * scene, bool gamma)
{
	for (unsigned int i = 0; i < node->mNumMeshes; i++) {
		Mesh mesh = processMesh(scene->mMeshes[node->mMeshes[i]], scene, gamma);
		mesh.init();
		meshes.push_back(mesh);
	}

	for (unsigned int i = 0; i < node->mNumChildren; i++) {
		processNode(node->mChildren[i], scene, gamma);
	}
}

Mesh AssimpModel::processMesh(aiMesh * mesh, const aiScene * scene, bool gamma)
{
	vector<Vertex> vertices;
	vector<GLuint> indices;
	vector<Texture> textures;

	//加载顶点
	for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
		Vertex vertex;
		vertex.position.x = mesh->mVertices[i].x;
		vertex.position.y = mesh->mVertices[i].y;
		vertex.position.z = mesh->mVertices[i].z;
		if (mesh->HasNormals()) {
			vertex.normal.x = mesh->mNormals[i].x;
			vertex.normal.y = mesh->mNormals[i].y;
			vertex.normal.z = mesh->mNormals[i].z;
		}
		if (mesh->mTextureCoords[0]) {
			vertex.texCoords.x = mesh->mTextureCoords[0][i].x;
			vertex.texCoords.y = mesh->mTextureCoords[0][i].y;
		}
		else
		{
			vertex.texCoords.x = 0.0f;
			vertex.texCoords.y = 0.0f;
		}
		if (mesh->HasTangentsAndBitangents())
		{
			vertex.tangent.x = mesh->mTangents[i].x;
			vertex.tangent.y = mesh->mTangents[i].y;
			vertex.tangent.z = mesh->mTangents[i].z;
			vertex.bitangent.x = mesh->mBitangents[i].x;
			vertex.bitangent.y = mesh->mBitangents[i].y;
			vertex.bitangent.z = mesh->mBitangents[i].z;
		}
		else {
			vertex.tangent.x = 0.0f;
			vertex.tangent.y = 0.0f;
			vertex.tangent.z = 0.0f;
			vertex.bitangent.x = 0.0f;
			vertex.bitangent.y = 0.0f;
			vertex.bitangent.z = 0.0f;
		}

		vertices.push_back(vertex);
	}

	//加载索引
	for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++) {
			indices.push_back(face.mIndices[j]);
		}
	}

	//加载材质
	if (mesh->mMaterialIndex >= 0) {
		aiMaterial * material = scene->mMaterials[mesh->mMaterialIndex];
		vector<Texture> diffuseMaps = loadMaterialTextures(material,
			aiTextureType_DIFFUSE, DIFFUSE, gamma);
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
		vector<Texture> specularMaps = loadMaterialTextures(material,
			aiTextureType_SPECULAR, SPECULAR, gamma);
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
	}

	return Mesh(vertices, indices, textures);
}

unsigned int AssimpModel::TextureFromFile(const char *path, const string &directory, bool gamma)
{
	string filename = string(path);
	filename = directory + '/' + filename;

	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char *data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format_img;
		GLenum format_buffer;
		if (nrComponents == 1) {
			format_img = GL_RED;
			format_buffer = GL_RED;
		}
			

		else if (nrComponents == 3) {
			format_img = gamma ? GL_SRGB:GL_RGB;
			format_buffer = GL_RGB;
		}
			

		else if (nrComponents == 4) {
			format_img = gamma ? GL_SRGB_ALPHA : GL_RGBA;
			format_buffer = GL_RGBA;
		}

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format_img, width, height, 0, format_buffer, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}

	return textureID;
}

vector<Texture> AssimpModel::loadMaterialTextures(aiMaterial * mat, aiTextureType aiType, LIGHT_PHONG_MODEL type, bool gamma)
{
	vector<Texture> textures;


	for (unsigned int i = 0; i < mat->GetTextureCount(aiType); i++)
	{
		aiString str;
		mat->GetTexture(aiType, i, &str);
		int index = TextureLoaded(str.C_Str());
		if (index == -1) //未从文件中加载纹理
		{
			Texture texture;
			texture.id = TextureFromFile(str.C_Str(), directory, gamma);
			texture.type = type;
			texture.path = str.C_Str();
			textures.push_back(texture);
			textures_loaded.push_back(texture);
		}
		else {
			textures.push_back(textures_loaded[index]);
		}

	}
	return textures;
}


