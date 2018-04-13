#version 330 core

in  vec2 texCoord;
out vec4 fColor;

uniform sampler2D texture;

void main() {
    ivec2 ires = textureSize(texture, 0);
    float ResS = float(ires.s);
    float ResT = float(ires.t);

    vec2 stp0 = vec2(1.0/ResS, 0.0);
    vec2 stpp = vec2(1.0/ResS, 1.0/ResT);

    vec3 c00 = texture2D(texture, texCoord).rgb;
    vec3 cp1p1 = texture2D(texture, texCoord + stpp).rgb;

    vec3 diffs = c00 - cp1p1;
    float maxch = max(diffs.r, max(diffs.g, diffs.b));

    float gray = clamp(maxch+0.5, 0.0, 1.0);
    vec4 grayVer = vec4(gray, gray, gray, 1.0);
    vec4 colorVer = vec4(gray * c00, 1.0);
    fColor = mix(grayVer, colorVer, 0.17);
}