#version 150

uniform mat4 projection_matrix;
uniform mat4 modelViewMatrix;
uniform mat4 orth_matrix;

uniform int shadingMode;

// for calculating flat shading
uniform vec4 lightPosition;
uniform vec3 lightDirection;
uniform float lightAngle;
uniform vec4 AmbientProduct, DiffuseProduct, SpecularProduct;
uniform float Shininess;

in  vec4 vPosition;
in  vec4 vColor;
in vec3 vNormal;

flat out vec4 flatColor;

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
  gl_Position = projection_matrix * modelViewMatrix * orthPos;

  if (shadingMode == 0)
  {
    // flat shading
            // Normalize the input lighting vectors
        vec4 normal4 = vec4(vNormal.xyz, 1.0);
        vec3 _normal3 = (modelViewMatrix * normal4).xyz;
        vec3 norm = normalize(_normal3);
        vec3 vPos = (modelViewMatrix * orthPos).xyz;
        vec3 spotToEye = normalize(-1.0 * vPos);
        vec3 spotToLight = normalize(lightPosition.xyz - vPos);
        vec3 reflect = normalize(-1.0 * spotToLight + 2.0 * dot(spotToLight, norm));
        vec3 lightNorm = normalize(lightDirection);

        vec4 ambient = AmbientProduct;
        float halfAngle = radians(lightAngle/2.0);

        if ( dot(-1.0 * spotToLight, lightNorm) > cos(halfAngle) )
        {
            float Kd = max(dot(spotToLight, norm), 0.0);
            vec4 diffuse = Kd*DiffuseProduct;

            // float Ks = pow(max(dot(N, H), 0.0), Shininess);
            float Ks = pow(max(dot(spotToEye, reflect), 0.0), Shininess);
            vec4 specular = Ks*SpecularProduct;
            // discard the specular highlight if the light's behind the vertex
            if( dot(spotToLight, norm) < 0.0 ) {
	            specular = vec4(0.0, 0.0, 0.0, 1.0);
            }
            flatColor = ambient + diffuse + specular; // + interpolatedColor * 0.2;
            flatColor.a = 1.0;
        }
        else
        {
            flatColor = ambient;
            flatColor.a = 1.0;
        }
  }
  else
  {
    vec4 normal4 = vec4(vNormal.xyz, 1.0);
    fNormal = (modelViewMatrix * normal4).xyz;
    fPosition = (modelViewMatrix * orthPos).xyz;
  }
}
