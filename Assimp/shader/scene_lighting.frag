#version 330 core

struct VFData{
	vec3 position;
	vec3 color;
	vec3 normal;
	vec2 texCoords;
};

struct Material{
	vec4 emission;
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
	float shininess;
	bool hasColor;
	bool hasTexture;
	sampler2D difTex;
	sampler2D speTex;
};

in VFData vfdata;

out vec4 FragColor;

uniform bool hasNormal;
uniform Material material;

vec4 calculateLight(vec4 ambient, vec4 diffuse, vec4 specular);

void main()
{
	if(hasNormal)
	{
		//光照计算
		if(material.hasTexture)
		{
			vec4 diffuse = texture(material.difTex, vfdata.texCoords);
			vec4 specular = texture(material.speTex, vfdata.texCoords);
			FragColor = calculateLight(diffuse, diffuse, specular);
		}else
		{
			FragColor = calculateLight(material.ambient, material.diffuse, material.specular);
		}
		
	}else
	{
		//无光照
		FragColor = vec4(vfdata.color, 1.0);
	}
	
}

vec4 calculateLight(vec4 ambient, vec4 diffuse, vec4 specular)
{
	return ambient + diffuse + specular;
}