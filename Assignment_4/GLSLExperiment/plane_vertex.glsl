#version 150

uniform mat4 projectionMatrix;
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;

in  vec3 vPosition;
in  vec3 vNormal; 
in  vec2 vTexCoord;

out vec2 texCoord;

void main() 
{
    texCoord = vTexCoord;
    vec4 vPos4 = vec4(vPosition, 1.0);
    gl_Position = projectionMatrix * viewMatrix * modelMatrix * vPos4;
}