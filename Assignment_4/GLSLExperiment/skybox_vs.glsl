#version 330 core
in vec4 vPosition;

uniform mat4 projectionMatrix;
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 orthoMatrix;

out vec3 TexCoords;

void main()
{
    vec4 vpos = orthoMatrix * vPosition;
    TexCoords = vpos.xyz;
    gl_Position = projectionMatrix * viewMatrix * modelMatrix * vpos;
}