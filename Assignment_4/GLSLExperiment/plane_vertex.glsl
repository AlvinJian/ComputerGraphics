#version 150

uniform mat4 projectionMatrix;
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;

in  vec4 vPosition;
in  vec3 vNormal; 
in  vec2 vTexCoord;

out vec2 texCoord;

void main() 
{
    texCoord = vTexCoord;
    gl_Position = projectionMatrix * viewMatrix * modelMatrix * vPosition;
}