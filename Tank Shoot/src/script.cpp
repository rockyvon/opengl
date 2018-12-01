#include "stdafx.h"
#include "script.h"
#include "camera.h"

void PlayerScript::update()
{
	if (Keyboard::click('w')) {
		object->transform->position = Transform::translate(object->transform->position, object->transform->localCoords.forword, 1.0f);
	}
	else if (Keyboard::click('s')) {
		object->transform->position = Transform::translate(object->transform->position, object->transform->localCoords.forword, -1.0f);
	}
	else if (Keyboard::click('a')) {
		object->transform->position = Transform::translate(object->transform->position, object->transform->localCoords.left, 1.0f);
	}
	else if (Keyboard::click('d')) {
		object->transform->position = Transform::translate(object->transform->position, object->transform->localCoords.left, -1.0f);
	}
	else if (Keyboard::click('q')) {

	}
	else if (Keyboard::click('e')) {

	}
}

void CameraScript::update()
{
	/*UnicornBase* player = Tag::getObjectByTag("Player");
	object->transform->position = Transform::translate(
		player->transform->position,
		player->transform->localCoords.forword,
		-15.0f);

	object->transform->position = Transform::translate(
		object->transform->position,
		player->transform->localCoords.left,
		-8.0f);

	object->transform->position = Transform::translate(
		object->transform->position,
		player->transform->localCoords.up,
		10.0f);*/

	Camera* camera = (Camera*)(object->getComponent(CAMERA_NAME));

	camera->updateCamera();
}
