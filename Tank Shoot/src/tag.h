#pragma once
#include "stdafx.h"
#include "unicorn.h"
#include "error.h"

class Tag {
public:
	static void addObject(string tag, UnicornBase* object);
	static void removeTag(string tag);
	static vector<UnicornBase*> getOjectsByTag(string tag);
	static UnicornBase* getObjectByTag(string tag);
private:
	static map<string, vector<UnicornBase*>> tags;
};