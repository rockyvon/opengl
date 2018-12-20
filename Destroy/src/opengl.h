#pragma once
#include <gl/glew.h>
#include <gl/glut.h>
#include <vector>

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

using namespace Assimp;

#include "shader.h"

using namespace glm;

const enum LIGHT_PHONG_MODEL
{
	AMBIENT = 0, 
	DIFFUSE = 1, 
	SPECULAR = 2
};

const enum LIGHT_TYPE
{
	DIRECTIONAL_LIGHT	= 0,
	POINT_LIGHT			= 1,
	SPOT_LIGHT			= 2
};