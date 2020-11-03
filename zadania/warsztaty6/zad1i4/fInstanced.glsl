#version 430 core

// in vec4 ourPosition;
// in vec3 ourNormal;
in vec2 outUV;
in vec4 ourPosition;
in vec3 ourNormal;
uniform vec3 cameraPos;
out vec4 outColor;
uniform vec3 lightPos;
uniform float lightStrength;
uniform vec3 lightDir;

uniform sampler2D texture;


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
		specularCoef = pow(max(dot(viewDir, reflectDir), 0.0f), 32.f);
		resultSpecular = vec3(specularCoeff);
		specularPart = resultSpecular;

		result += 50.f * (vec3(resultDiffuse) + (pow(max(dot(viewDir, reflectDir), 0.0f), 32.0f)*vec3(0.7f, 0.7f, 0.7f))) * objectColor;
	}
	// result += vec3(pow(max(dot(viewDir, reflectDir), 0.0f), 2.f)) * objectColor;
	// outColor = vec4(result, texture(texture, outUV).w);
	// (length(viewDir.xyz) * length(viewDir.xyz))
	outColor = vec4(objectColor / (length(cameraPos - vec3(ourPosition)) / 5.f + 1), texture(texture, outUV).w) + lightStrength * vec4(result, 0.f);


	// vec4 tex = texture(texture, outUV);
	// outColor = vec4(tex.xyz / (length(cameraPos - vec3(ourPosition)) / 5.f + 1), tex.w);
}
