#ifndef UNICORN_H
#define UNICORN_H

#include "stdafx.h"
#include "model.h"
#include "coordinate.h"
#include "error.h"

static string TRANSFORM_NAME = "Transform";
static string MESHRENDER_NAME = "MeshRender";
static string SCRIPT_NAME = "Script";
static string CAMERA_NAME = "Camera";

class UnicornBase;

class Component {
public:
	static string getName();
	void setObject(UnicornBase* object);
	UnicornBase* getObject();
	virtual string toString();
protected:
	static string name;
	UnicornBase* object;
private:
};

class Transform : public Component {
public:
	Transform();
	virtual string toString();
	vec3 position = vec3(0.0f);
	vec3 rotation = vec3(0.0f);
	vec3 scale = vec3(1.0f);
	Coordinate3D localCoords;
	static vec3 translate(vec3 &pos, vec3 &dir, float dis);
	static void rotate(Transform* transform, vec3 axis, float angle);
};

class MeshRender : public Component {
public:
	MeshRender();
	string path;
	Model* model;
};

class Script : public Component {
public:
	Script();
	virtual void update() = 0;
};

class UnicornBase {
public:
	Transform * transform;
	UnicornBase();
	~UnicornBase();
	void setName(string name);
	string getName();
	void setEnable(bool enabled);
	bool getEnable();

	void addComponent(Component* component);
	void removeComponent(string name, Component* component);
	Component* getComponent(string name);
	vector<Component*> getComponents(string name);
	void addChildren(UnicornBase* object);
	UnicornBase* getChild(string name);
	map<string, UnicornBase*>* getChildren();
	void setParent(UnicornBase* parent);
	UnicornBase* getParent();

private:
protected:
	map<string, vector<Component*>> components;
	map<string, UnicornBase*> children;
	UnicornBase* parent;
	bool enable = true;
	string name;
};

#endif // !UNICORN_H