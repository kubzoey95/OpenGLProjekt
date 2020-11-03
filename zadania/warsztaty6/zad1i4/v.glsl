#version 330 core

layout( location = 0 ) in vec4 inPosition;
layout( location = 1 ) in vec3 inNormal;
layout( location = 2 ) in vec2 inUV;

out vec4 ourPosition;
out vec3 ourNormal;
out vec2 outUV;

uniform mat4 projMatrix;
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;

void main()
{
	gl_Position = projMatrix * viewMatrix * modelMatrix * inPosition;
	ourPosition = modelMatrix * inPosition;
	ourNormal = vec3(modelMatrix * vec4(inNormal, 0.f));
	outUV = inUV;
}
