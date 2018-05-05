#version 330 core

in  vec2 texCoord;
out vec4 fColor;

uniform sampler2D texture;

void main() {
    const float rmax = 0.48;
    const float rho = 1.8; 
    const vec2 texCenter = vec2(0.5, 0.5);
    vec2 delta = texCoord - texCenter;
    float r = sqrt(delta.s * delta.s + delta.t * delta.t);
    float z = sqrt(rmax * rmax - r * r);
    float betax = (1 - 1/rho) * asin( delta.s/sqrt(delta.s * delta.s + z * z) );
    float betay = (1 - 1/rho) * asin( delta.t/sqrt(delta.t * delta.t + z * z) );

    float _x = 0.0;
    if (r <= rmax) {
        _x = z * betax;
    }
    float _y = 0.0;
    if (r <= rmax) {
        _y = z * betay;
    }

    vec2 texCoordPrime = vec2(texCoord.s + _x, texCoord.t + _y);
    fColor = vec4(texture2D(texture, texCoordPrime).rgb, 1.0);
}