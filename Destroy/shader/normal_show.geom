# version 330 core

layout(triangles) in;
layout(line_strip, max_vertices = 6) out;

in vec3 geometry_normal[];

const float LENGTH = 0.001;

void thorn(int index)
{
	gl_Position = gl_in[index].gl_Position;
	EmitVertex();
	gl_Position = gl_in[index].gl_Position + vec4(normalize(geometry_normal[index]) * LENGTH, 1.0);
	EmitVertex();
	EndPrimitive();
}

void main()
{
	for(int i = 0; i < 3; i++)
	{
		
	}
	thorn(0);
	
}