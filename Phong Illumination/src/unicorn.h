#pragma once

#ifndef UNICORN_H
#define UNICORN_H

class UnicornBase {
public:
	void setEnable(bool enabled);
	bool getEnable();
	
private:
protected:
	bool enable = true;
};

#endif // !UNICORN_H