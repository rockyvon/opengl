#include "stdafx.h"
#include "light.h"

LightDirectional * LightManager::getDirectionl(int index)
{
	return lds[index];
}

LightPoint * LightManager::getPoint(int index)
{
	return lps[index];
}

LightSpot * LightManager::getSpot(int index)
{
	return lss[index];
}

Light::Light()
	:ambient(vec3(1.0f)), diffuse(vec3(1.0f)), specular(vec3(1.0f)), shading(false)
{
}

Light::Light(vec3 amb, vec3 dif, vec3 spe, bool shading)
	:ambient(amb), diffuse(dif), specular(spe), shading(shading)
{
}

void Light::enableShadow(bool enabled)
{
	shading = enabled;
}

void Light::refresh(int index, const string name, ShaderManager * shader)
{
	shader->setUniform3fv(name + "[" + to_string(index) + "].ambient", 1, value_ptr(enable ? ambient : BLACK_VEC3));
	shader->setUniform3fv(name + "[" + to_string(index) + "].diffuse", 1, value_ptr(enable ? diffuse : BLACK_VEC3));
	shader->setUniform3fv(name + "[" + to_string(index) + "].specular", 1, value_ptr(enable ? specular : BLACK_VEC3));
}

LightDirectional::LightDirectional(vec3 dir, vec3 amb, vec3 dif, vec3 spe, bool shading)
	:direction(dir), Light(amb, dif, spe, shading)
{
}

LightDirectional::LightDirectional(const GLfloat * data, bool shading)
	: LightDirectional(
		vec3(data[0], data[1], data[2]),
		vec3(data[3], data[4], data[5]),
		vec3(data[6], data[7], data[8]),
		vec3(data[9], data[10], data[11]),
		shading)
{}

LightDirectional::LightDirectional(const LightDirectional & ld):
	direction(ld.direction), Light(ld.ambient, ld.diffuse, ld.specular, ld.shading)
{}

void LightDirectional::refresh(int index, const string name, ShaderManager * shader)
{
	Light::refresh(index, name, shader);
	shader->setUniform3fv(name + "[" + to_string(index) + "].direction", 1, value_ptr(direction));
}

LightPoint::LightPoint()
	:position(vec3(0.0f)),constant(1.0f), linear(0.09f), quadratic(0.032f),Light()
{}

LightPoint::LightPoint(vec3 pos, float c, float l, float q, vec3 amb, vec3 dif, vec3 spe, bool shading)
	:position(pos),
	constant(c), linear(l), quadratic(q),
	Light(amb, dif, spe, shading) 
{}

LightPoint::LightPoint(GLfloat * data, bool shading)
	:LightPoint(
		vec3(data[0], data[1], data[2]),
		data[3], 
		data[4], 
		data[5],
		vec3(data[6], data[7], data[8]),
		vec3(data[9], data[10], data[11]),
		vec3(data[12], data[13], data[14]),
		shading)
{}

LightPoint::LightPoint(const LightPoint& lp) 
	:position(lp.position), constant(lp.constant), linear(lp.linear), quadratic(lp.quadratic),
	Light(lp.ambient, lp.diffuse, lp.specular, lp.shading) 
{}


void LightPoint::refresh(int index, const string name, ShaderManager * shader)
{
	Light::refresh(index, name, shader);
	shader->setUniform3fv(name + "[" + to_string(index) + "].position", 1, value_ptr(position));
	shader->setUniform1f(name + "[" + to_string(index) + "].c", constant);
	shader->setUniform1f(name + "[" + to_string(index) + "].l", linear);
	shader->setUniform1f(name + "[" + to_string(index) + "].q", quadratic);
}

LightSpot::LightSpot(vec3 dir, vec3 pos, float c, float l, float q, vec3 amb, vec3 dif, vec3 spe, float cut = 90.0f, bool shading)
	: direction(dir), cutoff(radians(cut)), LightPoint(pos, c, l, q, amb, dif, spe, shading)
{}

