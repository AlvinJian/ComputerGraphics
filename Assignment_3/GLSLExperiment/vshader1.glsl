#version 150

uniform mat4 projection_matrix;
uniform mat4 model_matrix;
// uniform float y_twist;
uniform mat4 orth_matrix;

in  vec4 vPosition;
in  vec4 vColor;
out vec4 interpolatedColor;

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
  // float f = length(orthPos);
  // float y_tw_rad = radians(f*y_twist);
  // float c = cos(y_tw_rad);
  // float s = sin(y_tw_rad);
  // mat4 twist;
  // twist[0] = vec4(c, 0.0, s, 0.0);
  // twist[1] = vec4(0.0, 1.0, 0.0, 0.0);
  // twist[2] = vec4(-1.0*s, 0.0, c, 0.0);
  // twist[3] = vec4(0.0, 0.0, 0.0, 1.0);
  // mat4 twist_t = transpose(twist);

  // gl_Position = projection_matrix * model_matrix * twist_t * orthPos;
  gl_Position = projection_matrix * model_matrix * orthPos;
  interpolatedColor = vColor;
} 
