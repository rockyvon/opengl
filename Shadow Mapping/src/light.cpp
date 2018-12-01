#include "stdafx.h"
#include "light.h"

Light::Light()
	:ambient(vec3(1.0f)), diffuse(vec3(1.0f)), specular(vec3(1.0f)), shading(false)
{
}

Light::Light(vec3 amb, vec3 dif, vec3 spe, bool shading)
	:ambient(amb), diffuse(dif), specular(spe), shading(shading)
{
}

void Light::setRenderFunc(void(*callback)(ShaderManager *))
{
	this->callback = callback;
}

void Light::refresh(int index, const string name, ShaderManager * shader)
{
	if (enable && shading && callback) {
		shadowRefresh(shader);
	}
	shader->setUniform1i(name + "[" + to_string(index) + "].enable", enable);
	if (enable) {
		shader->setUniform3fv(name + "[" + to_string(index) + "].ambient", 1, value_ptr(ambient));
		shader->setUniform3fv(name + "[" + to_string(index) + "].diffuse", 1, value_ptr(diffuse));
		shader->setUniform3fv(name + "[" + to_string(index) + "].specular", 1, value_ptr(specular));
	}
	
}

LightDirectional::LightDirectional(vec3 dir, vec3 amb, vec3 dif, vec3 spe, bool shading)
	:direction(dir), Light(amb, dif, spe, shading)
{
	enableShadow(shading);
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

void LightDirectional::enableShadow(bool enabled)
{
	shading = enabled;
	if (shading && shadow == NULL)
	{
		shadow = new DShadow();
	}
	if (!shading && shadow != NULL) {
		delete shadow;
		shadow = NULL;
	}
}

void LightDirectional::shadowRefresh(ShaderManager * shader)
{
	mat4 lsm;
	shadow->draw(vec3(-direction.x * 20, -direction.y * 20, -direction.z * 20), direction, lsm, callback);
	shader->use();
	shader->setUniform1i("depthMap", 0);
	shader->setUniformMatrix4fv("LSM", 1, value_ptr(lsm));
}

void LightDirectional::refresh(int index, const string name, ShaderManager * shader)
{
	Light::refresh(index, name, shader);
	if(enable)
		shader->setUniform3fv(name + "[" + to_string(index) + "].direction", 1, value_ptr(direction));
}

LightPoint::LightPoint()
	:position(vec3(0.0f)),constant(1.0f), linear(0.09f), quadratic(0.032f),Light()
{}

LightPoint::LightPoint(vec3 pos, float c, float l, float q, vec3 amb, vec3 dif, vec3 spe, bool shading)
	:position(pos),
	constant(c), linear(l), quadratic(q),
	Light(amb, dif, spe, shading) 
{
	enableShadow(shading);
}

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

void LightPoint::enableShadow(bool enabled)
{
	shading = enabled;
	if (shading && shadow == NULL)
	{
		shadow = new LShadow();
	}
	if (!shading && shadow != NULL) {
		delete shadow;
		shadow = NULL;
	}
}

void LightPoint::shadowRefresh(ShaderManager * shader)
{
	mat4 lsm;
	shadow->draw(position, vec3(0.0f), lsm, callback);
	shader->use();
	shader->setUniformMatrix4fv("LSM", 1, value_ptr(lsm));
}

void LightPoint::refresh(int index, const string name, ShaderManager * shader)
{
	Light::refresh(index, name, shader);
	if (enable) {
		shader->setUniform3fv(name + "[" + to_string(index) + "].position", 1, value_ptr(position));
		shader->setUniform1f(name + "[" + to_string(index) + "].c", constant);
		shader->setUniform1f(name + "[" + to_string(index) + "].l", linear);
		shader->setUniform1f(name + "[" + to_string(index) + "].q", quadratic);
	}
	
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
	if (enable) {
		shader->setUniform3fv(name + "[" + to_string(index) + "].direction", 1, value_ptr(direction));
		shader->setUniform1f(name + "[" + to_string(index) + "].cutoff", cutoff);
	}
	
}

void LightManager::initData()
{
	const int LD_SIZE = 1;
	const int LP_SIZE = 1;
	const int LS_SIZE = 1;
	GLfloat ldsData[LD_SIZE][12] = {
		{
			1.0f, -1.0f, 1.0f,
			0.3f,  0.3f,  0.3f,
			0.5f,  0.5f,  0.5f,
			0.2f,  0.2f,  0.2f,
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
		false
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
		false
	};

	for (unsigned int i = 0; i < LD_SIZE; i++)
	{
		addDirectional("ld" + to_string(i), ldsData[i], ldsShadow[i]);
	}
	for (unsigned int i = 0; i < LP_SIZE; i++)
	{
		addPoint("lp" + to_string(i), lpsData[i], lpsShadow[i]);
	}
	for (unsigned int i = 0; i < LS_SIZE; i++)
	{
		addSpot("ls" + to_string(i), lssData[i], lssShadow[i]);
	}
}

void LightManager::refresh(ShaderManager* shader)
{
	map<string, LightDirectional*>::iterator itd = lds.begin();
	for (unsigned int i = 0; i < lds.size(); i++)
	{
		itd->second->refresh(i, DL, shader);
		itd++;
	}
	map<string, LightPoint*>::iterator itp = lps.begin();
	for (unsigned int i = 0; i < lps.size(); i++)
	{
		itp->second->refresh(i, PL, shader);
		itp++;
	}
	map<string, LightSpot*>::iterator its = lss.begin();
	for (unsigned int i = 0; i < lss.size(); i++)
	{
		its->second->refresh(i, SL, shader);
		its++;
	}
}

void LightManager::setDirectionalEnable(string name, bool enabled)
{
	lds[name]->setEnable(enabled);
}

void LightManager::setPointEnable(string name, bool enabled)
{
	lps[name]->setEnable(enabled);
}

void LightManager::setSpotEnable(string name, bool enabled)
{
	lss[name]->setEnable(enabled);
}

void LightManager::setRenderFunc(void(*callback)(ShaderManager *))
{
	map<string, LightDirectional*>::iterator itd = lds.begin();
	for (unsigned int i = 0; i < lds.size(); i++)
	{
		itd->second->setRenderFunc(callback);
		itd++;
	}
	map<string, LightPoint*>::iterator itp = lps.begin();
	for (unsigned int i = 0; i < lps.size(); i++)
	{
		itp->second->setRenderFunc(callback);
		itp++;
	}
	map<string, LightSpot*>::iterator its = lss.begin();
	for (unsigned int i = 0; i < lss.size(); i++)
	{
		its->second->setRenderFunc(callback);
		its++;
	}
}

LightDirectional * LightManager::getDirectional(string name)
{
	return lds[name];
}

LightPoint * LightManager::getPoint(string name)
{
	return lps[name];
}

LightSpot * LightManager::getSpot(string name)
{
	return lss[name];
}

void LightManager::addDirectional(string name, LightDirectional * ld)
{
	lds[name] = ld;
}

void LightManager::addDirectional(string name, const GLfloat * data, bool shading)
{
	addDirectional(name, new LightDirectional(data, shading));
}

void LightManager::addPoint(string name, LightPoint * lp)
{
	lps[name] = lp;
}

void LightManager::addPoint(string name, GLfloat * data, bool shading)
{
	addPoint(name, new LightPoint(data, shading));
}

void LightManager::addSpot(string name, LightSpot * ls)
{
	lss[name] = ls;
}

void LightManager::addSpot(string name, GLfloat * data, bool shading)
{
	addSpot(name, new LightSpot(data, shading));
}
