# version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 vertex_normal;
layout(location = 2) in vec2 vertex_texture_coor;

out struct VFData{
	vec3 position;
	vec3 normal;
	vec2 vertex_coor;
	vec4 light_coord_position;
}vfdata;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform mat4 light_view;
uniform mat4 light_projection;

void main()
{
	vfdata.position = vec3(model * vec4(position, 1.0));
	vfdata.normal = mat3(transpose(inverse(model))) * vertex_normal;
	vfdata.vertex_coor = vertex_texture_coor;
	vfdata.light_coord_position = light_projection * light_view * vec4(vfdata.position, 1.0);
	gl_Position = projection * view * model * vec4(position, 1.0);
}