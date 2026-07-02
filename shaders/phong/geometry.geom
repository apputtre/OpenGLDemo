#version 330 core
layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in VS_OUT
{
	vec4 vColor;
	vec3 normal;
	vec3 fragPos;
} vs_in[];

out VS_OUT
{
	vec4 vColor;
	vec3 normal;
	vec3 fragPos;
} vs_out;

void main()
{
	vs_out.vColor = vs_in[0].vColor;
	vs_out.normal = vs_in[0].normal;
	vs_out.fragPos = vs_in[0].fragPos;
	gl_Position = gl_in[0].gl_Position;
	EmitVertex();

	vs_out.vColor = vs_in[1].vColor;
	vs_out.normal = vs_in[1].normal;
	vs_out.fragPos = vs_in[1].fragPos;
	gl_Position = gl_in[1].gl_Position;
	EmitVertex();

	vs_out.vColor = vs_in[2].vColor;
	vs_out.normal = vs_in[2].normal;
	vs_out.fragPos = vs_in[2].fragPos;
	gl_Position = gl_in[2].gl_Position;
	EmitVertex();

	EndPrimitive();
}