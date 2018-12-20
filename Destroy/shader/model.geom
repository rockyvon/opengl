# version 330 core

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

in struct VFData{
	vec3 position;
	vec3 normal;
	vec2 vertex_coor;
	vec4 light_coord_position;
}vfdata[];

out struct GFData{
	vec3 position;
	vec3 normal;
	vec2 vertex_coor;
	vec4 light_coord_position;
}gfdata;

uniform int explode_times;

const float LENGTH = 0.01;

vec3 calcute_normal(vec3 p1, vec3 p2, vec3 p3){
	vec3 v1 = p2 - p1;
	vec3 v2 = p2 - p3;
	return normalize(cross(v1, v2));
}

vec4 explode(vec4 position, vec3 normal)
{
	return position + vec4(normal * LENGTH * explode_times, 0.0);
}

void main()
{
	vec3 normal = calcute_normal(gl_in[0].gl_Position.xyz, gl_in[1].gl_Position.xyz, gl_in[2].gl_Position.xyz);
	for(int i = 0; i < 3; i++)
	{
		gl_Position = explode(gl_in[i].gl_Position, normal);
		gfdata.position = gl_Position.xyz;
		gfdata.normal = vfdata[i].normal;
		gfdata.vertex_coor = vfdata[i].vertex_coor;
		gfdata.light_coord_position = vfdata[i].light_coord_position;
		EmitVertex();
	}
	EndPrimitive();
	
}