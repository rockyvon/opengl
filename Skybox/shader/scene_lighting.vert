#version 330 core

struct VFData{
	vec3 position;
	vec3 color;
	vec3 normal;
	vec2 texCoords;
};

layout(location = 0) in vec3 position_vertex;
layout(location = 1) in vec3 color_vertex;
layout(location = 2) in vec3 normal_vertex;
layout(location = 3) in vec2 texCoords_vertex;

out VFData vfdata;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	vfdata.position = vec3(model * vec4(position_vertex, 1.0));
	vfdata.color = color_vertex;
	vfdata.normal = mat3(transpose(inverse(model))) * normal_vertex;
	vfdata.texCoords = texCoords_vertex;
	gl_Position = projection * view * model * vec4(position_vertex, 1.0);

}