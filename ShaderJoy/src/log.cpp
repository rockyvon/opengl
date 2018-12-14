#include "stdafx.h"
#include "log.h"

void CLog::error(const std::string& info)
{
	std::cout << "error:" << info.c_str() << std::endl;
	glfwTerminate();
	std::cout << "press any key to continue..." << std::endl;
	char res;
	std::cin >> res;
}

void CLog::error(const GLubyte * info)
{
	fprintf(stderr, "error: '%s'\n", info);
	glfwTerminate();
	std::cout << "press any key to continue..." << std::endl;
	char res;
	std::cin >> res;
}
