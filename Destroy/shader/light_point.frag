#version 330 core

in vec3 vertex_out_color;

out vec4 frag_color;

uniform vec3 color_light;
uniform vec3 color_object;

void main()
{
	frag_color = vec4(vertex_out_color, 1.0);
}