#version 330 core

in  vec2 texCoord;
out vec4 fColor;

uniform sampler2D texture;

void main()
{    const vec3 luminFactor = vec3(0.2125, 0.7154, 0.0721);

    ivec2 ires = textureSize(texture, 0);
    float resS = float(ires.s);
    float resT = float(ires.t);

    vec3 iColor = texture2D(texture, texCoord).rgb;
    vec2 stp0 = vec2(1.0/resS, 0.0);
    vec2 st0p = vec2(0.0, 1.0/resT);
    vec2 stpp = vec2(1.0/resS, 1.0/resT);
    vec2 stpn = vec2(1.0/resS, -1.0/resT);

    float i00 = dot(texture2D(texture, texCoord).rgb, luminFactor);
    float inn = dot(texture2D(texture, texCoord - stpp).rgb, luminFactor);
    float ipp = dot(texture2D(texture, texCoord + stpp).rgb, luminFactor);
    float inp = dot(texture2D(texture, texCoord - stpn).rgb, luminFactor);
    float ipn = dot(texture2D(texture, texCoord + stpn).rgb, luminFactor);
    float ip0 = dot(texture2D(texture, texCoord + stp0).rgb, luminFactor);
    float in0 = dot(texture2D(texture, texCoord - stp0).rgb, luminFactor);
    float i0p = dot(texture2D(texture, texCoord + st0p).rgb, luminFactor);
    float i0n = dot(texture2D(texture, texCoord - st0p).rgb, luminFactor);

    float h = -1.0 * inp - 2.0 * i0p - 1.0 * ipp + 1.0 * inn + 2.0 * i0n + 1.0 * ipn;
    float v = -1.0 * inn - 2.0 * in0 - 1.0 * inp + 1.0 * ipn + 2.0 * ip0 + 1.0 * ipp;

    float mangn = sqrt(h * h + v * v);
    const float magThres = 0.8;
    if (mangn > magThres)
    {
        fColor = vec4(0.0, 0.0, 0.0, 1.0);
    }
    else
    {
        float uQuantize = 10.0;
        float Quantize = 10.0;
        iColor.rgb *= uQuantize;
        // iColor += vec3(0.5, 0.5, 0.5);
        ivec3 intrgb = ivec3( round(iColor.r), round(iColor.g), round(iColor.b) );
        iColor.rgb = vec3( intrgb )/ Quantize;
        fColor = vec4(iColor.rgb, 1.0);
    }

}