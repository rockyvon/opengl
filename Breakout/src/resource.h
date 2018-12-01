#ifndef RESOURCE_H
#define RESOURCE_H

#include "texture.h"

class ResourceLoader
{
public:
	ResourceLoader();
	~ResourceLoader();

	static unsigned char* loadImage(string path, int& width, int& height, int& comp);
};

class TextureMng {
public:
	static Texture2D* getTexture(string name);
	static void addTexture(string name, Texture2D* texture);
	static Texture2D* loadTexture(string path);
private:
	static map<string, Texture2D*> textures;
};

#endif // !RESOURCE_H
