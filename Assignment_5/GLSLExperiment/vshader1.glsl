#version 330 core

layout (location = 0) in vec4 vPosition;
layout (location = 1) in vec2 vTexCoord;
out vec2 texCoord;

uniform mat4 Projection;

void main() 
{
    // We only need the s and t values of the texture coordinate
    texCoord = vTexCoord;
    gl_Position = Projection * vPosition;
}