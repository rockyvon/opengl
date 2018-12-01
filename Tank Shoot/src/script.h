#pragma once

#ifndef SCRIPT_H
#define SCRIPT_H

#include "action.h"
#include "unicorn.h"
#include "tag.h"

class PlayerScript : public Script {
public:
	virtual void update();
private:

};

class CameraScript : public Script {
public:
	virtual void update();
private:
};

#endif // !SCRIPT_H