#version 430 core

in vec4 ourPosition;
in vec3 ourNormal;
in vec2 outUV;
out vec4 outColor;

uniform sampler2D texture;
uniform vec3 lightPos;
uniform float lightStrength;

struct DirLight {
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
uniform DirLight dirLight;

struct PointLight {    
    vec3 position;
    
    float constant;
    float linear;
    float quadratic;  

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};  
#define NR_POINT_LIGHTS 4  
uniform PointLight pointLights[NR_POINT_LIGHTS];

struct Light {
	vec3 ambient;
	vec3 diffuse;
	vec3 position;
};

struct SpecularLight {
	vec3 ambient;
	vec3 diffuse;
	vec3 position;
	vec3 specular;
};
uniform vec3 materialAmbient;
uniform vec3 materialDiffuse;
uniform vec3 materialSpecular;
uniform float materialShine;

uniform vec3 lightDir;

#define LIGHTS_NO 1
#define SPEC_LIGHTS_NO 1

uniform vec3 cameraPos;

layout (std140, binding = 1) uniform specLights
{
	SpecularLight specLightBlock[SPEC_LIGHTS_NO];
};

layout (std140, binding = 2) uniform lights
{
	Light lightBlock[LIGHTS_NO];
};

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), materialShine);
    // combine results
    vec3 ambient  = light.ambient  * vec3(texture(texture, outUV));
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(texture, outUV));
    vec3 specular = light.specular * spec * vec3(texture(texture, outUV));
    return (ambient + diffuse + specular);
} 

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), materialShine);
    // attenuation
    float distance    = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + 
  			     light.quadratic * (distance * distance));    
    // combine results
    vec3 ambient  = light.ambient  * vec3(texture(texture, outUV));
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(texture, outUV));
    vec3 specular = light.specular * spec * vec3(texture(texture, outUV));
    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
} 


void main()
{	
	// vec3 Light_Ambient = specLightBlock[0].ambient;
	// vec3 Light_Diffuse = specLightBlock[0].diffuse;
	// vec3 Light_Position = specLightBlock[0].position;
	// ustawienie domyslnego koloru fragmentu
	// mozna tutaj uwzglednic tekstury i inne parametry
	vec3 objectColor = vec3(texture(texture, outUV));

	vec3 viewDir = normalize(cameraPos - vec3(ourPosition));

	// obliczenie kata pomiedzy wektorem lightDir oraz wektorem normalnym
	// wartosc kata okresla pod jakim katem padaja promienie
	vec3 lightDirection = vec3(0.f);
	float lightCoeff = 0.f; 

	float resultDiffuse = 0.f;

	vec3 result = vec3(0.f);
	lightDirection = normalize(lightPos - vec3(ourPosition));
	if (acos(dot(lightDir, lightDirection) / length(lightDir)) < 0.2){
		vec3  reflectDir = vec3(0.f);
		float specularCoeff = 0.f;
		vec3  resultSpecular = vec3(0.f);
		float specularCoef = 0.f;

		vec3 specularPart = vec3(0.f);
		lightCoeff = max(dot(ourNormal, lightDir), 0.0f);
		resultDiffuse = lightCoeff * (5.f / (1.f + length(lightPos.xyz - ourPosition.xyz) * length(lightPos.xyz - ourPosition.xyz)));
		
		reflectDir = reflect(-lightDir, ourNormal);
		specularCoef = pow(max(dot(viewDir, reflectDir), 0.0f), materialShine);
		resultSpecular = vec3(specularCoeff);
		specularPart = resultSpecular;

		result += 50.f * (vec3(resultDiffuse) + (pow(max(dot(viewDir, reflectDir), 0.0f), materialShine)*materialSpecular)) * objectColor;
	}
	// result += vec3(pow(max(dot(viewDir, reflectDir), 0.0f), 2.f)) * objectColor;
	// outColor = vec4(result, texture(texture, outUV).w);
	// (length(viewDir.xyz) * length(viewDir.xyz))
	outColor = vec4(objectColor / (length(cameraPos - vec3(ourPosition)) / 5.f + 1), texture(texture, outUV).w) + lightStrength * vec4(result, 0.f);
	// if (acos(dot(lightDir, lightDirection) / length(lightDir)) < 0.2){
	// 	outColor = vec4(1.f,1.f,1.f,1.f);
	// }
}