LightSpot::LightSpot(GLfloat * data, bool shading)
	: LightSpot(
		vec3(data[0], data[1], data[2]),
		vec3(data[3], data[4], data[5]),
		data[6],
		data[7],
		data[8],
		vec3(data[9], data[10], data[11]),
		vec3(data[12], data[13], data[14]),
		vec3(data[15], data[16], data[17]),
		data[18],
		shading)
{}

LightSpot::LightSpot(const LightSpot & ls)
	:direction(ls.direction), 
	cutoff(ls.cutoff), 
	LightPoint(ls.position, ls.constant, ls.linear, ls.quadratic, ls.ambient, ls.diffuse, ls.specular, ls.shading)
{}



void LightSpot::refresh(int index, const string name, ShaderManager * shader)
{
	LightPoint::refresh(index, name, shader);
	shader->setUniform3fv(name + "[" + to_string(index) + "].direction", 1, value_ptr(direction));
	shader->setUniform1f(name + "[" + to_string(index) + "].cutoff", enable ? cutoff : 0.0f);
}

void LightManager::initData()
{
	const int LD_SIZE = 1;
	const int LP_SIZE = 1;
	const int LS_SIZE = 1;
	GLfloat ldsData[LD_SIZE][12] = {
		{
			-1.0f, -1.0f, -1.0f,
			0.3f,  0.3f,  0.3f,
			0.3f,  0.5f,  0.5f,
			0.3f,  0.3f,  0.3f,
		}
	};
	GLfloat ldsShadow[LD_SIZE] = {
		true
	};
	GLfloat lpsData[LP_SIZE][15] = {
		{
			100.00f, 100.00f, 100.000f,
			1.00f,   0.09f,   0.032f,
			0.3f,    0.3f,    0.3f,
			0.8f,    0.8f,    0.8f,
			0.6f,    0.6f,    0.6f
		}
	};
	GLfloat lpsShadow[LP_SIZE] = {
		true
	};
	GLfloat lssData[LS_SIZE][19] = {
		{
			0.0f,    0.0f,   -1.0f,
			0.0f,    1.0f,    1.0f,
			1.00f,   0.09f,   0.032f,
			0.3f,    0.3f,    0.3f,
			0.8f,    0.8f,    0.8f,
			0.6f,    0.6f,    0.6f,
			20.0f
		}
	};
	GLfloat lssShadow[LS_SIZE] = {
		true
	};

	for (unsigned int i = 0; i < LD_SIZE; i++)
	{
		lds.push_back(new LightDirectional(ldsData[i], ldsShadow[i]));
	}
	for (unsigned int i = 0; i < LP_SIZE; i++)
	{
		lps.push_back(new LightPoint(lpsData[i], lpsShadow[i]));
	}
	for (unsigned int i = 0; i < LS_SIZE; i++)
	{
		lss.push_back(new LightSpot(lssData[i], lssShadow[i]));
	}
}

void LightManager::refresh(ShaderManager* shader)
{
	for (unsigned int i = 0; i < lds.size(); i++)
	{
		lds[i]->refresh(i, DL, shader);
	}
	for (unsigned int i = 0; i < lps.size(); i++)
	{
		lps[i]->refresh(i, PL, shader);
	}
	for (unsigned int i = 0; i < lss.size(); i++)
	{
		lss[i]->refresh(i, SL, shader);
	}
}

void LightManager::setAllLightEnable(bool dl_enabled, bool pl_enabled, bool ps_enabled)
{
	setDirectionalEnable(dl_enabled);
	setPointEnable(pl_enabled);
	setSpotEnable(ps_enabled);
}

void LightManager::setDirectionalEnable(bool enabled)
{
	for (unsigned int i = 0; i < lds.size(); i++)
		lds[i]->setEnable(enabled);
}

void LightManager::setPointEnable(bool enabled)
{
	for (unsigned int i = 0; i < lps.size(); i++)
		lps[i]->setEnable(enabled);
}

void LightManager::setSpotEnable(bool enabled)
{
	for (unsigned int i = 0; i < lss.size(); i++)
		lss[i]->setEnable(enabled);
}
