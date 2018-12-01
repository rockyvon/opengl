#include "stdafx.h"
#include "camera.h"

void EditCamera::updateMouse(int button, int state, int x, int y)
{
}

void EditCamera::updateMouseMotion(int x, int y)
{
	if (last_x < 0) {
		last_x = x;
	}
	if (last_y < 0) {
		last_y = y;
	}
	angle_h += (x - last_x) * yaw_speed;
	angle_v += (y - last_y) * pitch_speed;
	last_x = x;
	last_y = y;
}

void EditCamera::updateSpecial(int key, int x, int y)
{
	
	switch (key)
	{
	case GLUT_KEY_PAGE_UP:
		camera_position += camera_up * move_speed;
		camera_center += camera_up * move_speed;
		break;
	case GLUT_KEY_PAGE_DOWN:
		camera_position -= camera_up * move_speed;
		camera_center -= camera_up * move_speed;
		break;
	default:
		break;
	}
}

void EditCamera::updateKeyBoard(unsigned char button, int x, int y)
{
	switch (button)
	{
		vec3 camera_forward;
		vec3 camera_left;
	case 'w':
		camera_forward = normalize(camera_center - camera_position);
		camera_position += camera_forward * move_speed;
		camera_center += camera_forward * move_speed;
		break;
	case 's':
		camera_forward = normalize(camera_center - camera_position);
		camera_position -= camera_forward * move_speed;
		camera_center -= camera_forward * move_speed;
		break;
	case'a':
		camera_forward = normalize(camera_center - camera_position);
		camera_left = normalize(cross(camera_up, camera_forward));
		camera_position += camera_left * move_speed;
		camera_center += camera_left * move_speed;
		break;
	case'd':
		camera_forward = normalize(camera_center - camera_position);
		camera_left = normalize(cross(camera_up, camera_forward));
		camera_position -= camera_left * move_speed;
		camera_center -= camera_left * move_speed;
		break;
	default:
		break;
	}
}

mat4 EditCamera::setCamera()
{
	mat4 view = mat4(1.0f);
	view = lookAt(camera_position, camera_center, camera_up);
	view = rotate(view, angle_h, vec3(0, 1.0f, 0));
	view = rotate(view, angle_v, vec3(1.0f, 0, 0));
	return view;
}

vec3 Camera::getCameraPosition()
{
	return camera_position;
}

