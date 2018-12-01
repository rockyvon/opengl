#version 330 core   

struct vfData{
    vec2 texCoords;
};

in vfData data;

out vec4 FragColor;

uniform sampler2D image;
uniform vec3 color;

void main()
{
    FragColor = vec4(color, 1.0) * texture(image, data.texCoords);
   // FragColor = vec4(1.0);
}