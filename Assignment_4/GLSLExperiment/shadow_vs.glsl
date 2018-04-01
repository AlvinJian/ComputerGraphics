#version 150

uniform mat4 projectionMatrix;
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 shadowProjMatrix;
uniform mat4 orthoMatrix;

in vec4 vPosition;

void main()
{
    gl_Position = projectionMatrix * viewMatrix * shadowProjMatrix * modelMatrix * orthoMatrix * vPosition;
}