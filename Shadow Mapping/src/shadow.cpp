#include "stdafx.h"
#include "shadow.h"

Shadow::Shadow()
{
}

Shadow::~Shadow()
{
}

void Shadow::draw(vec3 position, vec3 direction, mat4& lsm, void(*callback)(ShaderManager *))
{
	shader->use();
	glViewport(0, 0, shadowSize[0], shadowSize[1]);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	glClear(GL_DEPTH_BUFFER_BIT);

	mat4 lightProjection = ortho(-100.0f, 100.0f, -100.0f, 100.0f, 1.0f, 100.0f);
	mat4 lightView = lookAt(position, position + direction, vec3(1.0f));
	lsm = lightProjection * lightView;

	shader->setUniformMatrix4fv("LSM", 1, value_ptr(lsm));
	glCullFace(GL_BACK);
	callback(shader);
	glCullFace(GL_FRONT);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texId);




	/*glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	shader->setUniform1i("depthMap", 0);
	callback(shader);
	shader->checkGLError();*/
}

DShadow::DShadow()
{
	init();
}


DShadow::~DShadow()
{
}

void DShadow::init()
{
	shader = ShaderManager::NewInstance();
	shader->load(shader_shdow_path[0], shader_shdow_path[1], shader_shdow_path[2]);
	shader->use();
	glGenFramebuffers(1, &fbo);

	glGenTextures(1, &texId);
	glBindTexture(GL_TEXTURE_2D, texId);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, shadowSize[0], shadowSize[1], 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, texId, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

}

LShadow::LShadow()
{
}

LShadow::~LShadow()
{
}

void LShadow::init()
{
}
