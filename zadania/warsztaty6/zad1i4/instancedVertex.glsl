#version 330

layout( location = 0 ) in vec4 inPosition;
layout( location = 1 ) in vec3 inNormal;
layout( location = 2 ) in vec2 inUV;
layout( location = 3 ) in vec3 transVec;

// out vec4 ourPosition;
// out vec3 ourNormal;
out vec2 outUV;
out vec4 ourPosition;
out vec3 ourNormal;

uniform mat4 projMatrix;
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;


void main()
{
	// gl_Position = (projMatrix * viewMatrix * modelMatrix  * (inPosition + transVec));
	gl_Position = (projMatrix * viewMatrix * (inPosition + vec4(transVec, 0.f)));
	// gl_Position += vec4(transVec, 0.f);
    // gl_Position = vec4(transVec, 1.f);
	// ourPosition = inPosition;
	// ourNormal = vec3(modelMatrix * vec4(inNormal, 0.f));
	ourPosition = inPosition + vec4(transVec, 0.f);
	ourNormal = inNormal;
	outUV = inUV;
}