//fragment shader
#version 330

out vec4 color;  //color output variable

in vec4 vCol; //input variable for color from vertex shader
in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;

const int MAX_POINT_LIGHTS = 3;

struct Light
{
	vec3 color;
	float ambientIntensity;
	float diffuseIntensity;
};

struct DirectionalLight
{
	Light base;
	vec3 direction;
};

struct PointLight{
	Light base;
	vec3 position;
	float constant;
	float linear;
	float exponent;
};

struct Material
{
	float specularIntensity;
	float shininess;
};

uniform sampler2D theTexture;
uniform int pointLightCount;

uniform DirectionalLight directionalLight;
uniform PointLight pointLights[MAX_POINT_LIGHTS];

uniform Material material;

uniform vec3 eyePosition;

vec4 calcLightByDirection(Light light, vec3 direction)
{
	vec4 ambientColor = vec4(light.color, 1.0f) * light.ambientIntensity;
	
	float diffuseFactor = max(dot(normalize(Normal), normalize(direction)), 0.0f);
	vec4 diffuseColor = vec4(light.color, 1.0f) * light.diffuseIntensity * diffuseFactor;

	vec4 specularColor = vec4(0, 0, 0, 0);

	if(diffuseFactor > 0.0f)
	{
		vec3 fragToEye = normalize(eyePosition - FragPos);
		vec3 reflectedVertex = normalize(reflect(direction, normalize(Normal)));

		float specularFactor = dot(fragToEye, reflectedVertex);

		if(specularFactor > 0.0f)
		{
			specularFactor = pow(specularFactor, material.shininess);
			specularColor = vec4(light.color * material.specularIntensity * specularFactor, 1.0f);
		}
	}

	return (ambientColor + diffuseColor + specularColor);

}

vec4 calcDirectionalLight()
{
	return calcLightByDirection(directionalLight.base, directionalLight.direction);
}

vec4 calcPointLights()
{
	vec4 totalColor = vec4(0,0,0,0);

	for(int i = 0; i < pointLightCount; i++)
	{
		vec3 direction = FragPos - pointLights[i].position;
		float distance1 = length(direction);
		direction = normalize(direction);

		vec4 color = calcLightByDirection(pointLights[i].base, direction);
		float attenuation = pointLights[i].exponent*pow(distance1, 2) + pointLights[i].linear * distance1 + pointLights[i].constant;

		totalColor += (color / attenuation);
	}

	return totalColor;
}

void main()
{
	vec4 finalColor = calcDirectionalLight();
	finalColor += calcPointLights();

	color = texture(theTexture, TexCoord) * finalColor;

}