#include "stdafx.h"
#include "fileio.h"
#include "utils.h"

LogUtils::LogUtils() 
{

}

LogUtils * LogUtils::_Instance = NULL;
LogUtils* LogUtils::createInstance()
{
	if (!_Instance)
		_Instance = new LogUtils();
	return _Instance;
}

void LogUtils::init(const char* path)
{
	this->path = + path;
}

void LogUtils::writeToFile()
{
	if (cursor == -1)
		return;
	FILE* file;
	int fail = fopen_s(&file, path, "a");
	if (fail) {
		cout << "文件无法打开：" << path << endl;
		exit(1);
	}
	//fputs(buffer, file);
	for (int i = 0; i <= cursor; i++) {
		fputc(buffer[i], file);
	}
	fclose(file);
	cursor = -1;
}


void LogUtils::write(const char* data)
{
	//int data_length = Utils::getLength(data);
	int data_length = strlen(data);
	int length = Utils::min(data_length, BUFFER_SIZE - (cursor + 1));
	
	for (int i = 0; i < length; i++)
	{
		cursor++;
		buffer[cursor] = data[i];
	}
	if (cursor >= BUFFER_SIZE - 1)//缓冲区已满
	{
		writeToFile();
	}
	if (data_length > length)
	{
		write(data + length * sizeof(char));
	}
}

void LogUtils::flush()
{
	writeToFile();
}