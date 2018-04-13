#version 330 core

in  vec2 texCoord;
out vec4 fColor;

uniform sampler2D texture;

void main()
{
    const vec2 texCenter = vec2(0.5, 0.5);
    const float alpha = 0.55;
    const float rmax = 0.78;
    vec2 delta = texCoord - texCenter;
    float r = sqrt(delta.s * delta.s + delta.t * delta.t);
    float beta = atan(delta.t, delta.s) + alpha * (rmax - r) / rmax;

    float s = texCoord.s;
    if (r <= rmax)
    {
        s = texCenter.s + r * cos(beta);
    }
    float t = texCoord.t;
    if (r <= rmax)
    {
        t = texCenter.t + r * sin(beta);
    }
    vec2 pt = vec2(s, t);
    fColor = texture2D(texture, pt);
}