#version 330 core

struct Light
{
	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

in VS_OUT
{
	vec3 normal;
	vec3 fragPos;
	vec2 texCoords;
	float shininess;
	mat4 model;
} vs_in;

out vec4 FragColor;

uniform mat4 view, projection;
uniform vec3 viewPos;
uniform Light light;
uniform sampler2D matDiffuse;
uniform sampler2D matSpecular;

void main()
{
	mat3 normal_matrix = transpose(inverse(mat3(vs_in.model)));
	vec3 normal = normalize(normal_matrix * vs_in.normal);

	// ambient lighting
	vec3 ambient = vec3(texture(matDiffuse, vs_in.texCoords)) * light.ambient;

	vec3 to_light = normalize(light.position - vs_in.fragPos);
	// store this result here becaues it will be used again for the specular light calculation
	float exposure = max(dot(to_light, normal), 0.0);

	// diffuse lighting
	vec3 diffuse = exposure * vec3(texture(matDiffuse, vs_in.texCoords)) * light.diffuse;

	// specular highlight
	vec3 specular;
	/*
	If exposure == 0, then the fragment's normal vector is facing away from the light source and there should be no specular highlight.
	If we don't check for this, specular highlights will be visible on faces that are facing away from the light source
	*/
	if (exposure > 0)
	{
		vec3 view_dir = normalize(viewPos - vs_in.fragPos);
		vec3 reflect_dir = reflect(-to_light, normal);
		float spec = pow(max(dot(view_dir, reflect_dir), 0.0), vs_in.shininess);
		specular = spec * vec3(texture(matSpecular, vs_in.texCoords)) * light.specular;
	}

	vec3 result = ambient + diffuse + specular;

	FragColor = vec4(result, 1);
}