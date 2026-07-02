#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec4 aColor;
layout (location = 3) in mat4 model;

out VS_OUT
{
	vec4 vColor;
	vec3 normal;
	vec3 fragPos;
} vs_out;

uniform mat4 view, projection;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);

	vs_out.vColor = aColor;
	vs_out.normal = aNormal;
	vs_out.fragPos = (model * vec4(aPos, 1.0)).xyz;
}