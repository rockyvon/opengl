#include "stdafx.h"
#include "unicorn.h"

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
