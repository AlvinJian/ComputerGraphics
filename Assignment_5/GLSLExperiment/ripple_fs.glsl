#version 330 core

in  vec2 texCoord;
out vec4 fColor;

uniform sampler2D texture;

void main()
{
    ivec2 ires = textureSize(texture, 0);
    float resS = float(ires.s);
    float resT = float(ires.t);

    float taox = 120.0 / resS;
    float taoy = 250.0 / resT;
    float ax = 15.0 / resS;
    float ay = 30.0 / resT;

    float s = texCoord.s + ax * sin(radians(360.0) * texCoord.t / taox);
    float t = texCoord.t + ay * sin(radians(360.0) * texCoord.s / taoy);
    vec2 pt = vec2(s, t);
    fColor = texture2D(texture, pt);
}