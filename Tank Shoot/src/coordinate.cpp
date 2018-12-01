#include "stdafx.h"

#include "coordinate.h"

Coordinate3D::Coordinate3D() :
	position(vec3(0.0f)), 
	forword(vec3(0.0f, 0.0f, 1.0f)),
	up(vec3(0.0f, 1.0f, 0.0f)),
	left(vec3(1.0f, 0.0f, 0.0f))
{

}

Coordinate3D::Coordinate3D(vec3 pos, vec3 forword, vec3 up, vec3 left) :
	position(pos),
	forword(forword),
	up(up),
	left(left)
{
}
