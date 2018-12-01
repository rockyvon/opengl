#pragma once

#ifndef SCENECONFIG_H
#define SCENECONFIG_H

#include "stdafx.h"
#include "opengl.h"
#include "unicorn.h"
#include "tag.h"
#include "cJSON.h"
#include <fstream>
#include <cassert>

/*
struct ModelData
{
public:
	string name;
	string tag;
	string path;
	vec3 position;
	vec3 rotation;
	vec3 scale;
};*/

/*struct SceneData
{
public:
	string name;
	UnicornBase root;
	SceneData()
	{
		root.name = "root";
		root.position = vec3(0.0f);
		root.rotation = vec3(0.0f);
		root.scale = vec3(1.0f);
	}
};*/

class SceneConfigLoader
{
public:
	void loadSceneData(string path, UnicornBase** object);
private:
	string loadJson(string path);
	void processData(string data, UnicornBase** object);
	void processNode(UnicornBase* object, cJSON* node);
};

#endif // !SCENECONFIG_H