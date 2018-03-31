#version 150

in  vec2 texCoord;
out vec4 fColor;

uniform sampler2D planeTexture;

void main() 
{ 
    fColor = texture2D( planeTexture , texCoord );
}