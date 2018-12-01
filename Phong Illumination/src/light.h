#include "opengl.h"
#include "shader.h"
#include "unicorn.h"

#include <map>

const char DL[4] = "lds";
const char PL[4] = "lps";
const char SL[4] = "lss";

class Light : public UnicornBase {
public:
	Light();
	Light(vec3 amb, vec3 dif, vec3 spe, bool shading);
	void enableShadow(bool enabled);
	virtual void refresh(int index, const string name, ShaderManager * shader);
protected:
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	bool shading = false;
	vec3 BLACK_VEC3 = vec3(0.0f);
private:

};

class LightDirectional : public Light
{
protected:
	
public:
	vec3 direction;
	LightDirectional(vec3 dir, vec3 amb, vec3 dif, vec3 spe, bool shading = false);
	LightDirectional(const GLfloat * data, bool shading = false);
	LightDirectional(const LightDirectional& ld);
	virtual void refresh(int index, const string name, ShaderManager * shader);
};

class LightPoint : public Light
{
public:
	LightPoint();
	LightPoint(vec3 pos, float c, float l, float q, vec3 amb, vec3 dif, vec3 spe, bool shading = false);
	LightPoint(GLfloat * data, bool shading = false);
	LightPoint(const LightPoint& lp);
	virtual void refresh(int index, const string name, ShaderManager * shader);
protected:
	vec3 position;
	float constant;
	float linear;
	float quadratic;
};

class LightSpot : public LightPoint
{
public:
	LightSpot(vec3 dir, vec3 pos, float c, float l, float q, vec3 amb, vec3 dif, vec3 spe, float cut, bool shading = false);
	LightSpot(GLfloat * data, bool shading = false);
	LightSpot(const LightSpot& ls);
	void refresh(int index, const string name, ShaderManager * shader);
private:
	vec3 direction;
	float cutoff;
};

class LightManager {
public:
	void initData();
	void refresh(ShaderManager* shader);
	void setDirectionalEnable(string name, bool enabled);
	void setPointEnable(string name, bool enabled);
	void setSpotEnable(string name, bool enabled);
	LightDirectional* getDirectional(string name);
	LightPoint* getPoint(string name);
	LightSpot* getSpot(string name);
	void addDirectional(string name, LightDirectional* ld);
	void addDirectional(string name, const GLfloat * data, bool shading = false);
	void addPoint(string name, LightPoint* lp);
	void addPoint(string name, GLfloat * data, bool shading = false);
	void addSpot(string name, LightSpot* ls);
	void addSpot(string name, GLfloat * data, bool shading = false);
private:
	ShaderManager * shader;
	map<string, LightDirectional*> lds;
	map<string, LightPoint*> lps;
	map<string, LightSpot*> lss;
};