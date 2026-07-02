#version 330 core

layout (location = 0) in vec3 vPos;
layout (location = 1) in vec3 vNormal;
layout (location = 2) in vec2 vTexCoords;
layout (location = 3) in float vShininess;
layout (location = 4) in mat4 vModel;

out VS_OUT
{
	vec3 normal;
	vec3 fragPos;
	vec2 texCoords;
	float shininess;
	mat4 model;
} vs_out;

uniform mat4 view, projection;

void main()
{
    gl_Position = projection * view * vModel * vec4(vPos, 1.0);

	vs_out.normal = vNormal;
	vs_out.fragPos = (vModel * vec4(vPos, 1.0)).xyz;
	vs_out.texCoords = vTexCoords;
	vs_out.shininess = vShininess;
	vs_out.model = vModel;
}