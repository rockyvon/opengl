#pragma once
#include "sceneconfig.h"
#include "shader.h"
#include "opengl.h"
#include "model.h"
#include "unicorn.h"

class Scene {
public:
	void init();
	void draw(ShaderManager* shader, vec3 s = vec3(1.0f));
private:
	UnicornBase * object;
	string scene_config_path = "config/scene.json";
	SceneConfigLoader scLoader;
	AssimpModelLoader* loader;

	void initNode(UnicornBase* object);
	void drawNode(UnicornBase* object, ShaderManager* shader, vec3 s);
};
