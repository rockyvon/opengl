#ifndef LOG_H
#define LOG_H

#include "stdafx.h"
#include "opengl.h"

class CLog
{
public:
	static void error(const std::string&);
	static void error(const GLubyte*);
};

#endif // !LOG_H

