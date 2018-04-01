#version 330 core
out vec4 fColor;

in vec3 TexCoords;

uniform samplerCube skybox;
uniform int skyboxMode;

vec4 plainCube(vec3 point)
{
    vec4 blue = vec4(0.12, 0.12, 0.55, 1.0);
    vec4 gray = vec4(0.45, 0.45, 0.45, 1.0);

    if (abs(point.y) >= max(abs(point.x), abs(point.z)) && point.y < 0)
    {
        return gray;
    } else
    {
        return blue;
    }
}

void main()
{    
    if (skyboxMode == 1)
    {
        fColor = plainCube(TexCoords);
    }
    else
    {
        fColor = textureCube(skybox, TexCoords);
    }    
}