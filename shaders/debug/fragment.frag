#version 330 core

in VS_OUT
{
	vec4 vColor;
	vec3 normal;
	vec3 fragPos;
} vs_in;

out vec4 FragColor;

void main()
{
	vec3 normalColor;

	if (vs_in.normal.x < 0)
		normalColor.yz += vec2(1, 1) * -vs_in.normal.x;
	else
		normalColor.x += vs_in.normal.x;

	if (vs_in.normal.y < 0)
		normalColor.xz += vec2(1, 1) * -vs_in.normal.y;
	else
		normalColor.y += vs_in.normal.y;

	if (vs_in.normal.z < 0)
		normalColor.xy += vec2(1, 1) * -vs_in.normal.z;
	else
		normalColor.z += vs_in.normal.z;

	normalColor = normalize(normalColor);

	FragColor = vec4(normalColor, 1);
}