#version 330 core
out vec4 fColor;

in vec3 TexCoords;

uniform samplerCube skybox;

void main()
{
    fColor = textureCube(skybox, TexCoords);
}