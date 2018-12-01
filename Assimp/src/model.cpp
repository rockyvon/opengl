#include "stdafx.h"
#include "model.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

ModelLoader::ModelLoader()
{
}

ModelLoader::~ModelLoader()
{
}

void ModelLoader::draw(ShaderManager * shader)
{
	shader->setUniform1i("hasNormal", hasNormal);
	for (unsigned int i = 0; i < meshes.size(); i++)
	{
		meshes[i].draw(shader);
	}
}

void AssimpModel::loadScene(string path)
{
	modelPath = path;
	Importer importer;
	const aiScene* scene = importer.ReadFile(path.c_str(), aiProcessPreset_TargetRealtime_Quality);
	if (scene) {
		processAllTextures(scene);
		processNode(scene->mRootNode, scene);
	}
	else {
		cout << importer.GetErrorString() << endl;
	}
}

string AssimpModel::getBasePath(string path)
{
	size_t pos = path.find_last_of("\\/");
	return (std::string::npos == pos) ? "" : path.substr(0, pos + 1);
}

void AssimpModel::processAllTextures(const aiScene * scene, bool gamma)
{
	unsigned int i = 0, j = 0;
	/*if (!scene->HasTextures()) {
		return;
	}*/
	for (i = 0; i < scene->mNumMaterials; i++) {
		int texIndex = 0;

		aiString path;	// filename

		while (scene->mMaterials[i]->GetTexture(aiTextureType_DIFFUSE, texIndex, &path) == AI_SUCCESS)
		{
			textureMap[path.data] = NULL; //fill map with textures, pointers still NULL yet
			texIndex++;
		}
		texIndex = 0;
		while (scene->mMaterials[i]->GetTexture(aiTextureType_SPECULAR, texIndex, &path) == AI_SUCCESS)
		{
			textureMap[path.data] = NULL; //fill map with textures, pointers still NULL yet
			texIndex++;
		}
	}

	int numTextures = textureMap.size();

	textureIds = new GLuint[numTextures];
	glGenTextures(numTextures, textureIds);

	std::map<std::string, GLuint*>::iterator itr = textureMap.begin();

	std::string basepath = getBasePath(modelPath);
	for (int i = 0; i<numTextures; i++)
	{

		std::string fileLocation = basepath + (*itr).first;  // get filename
		(*itr).second = &textureIds[i];	  // save texture id for filename in map
		itr++;								  // next texture
		int width, height, nrComponents;
		unsigned char *data = stbi_load(fileLocation.c_str(), &width, &height, &nrComponents, 0);
		if (data)
		{
			GLenum format_img;
			GLenum format_buffer;
			if (nrComponents == 1) {
				format_img = GL_RED;
				format_buffer = GL_RED;
			}


			else if (nrComponents == 3) {
				format_img = gamma ? GL_SRGB : GL_RGB;
				format_buffer = GL_RGB;
			}


			else if (nrComponents == 4) {
				format_img = gamma ? GL_SRGB_ALPHA : GL_RGBA;
				format_buffer = GL_RGBA;
			}

			glBindTexture(GL_TEXTURE_2D, textureIds[i]);
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
			std::cout << "Texture failed to load at path: " << fileLocation << std::endl;
			stbi_image_free(data);
		}
	}
}

