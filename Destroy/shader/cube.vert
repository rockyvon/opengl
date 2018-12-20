# version 330 core

layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec3 vertex_color;
layout(location = 2) in vec2 vertex_texture_coor;
layout(location = 3) in vec3 vertex_normal;

out vec3 fragment_color;
out vec2 fragment_vertex_coor;
out vec3 fragment_position;
out vec3 fragment_normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	fragment_color = vertex_color;
	fragment_position = vec3(model * vec4(vertex_position, 1.0));
	fragment_normal = mat3(transpose(inverse(model))) * vertex_normal;
	fragment_vertex_coor = vertex_texture_coor;
	gl_Position = projection * view * model * vec4(vertex_position, 1.0);
	
}