#version 330
uniform mat4 modelViewProjectionMatrix;
uniform vec2 pos;

layout( location = 0 ) in vec2 inPosition;


void main()
{
    gl_Position = modelViewProjectionMatrix * vec4( inPosition + pos, 0.0, 1.0 );
}
