#include "opengl.h"

class  Camera
{
public:

	vec3 getCameraPosition();
protected:

	GLfloat move_speed	= 1.0f;
	GLfloat yaw_speed	= 0.01f;
	GLfloat pitch_speed = 0.01f;
	vec3 camera_position	= vec3(0.0f, 0.0f, 10.0f);
	vec3 camera_center		= vec3(0.0f, 0.0f,  0.0f);
	vec3 camera_up			= vec3(0.0f, 1.0f,  0.0f);

};

class EditCamera:public Camera
{
public:
	virtual void updateMouse(int button, int state, int x, int y);
	virtual void updateMouseMotion(int x, int y);
	virtual void updateSpecial(int key, int x, int y);
	virtual void updateKeyBoard(unsigned char button, int x, int y);
	virtual mat4 setCamera();
private:
	

	GLfloat max_pitch_angle = 45.0f;

	GLfloat angle_h = 0;
	GLfloat angle_v = 0;

	vec3 camera_move = vec3(0.0f);

	GLint last_x = -1;
	GLint last_y = -1;
};

class GameCamera :public Camera
{};
