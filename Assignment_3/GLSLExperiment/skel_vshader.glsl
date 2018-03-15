#version 150

uniform mat4 projection_matrix;
uniform mat4 modelViewMatrix;
uniform mat4 orth_matrix;

in vec4 vPosition;
in vec4 vColor;

out vec4 color;

void main()
{
    gl_Position = projection_matrix * modelViewMatrix * orth_matrix * vPosition;
    color = vColor;
}