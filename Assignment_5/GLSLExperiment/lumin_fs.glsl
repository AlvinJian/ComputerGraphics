#version 330 core

in  vec2 texCoord;
out vec4 fColor;

uniform sampler2D texture;

void main()
{
    const vec3 luminFactor = vec3(0.2125, 0.7154, 0.0721);
    vec3 origColor = texture2D(texture, texCoord).rgb;
    float luminance = dot(origColor, luminFactor);
    fColor = vec4(luminance, luminance, luminance, 1.0);
}