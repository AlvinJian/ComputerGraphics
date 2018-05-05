#version 150

uniform mat4 projectionMatrix;
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 orthoMatrix;

in vec4 vPosition;
in vec4 vColor;

out vec4 color;

void main()
{
    gl_Position = projectionMatrix * viewMatrix * modelMatrix * orthoMatrix * vPosition;
    color = vColor;
}