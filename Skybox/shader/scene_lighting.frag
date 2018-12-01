#version 330 core

#define MAX_DIRE_LICHT 1
#define MAX_POINT_LICHT 1
#define MAX_SPOT_LICHT 1

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

struct LightDirectional{
	bool enable;
	vec3 direction;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

};

struct LightPoint{
	bool enable;
	vec3 position;

	float c;
	float l;
	float q;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

};

struct LightSpot 
{
	bool enable;
	vec3 direction;
	float cutoff;

	vec3 position;

	float c;
	float l;
	float q;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

in VFData vfdata;

out vec4 FragColor;

uniform bool hasNormal;
uniform bool blinn;
uniform vec3 camera_position;
uniform Material material;
uniform LightDirectional lds[MAX_DIRE_LICHT];
uniform LightPoint lps[MAX_POINT_LICHT];
uniform LightSpot lss[MAX_SPOT_LICHT];

vec4 calculateLight(vec4 ambient, vec4 diffuse, vec4 specular);
vec3 calculate_light_directional(LightDirectional light, vec3 normal, vec3 view_direcion, vec3 amb_color, vec3 dif_color, vec3 spe_color);
vec3 calculate_light_point(LightPoint light, vec3 normal, vec3 view_direcion, vec3 amb_color, vec3 dif_color, vec3 spe_color);
vec3 calculate_light_spot(LightSpot light, vec3 normal, vec3 view_direcion, vec3 amb_color, vec3 dif_color, vec3 spe_color);


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
	float gamma = 2.2;
	vec3 result =vec3(0.0);
	vec3 normal_normalize = normalize(vfdata.normal);
	vec3 view_direcion = normalize(camera_position - vfdata.position);

	for(int i = 0; i < MAX_DIRE_LICHT; i++)
	{
		if(lds[i].enable)
			result += calculate_light_directional(lds[i], normal_normalize, view_direcion, vec3(ambient), vec3(diffuse), vec3(specular));
	}
	
	for(int i = 0; i < MAX_POINT_LICHT; i++)
	{
		if(lps[i].enable)
			result += calculate_light_point(lps[i], normal_normalize, view_direcion, vec3(ambient), vec3(diffuse), vec3(specular));
	}
	for(int i = 0; i < MAX_SPOT_LICHT; i++)
	{
		if(lss[i].enable)
			result += calculate_light_spot(lss[i], normal_normalize, view_direcion, vec3(ambient), vec3(diffuse), vec3(specular));
	}
	return vec4(pow(result, vec3(1.0/gamma)), 1.0);
}

vec3 calculate_light_directional(LightDirectional light, vec3 normal, vec3 view_direcion, vec3 amb_color, vec3 dif_color, vec3 spe_color)
{
	vec3 ambient = light.ambient * amb_color;
	vec3 diffuse = light.diffuse * max(dot(normalize(-light.direction), normal), 0) * dif_color;
	vec3 specular;
	if(blinn)
		specular = light.specular * pow(max(dot(normalize(normalize(-light.direction) + normalize(view_direcion)), view_direcion), 0), material.shininess) * spe_color;
	else
		specular = light.specular * pow(max(dot(normalize(reflect(light.direction, normal)), view_direcion), 0), material.shininess) * spe_color;
	
	return ambient + diffuse + specular;
}

vec3 calculate_light_point(LightPoint light, vec3 normal, vec3 view_direcion, vec3 amb_color, vec3 dif_color, vec3 spe_color)
{
	vec3 light_target = vfdata.position - light.position;
	vec3 ambient = light.ambient * amb_color;
	vec3 diffuse = light.diffuse * max(dot(normalize(-light_target), normal), 0) * dif_color;
	vec3 specular;
	if(blinn)
		specular= light.specular * pow(max(dot(normalize(normalize(-light_target) + normalize(view_direcion)), view_direcion), 0), material.shininess) * spe_color;
	else
		specular= light.specular * pow(max(dot(normalize(reflect(normalize(light_target), normal)), view_direcion), 0), material.shininess) * spe_color;
	float dis = length(light_target);
	float attenuation  = 1/(light.c + light.l * dis + light.q * pow(dis, 2)); 
	return (ambient + diffuse + specular);
}

vec3 calculate_light_spot(LightSpot light, vec3 normal, vec3 view_direcion, vec3 amb_color, vec3 dif_color, vec3 spe_color)
{
	vec3 light_target = vfdata.position - light.position;
	float theta = dot(normalize(light_target), normalize(light.direction));
	float phi = cos(light.cutoff);
	if(theta > phi)
	{
		vec3 ambient = light.ambient * amb_color;
		vec3 diffuse = light.diffuse * max(dot(normalize(-light_target), normal), 0) * dif_color;
		vec3 specular;
		if(blinn)
			specular= light.specular * pow(max(dot(normalize(normalize(-light_target) + normalize(view_direcion)), view_direcion), 0), material.shininess) * spe_color;
		else
			specular= light.specular * pow(max(dot(normalize(reflect(normalize(light_target), normal)), view_direcion), 0), material.shininess) * spe_color;
		float dis = length(light_target);
		float attenuation  = 1/(light.c + light.l * dis + light.q * pow(dis, 2)); 
		
		return attenuation * (ambient + diffuse + specular);
	}else{
		return vec3(0.0);
	}
}