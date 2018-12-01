#include "stdafx.h"
#include "resource.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

ResourceLoader::ResourceLoader()
{
}

ResourceLoader::~ResourceLoader()
{
}

unsigned char * ResourceLoader::loadImage(string path, int & width, int & height, int & comp)
{
	unsigned char *data = stbi_load(path.c_str(), &width, &height, &comp, 0);
	return data;
}

map<string, Texture2D*> TextureMng::textures;

Texture2D * TextureMng::getTexture(string name)
{
	map<string, Texture2D*>::iterator iter = textures.find(name);
	if (iter != textures.end())
		return textures[name];
	else
		return nullptr;
}

void TextureMng::addTexture(string name, Texture2D * texture)
{
	map<string, Texture2D*>::iterator iter = textures.find(name);
	if (iter != textures.end()) 
		cout << "添加失败：Texture2D管理器中已存在key为" << name << "的元素！\n";
	else
		textures[name] = texture;
}

Texture2D * TextureMng::loadTexture(string path)
{
	int width, height, comp;
	unsigned char* data = ResourceLoader::loadImage(path, width, height, comp);
	if (!data) {
		cout << "不存在图片：" << path << endl;
		return nullptr;
	}
	else {
		Texture2D* texture = new Texture2D();
		texture->process(width, height, comp, data);
		return texture;
	}
}
