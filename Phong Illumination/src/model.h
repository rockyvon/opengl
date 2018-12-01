#pragma once
#include "opengl.h"
#include "mesh.h"
#include "config.h"
#include <map>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

using namespace Assimp;

class ModelLoader
{
public:
	ModelLoader();
	~ModelLoader();

	virtual void loadScene(string path) = 0;
	void draw(ShaderManager* shader);
protected:
	GLboolean hasNormal = true;
	vector<Mesh> meshes;
private:
	
};

class AssimpModel :public ModelLoader {
public:
	virtual void loadScene(string path);
private:
	GLuint * textureIds;
	string modelPath;
	map<string, GLuint*> textureMap;
	string getBasePath(string path);
	void processAllTextures(const aiScene* scene, bool gamma = false);
	Material processMaterial(aiMaterial* aimaterial);
	void processNode(const aiNode* node, const aiScene* scene);
	void arrayToVec4(vec4 &v, float *array);
	void aiColor4ToVec4(vec4 &v, aiColor4D &aicolor);
};
