# version 330 core

layout(location = 0) in vec3 position;

out vec3 lcposition;

uniform mat4 LSM;
uniform mat4 model;

void main()
{
	gl_Position = LSM * model * vec4(position, 1.0);
	lcposition = gl_Position.xyz / gl_Position.w;
	lcposition = lcposition * 0.5 + 0.5;
}