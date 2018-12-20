#include "stdafx.h"
#include "shadow.h"

void Shadow::init(LIGHT_TYPE light_type, vec3 eye, vec3 center, vec3 up)
{
	if (light_type == DIRECTIONAL_LIGHT) 
	{
		this->eye = vec3(-eye.x * 20, -eye.y * 20, -eye.z * 20);
	}
	else {
		this->eye = eye;
	}
	this->center = center;
	this->up = up;
	this->light_type = light_type;

	//准备深度缓冲
	glGenFramebuffers(1, &fbo);

	glGenTextures(1, &depth_texture);
	glBindTexture(GL_TEXTURE_2D, depth_texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, shadow_map_resolution[0], shadow_map_resolution[1], 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	//设置环绕方式，超出阴影计算边界则不会产生阴影
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	GLfloat borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depth_texture, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Shadow::bakeMap(ShaderManager * shader)
{
	//渲染深度贴图
	if(light_type == DIRECTIONAL_LIGHT)
	{
		projection = ortho(
			-shadow_size[0]/2, shadow_size[0] / 2,
			-shadow_size[1] / 2, shadow_size[1] / 2, 
			1.0f, 100.0f);
	}
	else
	{
		projection = perspective(radians(90.0f), 1.0f, 1.0f, 100.0f);
	}
	view = lookAt(eye, center, up);
	shader->setUniformMatrix4fv("projection", 1, value_ptr(projection));
	shader->setUniformMatrix4fv("view", 1, value_ptr(view));

	glViewport(0, 0, shadow_map_resolution[0], shadow_map_resolution[1]);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	glClear(GL_DEPTH_BUFFER_BIT);
}

void Shadow::renderShadow()
{
	//使用深度贴图渲染场景
	
}

void Shadow::endOffscreen()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Shadow::bindShadowTexture(ShaderManager * shader)
{
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, depth_texture);
	shader->setUniform1i("texture_depth", 2);
}

mat4 * Shadow::getView()
{
	return &view;
}

mat4 * Shadow::getProjection()
{
	return &projection;
}

GLint * Shadow::getResolution()
{
	return shadow_map_resolution;
}

GLfloat * Shadow::getSize()
{
	return shadow_size;
}


