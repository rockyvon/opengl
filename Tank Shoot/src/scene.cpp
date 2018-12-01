#include "stdafx.h"
#include "scene.h"

void Scene::init()
{
	SceneConfigLoader scLoader;
	scLoader.loadSceneData(scene_config_path, &object);
	loader = new AssimpModelLoader();
	initNode(object);
	delete loader;
	loader = NULL;
}

void Scene::draw(ShaderManager * shader, vec3 s)
{
	drawNode(object, shader, s);

}

void Scene::initNode(UnicornBase * object)
{
	Component* mrc = object->getComponent(MESHRENDER_NAME);
	if(mrc){
		MeshRender* render = (MeshRender*)mrc;
		render->model = loader->loadModel(render->path);
	}

	map<string, UnicornBase*>* children = object->getChildren();
	map<string, UnicornBase*>::iterator it;
	it = children->begin();
	while (it != children->end())
	{
		initNode(it->second);
		it++;
	}
}

void Scene::drawNode(UnicornBase * object, ShaderManager * shader, vec3 s)
{

	vector<Component*> scripts = (object->getComponents(SCRIPT_NAME));
	if (scripts.size() > 0) {
		unsigned int i = 0;
		for (i = 0; i < scripts.size(); i++) {
			((Script*)scripts[i])->update();
		}
	}
	Component* mrc = object->getComponent(MESHRENDER_NAME);
	if (mrc) {
		Transform* transform = object->transform;
		MeshRender* render = (MeshRender*)mrc;
		mat4 localModel = mat4(1.0f);

		localModel = translate(localModel, transform->position);
		localModel = rotate(localModel, radians(transform->rotation.x), vec3(1.0f, 0.0f, 0.0f)); 
		localModel = rotate(localModel, radians(transform->rotation.y), vec3(0.0f, 1.0f, 0.0f));
		localModel = rotate(localModel, radians(transform->rotation.z), vec3(0.0f, 0.0f, 1.0f));
		localModel = scale(localModel, transform->scale);
		localModel = scale(localModel, s);

		shader->setUniformMatrix4fv("model", 1, value_ptr(localModel));
		render->model->draw(shader);
	}

	map<string, UnicornBase*>* children = object->getChildren();
	map<string, UnicornBase*>::iterator it;
	it = children->begin();
	while (it != children->end())
	{
		drawNode(it->second, shader, s);
		it++;
	}
}