Material AssimpModel::processMaterial(aiMaterial * aimaterial)
{
	int ret1, ret2;
	aiColor4D diffuse;
	aiColor4D specular;
	aiColor4D ambient;
	aiColor4D emission;
	ai_real shininess, strength;
	int twoSided;
	int wireframe;
	unsigned int max;

	Material material;

	unsigned int texIndex = 0;
	aiString texPath;
	if (AI_SUCCESS == aimaterial->GetTexture(aiTextureType_DIFFUSE, texIndex, &texPath))
	{
		//bind texture
		unsigned int texId = *textureMap[texPath.data];
		glBindTexture(GL_TEXTURE_2D, texId);
		material.difTexId = texId;

		material.hasTexture = true;
	}

	if (AI_SUCCESS == aimaterial->GetTexture(aiTextureType_SPECULAR, texIndex, &texPath))
	{
		//bind texture
		unsigned int texId = *textureMap[texPath.data];
		glBindTexture(GL_TEXTURE_2D, texId);
		material.speTexId = texId;

		material.hasTexture = true;
	}

	vec4 color(0.0f);

	arrayToVec4(color, DEFAULT_EMISSION);
	if (AI_SUCCESS == aiGetMaterialColor(aimaterial, AI_MATKEY_COLOR_EMISSIVE, &emission))
		aiColor4ToVec4(color, emission);
	material.emission = color;

	arrayToVec4(color, DEFAULT_AMBIENT);
	if (AI_SUCCESS == aiGetMaterialColor(aimaterial, AI_MATKEY_COLOR_AMBIENT, &ambient))
		aiColor4ToVec4(color, ambient);
	material.ambient = color;

	arrayToVec4(color, DEFAULT_DIFFUSE);
	if (AI_SUCCESS == aiGetMaterialColor(aimaterial, AI_MATKEY_COLOR_DIFFUSE, &diffuse))
		aiColor4ToVec4(color, diffuse);
	material.diffuse = color;

	arrayToVec4(color, DEFAULT_SPECULAR);
	if (AI_SUCCESS == aiGetMaterialColor(aimaterial, AI_MATKEY_COLOR_SPECULAR, &specular))
		aiColor4ToVec4(color, specular);
	material.specular = color;

	max = 1;
	ret1 = aiGetMaterialFloatArray(aimaterial, AI_MATKEY_SHININESS, &shininess, &max);
	max = 1;
	ret2 = aiGetMaterialFloatArray(aimaterial, AI_MATKEY_SHININESS_STRENGTH, &strength, &max);
	if ((ret1 == AI_SUCCESS) && (ret2 == AI_SUCCESS))
		material.shininess = shininess * strength;
	else {
		material.shininess = DEFAULT_SHININESS;
		arrayToVec4(color, DEFAULT_SPECULAR);
		material.specular = color;
	}

	max = 1;
	if (AI_SUCCESS == aiGetMaterialIntegerArray(aimaterial, AI_MATKEY_ENABLE_WIREFRAME, &wireframe, &max))
		material.wireframe = wireframe;
	else
		material.wireframe = false;

	max = 1;
	if ((AI_SUCCESS == aiGetMaterialIntegerArray(aimaterial, AI_MATKEY_TWOSIDED, &twoSided, &max)) && twoSided)
		material.cullFace = twoSided;
	else
		material.cullFace = false;

	return material;
}

void AssimpModel::processNode(const aiNode* ainode, const aiScene* scene)
{
	unsigned int i = 0, j = 0, k = 0;
	
	for (i = 0; i < ainode->mNumMeshes; i++) {
		aiMesh* aimesh = scene->mMeshes[ainode->mMeshes[i]];
		vector<Vertex> vertices;
		vector<Face> faces;
		GLboolean hasColor = false;

		if (aimesh->mNormals == NULL)
			hasNormal = false;
		
		if (aimesh->mColors[0] == NULL)
			hasColor = false;
		else {
			hasColor = true;
		}

		Material material = processMaterial(scene->mMaterials[aimesh->mMaterialIndex]);
		material.hasColor = hasColor;

		for (j = 0; j < aimesh->mNumVertices; j++) {
			Vertex vertex;
			vertex.position.x = aimesh->mVertices[j].x;
			vertex.position.y = aimesh->mVertices[j].y;
			vertex.position.z = aimesh->mVertices[j].z;
			if (hasNormal) {
				vertex.normal.x = aimesh->mNormals[j].x;
				vertex.normal.y = aimesh->mNormals[j].y;
				vertex.normal.z = aimesh->mNormals[j].z;
			}
			else {
				vertex.normal.x = 0.0f;
				vertex.normal.y = 0.0f;
				vertex.normal.z = 0.0f;
			}
			if (hasColor) {
				vertex.color.r = aimesh->mColors[0][j].r;
				vertex.color.g = aimesh->mColors[0][j].g;
				vertex.color.b = aimesh->mColors[0][j].b;
				vertex.color.a = aimesh->mColors[0][j].a;
			}
			if (aimesh->HasTextureCoords(0)) {
				vertex.texCoords.x = aimesh->mTextureCoords[0][j].x;
				vertex.texCoords.y = 1 - aimesh->mTextureCoords[0][j].y;//ÎÆÀítÖá·­×ª
			}
			bool fafa = aimesh->HasTextureCoords(0);
			vertices.push_back(vertex);
		}

		for (j = 0; j < aimesh->mNumFaces; j++) {
			aiFace* aiface = &aimesh->mFaces[j];
			Face face;
			switch (aiface->mNumIndices)
			{
				case 1: face.type = GL_POINTS; break;
				case 2: face.type = GL_LINES; break;
				case 3: face.type = GL_TRIANGLES; break;
				default: face.type = GL_POLYGON; break;
			}

			for (k = 0; k < aiface->mNumIndices; k++) {
				int index = aiface->mIndices[k];
				face.indices.push_back(index);
			}

			faces.push_back(face);
		}

		meshes.push_back(Mesh(vertices, faces, material));
	}

	for (i = 0; i < ainode->mNumChildren; i++) {
		processNode(ainode->mChildren[i], scene);
	}
}

void AssimpModel::arrayToVec4(vec4 & v, float * array)
{
	v.x = array[0];
	v.y = array[1];
	v.z = array[2];
	v.w = array[3];
}

void AssimpModel::aiColor4ToVec4(vec4 & v, aiColor4D & aicolor)
{
	v.x = aicolor.r;
	v.y = aicolor.g;
	v.z = aicolor.b;
	v.w = aicolor.a;
}
