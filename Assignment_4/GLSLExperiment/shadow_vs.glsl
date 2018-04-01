#version 150

uniform vec4 lightPosition;
uniform mat4 projectionMatrix;
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 orthoMatrix;
uniform float ground; 

in vec4 vPosition;

vec4 calcProjectionPoint(vec4 vertPos)
{
    vec3 deltaVec = (vertPos - lightPosition).xyz;
    float alpha = (-vertPos.y + ground) / deltaVec.y;
    return vec4(vertPos.x + alpha * deltaVec.x,
                ground,
                vertPos.z + alpha * deltaVec.z,
                1.0);
}

void main()
{
    vec4 worldPos = modelMatrix * orthoMatrix * vPosition;
    gl_Position = projectionMatrix * viewMatrix * calcProjectionPoint(worldPos);
}