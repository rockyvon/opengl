#include "stdafx.h"
#include "unicorn.h"

UnicornBase::UnicornBase()
{
	transform = new Transform();
	components[Transform::getName()].push_back(transform);
}

UnicornBase::~UnicornBase()
{
}

void UnicornBase::setName(string name)
{
	this->name = name;
}

string UnicornBase::getName()
{
	return name;
}

void UnicornBase::setEnable(bool enabled)
{
	this->enable = enabled;
}

bool UnicornBase::getEnable()
{
	return enable;
}

void UnicornBase::addComponent(Component * component)
{
	component->setObject(this);
	components[component->getName()].push_back(component);
}

void UnicornBase::removeComponent(string name, Component * component)
{
	unsigned int i = 0;
	if (components.find(name) != components.end()) {
		vector<Component*>* coms = &components[name];
		vector<Component*>::iterator it;
		for (it = coms->begin(); it != coms->end();)
		{
			if (*it == component)
				it = coms->erase(it);
		else
			++it; 
		}
	}
	
}

Component * UnicornBase::getComponent(string name)
{
	vector<Component*> coms = getComponents(name);
	if (coms.size() > 0)
		return coms[0];
	else
		return NULL;
}

vector<Component*> UnicornBase::getComponents(string name)
{
	map<string, vector<Component*>>::iterator it;
	it = components.find(name);
	if (it == components.end())
	{
		Error::processError("no Components: " + name + " in " + this->name);
		return vector<Component*>();
	}else
	return it->second;
}

void UnicornBase::addChildren(UnicornBase * object)
{
	children[object->getName()] = object;
}

UnicornBase * UnicornBase::getChild(string name)
{
	return children[name];
}

map<string, UnicornBase*>* UnicornBase::getChildren()
{
	return &children;
}

void UnicornBase::setParent(UnicornBase * parent)
{
	this->parent = parent;
}

UnicornBase * UnicornBase::getParent()
{
	return parent;
}


string Component::name = "";

string Component::getName()
{
	return name;
}


void Component::setObject(UnicornBase * object)
{
	this->object = object;
}

UnicornBase * Component::getObject()
{
	return object;
}

string Component::toString()
{
	return "";
}


Transform::Transform()
{
	name = TRANSFORM_NAME;
}

string Transform::toString()
{
	return "name:" + name +
		"; position:" + to_string(position.x) + "," + to_string(position.y) + "," + to_string(position.z);
}

vec3 Transform::translate(vec3 &pos, vec3 &dir, float dis)
{
	vec3 target;
	vec3 distance = dir * dis;
	target.x = pos.x + dot(distance, vec3(1.0f, 0.0f, 0.0f));
	target.y = pos.y + dot(distance, vec3(0.0f, 1.0f, 0.0f));
	target.z = pos.z + dot(distance, vec3(0.0f, 0.0f, 1.0f));
	return target;
}

void Transform::rotate(Transform * transform, vec3 axis, float angle)
{
}

MeshRender::MeshRender()
{
	name = MESHRENDER_NAME;
}

Script::Script()
{
	name = SCRIPT_NAME;
}
