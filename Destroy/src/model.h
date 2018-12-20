#pragma once

#include "mesh.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

using namespace Assimp;

 enum ModelType
{
	MODEL_OBJ = 0,
	MODEL_FBX = 1
};

class ModelLoader {
public:
	virtual bool init() = 0;
	virtual void load_model(const char* path, bool gamma = false) = 0;
	virtual void load_model(const string path, bool gamma = false);
	virtual void draw(ShaderManager * shader);
protected:
	vector<Mesh> meshes;
	vector<Texture> textures_loaded;
};


class AssimpModel : public ModelLoader {
public:
	bool init();
	void load_model(const char* path, bool gamma = false);
	void load_model(const string path, bool gamma = false);
	void draw(ShaderManager * shader);
private:
	string directory;
	int TextureLoaded(string tex_name);
	void processNode(aiNode * node, const aiScene * scene, bool gamma);
	Mesh processMesh(aiMesh * mesh, const aiScene * scene, bool gamma);
	unsigned int TextureFromFile(const char *path, const string &directory, bool gamma);
	vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType aiType,
		LIGHT_PHONG_MODEL type, bool gamma);
};

class ModelManager {
public:
	static ModelLoader* createLoader(ModelType type);
};