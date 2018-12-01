#version 330 core

in vec3 lcposition;

out vec4 FragColor;

uniform sampler2D depthMap;

void main()
{
	//FragColor = vec4(vec3(texture(depthMap, lcposition.xy).rgb), 1.0);
	//FragColor = vec4(vec3(gl_FragCoord.z), 1.0);
	//FragColor = vec4(vec3(0.0f), 1.0);

}