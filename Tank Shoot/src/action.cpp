#include "stdafx.h"
#include "action.h"

int Mouse::mouse = 0x0003;
int Mouse::state = 0x0002;
int Mouse::x = -1;
int Mouse::y = -1;

void Mouse::updateMouse(int mouse, int state, int x, int y)
{
	Mouse::mouse = mouse;
	Mouse::state = state;
	Mouse::x = x;
	Mouse::y = y;

}

bool Mouse::click(int mouse)
{
	return Mouse::state == UNICORN_DOWN && mouse == Mouse::mouse;
}

void Mouse::getPosition(int & x, int & y)
{
	x = Mouse::x;
	y = Mouse::y;
}

int Keyboard::special = UNICORN_NULL_KEY;
char Keyboard::button = UNICORN_NULL_KEY;

void Keyboard::updateButton(char button)
{
	Keyboard::button = button;
}

void Keyboard::updateUpButton(char button)
{
	Keyboard::button = UNICORN_NULL_KEY;
}

void Keyboard::updateSpecial(int special)
{
	Keyboard::special = special;
}

void Keyboard::updateUpSpecial(int special)
{
	Keyboard::special = UNICORN_NULL_KEY;
}

bool Keyboard::click(char button)
{
	return Keyboard::button == button;
}

bool Keyboard::click(int special)
{
	return Keyboard::special == special;;
}


