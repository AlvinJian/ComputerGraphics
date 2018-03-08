#version 150

uniform mat4 projection_matrix;
uniform mat4 model_matrix;
uniform mat4 orth_matrix;
uniform vec4 lightPosition;

in  vec4 vPosition;
in  vec4 vColor;
in vec3 vNormal;

out vec4 interpolatedColor;

// output values that will be interpretated per-fragment
out vec3 fNormal;
out vec3 fPosition;

// PROTIP #3
// can extend to gl_Position = projection * camera * models * vertex
// you may wish to view camera motion as a transform on the projection (projection * camera) * models ...
// this could be thought of as moving the observer in space
// or you may view camera motion as transforming all objects in the world while the observer remains still
// projection * (camera * models) * ...
// notice mathematically there is no difference
// however in rare circumstances the order of the transform may affect the numerical stability
// of the overall projection

void main() 
{
  vec4 orthPos = orth_matrix * vPosition;
  gl_Position = projection_matrix * model_matrix * orthPos;
  interpolatedColor = vColor;

  vec4 normal4 = vec4(vNormal.xyz, 1.0);
  fNormal = (model_matrix * normal4).xyz;
  fPosition = (model_matrix * orthPos).xyz;
}
