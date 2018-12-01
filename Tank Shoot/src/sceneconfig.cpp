#include "stdafx.h"
#include "sceneconfig.h"

void SceneConfigLoader::loadSceneData(string path, UnicornBase** object)
{
	string data = loadJson(path);
	processData(data, object);
}

string SceneConfigLoader::loadJson(string path)
{
	ifstream infile;
	infile.open(path.data());   //将文件流对象与文件连接起来 
	assert(infile.is_open());   //若失败,则输出错误消息,并终止程序运行 

	string result;
	string s;
	while (getline(infile, s))
	{
		result += s;
	}
	infile.close();
	return result;
}

void SceneConfigLoader::processData(string data, UnicornBase** object)
{
	cJSON * base = NULL, *root = NULL;
	base = cJSON_Parse(data.data());
	if (!base)
	{
		printf("Error before: [%s]\n", cJSON_GetErrorPtr());
	}
	root = cJSON_GetObjectItem(base, "root");

	*object = new UnicornBase();
	processNode(*object, root);
	delete base;
	delete root;
}

void SceneConfigLoader::processNode(UnicornBase* object, cJSON * node)
{
	unsigned int i = 0;

	cJSON* jname = cJSON_GetObjectItem(node, "name");
	cJSON* jposition = cJSON_GetObjectItem(node, "position");
	cJSON* jrotation = cJSON_GetObjectItem(node, "rotation");
	cJSON* jscale = cJSON_GetObjectItem(node, "scale");
	object->transform->position = vec3(
		cJSON_GetObjectItem(jposition, "x")->valueint,
		cJSON_GetObjectItem(jposition, "y")->valueint,
		cJSON_GetObjectItem(jposition, "z")->valueint
	);
	object->transform->rotation = vec3(
		cJSON_GetObjectItem(jrotation, "x")->valueint,
		cJSON_GetObjectItem(jrotation, "y")->valueint,
		cJSON_GetObjectItem(jrotation, "z")->valueint
	);
	object->transform->scale = vec3(
		cJSON_GetObjectItem(jscale, "x")->valueint,
		cJSON_GetObjectItem(jscale, "y")->valueint,
		cJSON_GetObjectItem(jscale, "z")->valueint
	);
	if(jname->valuestring)
		object->setName(jname->valuestring);

	cJSON* jpath = cJSON_GetObjectItem(node, "path");
	if (jpath->valuestring)//如果path！=null
	{
		//向object加入meshRender
		MeshRender* render = new MeshRender();
		object->addComponent(render);
		render->path = jpath->valuestring;
	}

	UnicornBase* parent = object->getParent();
	if (parent) {
		parent->addChildren(object);
	}

	cJSON* children = cJSON_GetObjectItem(node, "children");
	unsigned int size = cJSON_GetArraySize(children);
	if (size > 0) //如果存在子物体
	{
		//遍历自物体
		for (i = 0; i < size; i++)
		{
			cJSON * cchild = cJSON_GetArrayItem(children, i);
			cJSON* jtag = cJSON_GetObjectItem(cchild, "tag");
			UnicornBase* child = NULL;
			if (jtag && jtag->valuestring) {
				string tag = jtag->valuestring;

				child = new UnicornBase();
				
				Tag::addObject(tag, child);

			}


			if(!child)
				child = new UnicornBase();

			child->setParent(object);
			
			processNode(child, cchild);
		}
			
	}
}
