#ifndef UNICORN_H
#define UNICORN_H

#include "stdafx.h"

static string TRANSFORM_NAME = "Transform";
static string MESHRENDER_NAME = "MeshRender";
static string SCRIPT_NAME = "Script";
static string CAMERA_NAME = "Camera";

class UnicornBase {
public:
	void setName(string name);
	string getName();
	void setEnable(bool enabled);
	bool getEnable();

private:
protected:
	bool enable = true;
	string name;
};

#endif // !UNICORN_H