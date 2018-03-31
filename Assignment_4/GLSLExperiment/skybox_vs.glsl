#version 330 core
in vec4 vPosition;

uniform mat4 projectionMatrix;
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;

out vec3 TexCoords;

void main()
{
    TexCoords = vPosition.xyz;
    vec4 pos = projectionMatrix * viewMatrix * modelMatrix * vPosition;
    gl_Position = pos;
}