# version 330 core
#define MAX_DIRE_LICHT 1
#define MAX_POINT_LICHT 1
#define MAX_SPOT_LICHT 1

struct Material{
	sampler2D diffuse0;
	sampler2D diffuse1;
	sampler2D diffuse2;
	sampler2D diffuse3;
	sampler2D specular0;
	sampler2D specular1;
	sampler2D specular2;
	sampler2D specular3;
	int shininess;
};

struct LightDirectional{
	vec3 direction;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

};

struct LightPoint{
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

in struct GFData{
	vec3 position;
	vec3 normal;
	vec2 vertex_coor;
	vec4 light_coord_position;
}gfdata;

out vec4 FragColor;

uniform Material material;
uniform LightDirectional lds[MAX_DIRE_LICHT];
uniform LightPoint lps[MAX_POINT_LICHT];
uniform LightSpot lss[MAX_SPOT_LICHT];
uniform vec3 camera_position;
uniform bool blinn;

vec3 calculate_light_directional(LightDirectional light, vec3 normal, vec3 view_direcion, vec3 dif_color, vec3 spe_color)
{
	vec3 ambient = light.ambient * dif_color;
	vec3 diffuse = light.diffuse * max(dot(normalize(-light.direction), normal), 0) * dif_color;
	vec3 specular;
	if(blinn)
		specular = light.specular * pow(max(dot(normalize(normalize(-light.direction) + normalize(view_direcion)), view_direcion), 0), material.shininess) * spe_color;
	else
		specular = light.specular * pow(max(dot(normalize(reflect(light.direction, normal)), view_direcion), 0), material.shininess) * spe_color;
	
	return ambient + diffuse + specular;
}

vec3 calculate_light_point(LightPoint light, vec3 normal, vec3 view_direcion, vec3 light_target, vec3 dif_color, vec3 spe_color)
{
	vec3 ambient = light.ambient * dif_color;
	vec3 diffuse = light.diffuse * max(dot(normalize(-light_target), normal), 0) * dif_color;
	vec3 specular;
	if(blinn)
		specular= light.specular * pow(max(dot(normalize(normalize(-light_target) + normalize(view_direcion)), view_direcion), 0), material.shininess) * spe_color;
	else
		specular= light.specular * pow(max(dot(normalize(reflect(normalize(light_target), normal)), view_direcion), 0), material.shininess) * spe_color;
	float dis = length(light_target);
	float attenuation  = 1/(light.c + light.l * dis + light.q * pow(dis, 2)); 
	return attenuation * (ambient + diffuse + specular);
}

vec3 calculate_light_point(LightPoint light, vec3 normal, vec3 view_direcion, vec3 dif_color, vec3 spe_color)
{
	vec3 light_target = gfdata.position - light.position;
	return calculate_light_point(light, normal, view_direcion, light_target, dif_color, spe_color);
}

vec3 calculate_light_spot(LightSpot light, vec3 normal, vec3 view_direcion, vec3 dif_color, vec3 spe_color)
{
	vec3 light_target = gfdata.position - light.position;
	float theta = dot(normalize(light_target), normalize(light.direction));
	float phi = cos(light.cutoff);
	if(theta > phi)
	{
		vec3 ambient = light.ambient * dif_color;
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

void main()
{
	float gamma = 2.2;
	vec3 result =vec3(0.0);
	vec3 normal_normalize = normalize(gfdata.normal);
	vec3 view_direcion = normalize(camera_position - gfdata.position);
	
	vec3 dif_color = vec3(texture(material.diffuse0, gfdata.vertex_coor));
	vec3 spe_color = vec3(texture(material.specular0, gfdata.vertex_coor));
	for(int i = 0; i < MAX_DIRE_LICHT; i++)
	{
		result += calculate_light_directional(lds[i], normal_normalize, view_direcion, dif_color, spe_color);
	}
	
	for(int i = 0; i < MAX_POINT_LICHT; i++)
	{
		result += calculate_light_point(lps[i], normal_normalize, view_direcion, dif_color, spe_color);
	}
	for(int i = 0; i < MAX_SPOT_LICHT; i++)
	{
		result += calculate_light_spot(lss[i], normal_normalize, view_direcion, dif_color, spe_color);
	}
	FragColor = vec4(pow(result, vec3(1.0/gamma)), 1.0);
	//FragColor = vec4(vec3(gl_FragCoord.z, 1 - gl_FragCoord.z, gl_FragCoord.z / 2), 1.0);

}



