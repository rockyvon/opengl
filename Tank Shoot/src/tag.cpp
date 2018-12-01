#include "stdafx.h"
#include "tag.h"

map<string, vector<UnicornBase*>> Tag::tags = map<string, vector<UnicornBase*>>();

void Tag::addObject(string tag, UnicornBase * object)
{
	if (tag.empty()) {
		Error::processError("tag is empty");
		return;
	}
		
	tags[tag].push_back(object);
}

void Tag::removeTag(string tag)
{
	tags[tag].clear();
}

vector<UnicornBase*> Tag::getOjectsByTag(string tag)
{
	return tags[tag];
}

UnicornBase * Tag::getObjectByTag(string tag)
{
	vector<UnicornBase*> objs = tags[tag];
	if (objs.size() > 0)
		return objs[0];
	else
		return nullptr;
}
