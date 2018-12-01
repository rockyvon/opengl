#include "stdafx.h"
#include "camera.h"

/*
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
		position += up * move_speed;
		center += up * move_speed;
		break;
	case GLUT_KEY_PAGE_DOWN:
		position -= up * move_speed;
		center -= up * move_speed;
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
		camera_forward = normalize(center - position);
		position += camera_forward * move_speed;
		center += camera_forward * move_speed;
		break;
	case 's':
		camera_forward = normalize(center - position);
		position -= camera_forward * move_speed;
		center -= camera_forward * move_speed;
		break;
	case'a':
		camera_forward = normalize(center - position);
		camera_left = normalize(cross(up, camera_forward));
		position += camera_left * move_speed;
		center += camera_left * move_speed;
		break;
	case'd':
		camera_forward = normalize(center - position);
		camera_left = normalize(cross(up, camera_forward));
		position -= camera_left * move_speed;
		center -= camera_left * move_speed;
		break;
	default:
		break;
	}
}

mat4 EditCamera::setCamera()
{
	mat4 view = mat4(1.0f);
	view = lookAt(position, center, up);
	view = rotate(view, angle_h, vec3(0, 1.0f, 0));
	view = rotate(view, angle_v, vec3(1.0f, 0, 0));
	return view;
}*/

Camera::Camera()
{
	name = CAMERA_NAME;
}

void Camera::updateCamera()
{
	center = center + object->transform->position - position;
	position = object->transform->position;
	
	
	//cout << "center:" << center.x << "\t" << center.y << "\t" << center.z << endl;
	
	view = lookAt(position, center, up);
}

vec3 Camera::getCameraPosition()
{
	return position;
}

