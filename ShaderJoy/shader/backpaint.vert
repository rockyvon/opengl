#version 330 core

struct vfData{
    vec2 texCoords;
};

layout(location = 0) in vec2 position;
layout(location = 1) in vec2 texCoords;


out vfData data;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    data.texCoords = texCoords;
    gl_Position = projection * view * model * vec4(position, 0.0, 1.0);
}