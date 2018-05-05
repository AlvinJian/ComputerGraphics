#version 330 core

in  vec2 texCoord;
out vec4 fColor;

uniform sampler2D texture;

void main() 
{ 
    fColor = texture2D( texture, texCoord );
    // fColor = vec4(1.0, 0.0, 0.0, 1.0);
}