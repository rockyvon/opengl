#include "stdafx.h"

class LogUtils {
public :
	static LogUtils* createInstance();
	void init(const char* path);
	void write(const char* data);
	void flush();
private:
	LogUtils();
	void writeToFile();
	static LogUtils* _Instance;
	const char * path;
	static const int BUFFER_SIZE = 512;
	char buffer[BUFFER_SIZE];
	int cursor = -1;

};