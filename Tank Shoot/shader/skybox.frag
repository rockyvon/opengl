#version 330 core

in vec3 texCoords;
out vec4 FragColor;
uniform samplerCube skyTex;

void main()
{
	FragColor = texture(skyTex, texCoords);
	//FragColor = vec4(vec3(gl_FragCoord.z), 1.0f);
}