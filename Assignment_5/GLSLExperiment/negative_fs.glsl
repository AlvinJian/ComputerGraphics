#version 330 core

in  vec2 texCoord;
out vec4 fColor;

uniform sampler2D texture;

void main()
{
    vec3 origColor = texture2D(texture, texCoord).rgb;
    fColor = vec4(1.0 - origColor.r, 1.0 - origColor.g, 1.0 - origColor.b, 1.0);
}