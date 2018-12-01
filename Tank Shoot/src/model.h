#pragma once

#ifndef MODEL_H
#define MODEL_H

#include "opengl.h"
#include "mesh.h"
#include "config.h"
#include <map>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

using namespace Assimp;

class Model {
public:
	Model();
	void draw(ShaderManager* shader);
	GLboolean hasNormal = true;
	vector<Mesh>* meshes;
private:
};

class ModelLoader
{
public:
	ModelLoader();
	~ModelLoader();

	virtual Model* loadModel(string path);
protected:
	
	GLboolean loaded = false;
	static map<string, Model*> loadedModels;
	string modelPath;
private:
	void init();
};

class AssimpModelLoader :public ModelLoader {
public:
	virtual Model* loadModel(string path);
private:
	void init();
	GLuint * textureIds;
	map<string, GLuint*> textureMap;
	string getBasePath(string path);
	void processAllTextures(const aiScene* scene, bool gamma = false);
	Material processMaterial(aiMaterial* aimaterial);
	void processNode(const aiNode* node, const aiScene* scene, Model* model);
	void arrayToVec4(vec4 &v, float *array);
	void aiColor4ToVec4(vec4 &v, aiColor4D &aicolor);
};

#endif // !MODEL_H
