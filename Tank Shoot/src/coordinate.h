#ifndef COORDINATE_H
#define COORDINATE_H

#include "opengl.h"
class Coordinate {

};

class Coordinate3D : public Coordinate {
public:
	Coordinate3D();
	Coordinate3D(vec3 pos, vec3 forword, vec3 up, vec3 left);
	vec3 position;
	vec3 forword;
	vec3 up;
	vec3 left;
private:
};

#endif // !COORDINATE_H