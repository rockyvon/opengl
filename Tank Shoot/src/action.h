#ifndef ACTION_H
#define ACTION_H

#define UNICORN_NULL_KEY					   0x0000

#define  UNICORN_KEY_F1                        0x0001
#define  UNICORN_KEY_F2                        0x0002
#define  UNICORN_KEY_F3                        0x0003
#define  UNICORN_KEY_F4                        0x0004
#define  UNICORN_KEY_F5                        0x0005
#define  UNICORN_KEY_F6                        0x0006
#define  UNICORN_KEY_F7                        0x0007
#define  UNICORN_KEY_F8                        0x0008
#define  UNICORN_KEY_F9                        0x0009
#define  UNICORN_KEY_F10                       0x000A
#define  UNICORN_KEY_F11                       0x000B
#define  UNICORN_KEY_F12                       0x000C
#define  UNICORN_KEY_LEFT                      0x0064
#define  UNICORN_KEY_UP                        0x0065
#define  UNICORN_KEY_RIGHT                     0x0066
#define  UNICORN_KEY_DOWN                      0x0067
#define  UNICORN_KEY_PAGE_UP                   0x0068
#define  UNICORN_KEY_PAGE_DOWN                 0x0069
#define  UNICORN_KEY_HOME                      0x006A
#define  UNICORN_KEY_END                       0x006B
#define  UNICORN_KEY_INSERT                    0x006C

#define  UNICORN_LEFT_BUTTON                   0x0000
#define  UNICORN_MIDDLE_BUTTON                 0x0001
#define  UNICORN_RIGHT_BUTTON                  0x0002
#define  UNICORN_DOWN                          0x0000
#define  UNICORN_UP                            0x0001
#define  UNICORN_LEFT                          0x0000
#define  UNICORN_ENTERED                       0x0001
class Mouse {
public:
	static void updateMouse(int mouse, int state, int x, int y);
	static bool click(int mouse);
	static void getPosition(int &x, int &y);
private:
	static int mouse;
	static int state;
	static int x;
	static int y;
};

class Keyboard {
public:
	static void updateButton(char button);
	static void updateUpButton(char button);
	static void updateSpecial(int special);
	static void updateUpSpecial(int special);
	static bool click(char button);
	static bool click(int special);
private:
	static int special;
	static char button;
};

#endif // !ACTION_H