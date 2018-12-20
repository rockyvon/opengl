# version 330 core

struct Material{
	sampler2D diffuse;
	vec3 specular;
	int shininess;
};

struct Light{
	vec3 direction;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

in vec3 fragment_color;
in vec3 fragment_position;
in vec2 fragment_vertex_coor;
in vec3 fragment_normal;

out vec4 FragColor;

uniform sampler2D texture1;
uniform sampler2D texture2;

uniform vec3 light_color;

//环境光

//散射光

//高光
uniform vec3 view_position;

uniform Material material;
uniform Light light;


void main()
{
	//FragColor = mix(texture(texture1, fragment_vertex_coor), texture(texture2, fragment_vertex_coor), 0.5);//多纹理混合
	//FragColor = texture(texture1, fragment_vertex_coor);//单纹理
	//FragColor = vec4(fragment_color, 1.0);//纯颜色

	//vec3 light_dir = light.position - fragment_position;
	vec3 light_dir = -light.direction;

	vec3 ambient = light.ambient * vec3(texture(texture1, fragment_vertex_coor));
	vec3 diffuse = light.diffuse * (max(dot(normalize(light_dir) , normalize(fragment_normal)), 0.0) * vec3(texture(texture1, fragment_vertex_coor)));
	vec3 specular = light.specular * (pow(max(dot(normalize(reflect(-light_dir, fragment_normal)), normalize(view_position - fragment_position)), 0.0), material.shininess) * material.specular);
	FragColor = vec4(ambient + diffuse + specular, 1.0);//冯氏光照
	
}