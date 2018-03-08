#version 150

uniform vec4 lightPosition;
uniform vec3 lightDirection;
uniform float lightAngle;
uniform vec4 AmbientProduct, DiffuseProduct, SpecularProduct;
uniform float Shininess;

in  vec4  interpolatedColor;

// per-fragment interpolated values from the vertex shader
in  vec3 fN;
in  vec3 fL;
in  vec3 fE;

out vec4  fColor;

void main() 
{ 
    // fColor = interpolatedColor;

    // Normalize the input lighting vectors
    vec3 N = normalize(fN);
    vec3 E = normalize(fE);
    vec3 L = normalize(fL);
    vec3 H = normalize( L + E );
    vec3 R = normalize(-1.0 * L + 2.0 * dot(L, N));

    vec3 F = normalize(lightDirection);
    vec4 ambient = AmbientProduct;
    float halfAngle = radians(lightAngle/2.0);
    if ( dot(-1.0 * L, F) > cos(halfAngle) )
    {
        float Kd = max(dot(L, N), 0.0);
        vec4 diffuse = Kd*DiffuseProduct;

        // float Ks = pow(max(dot(N, H), 0.0), Shininess);
        float Ks = pow(max(dot(E, R), 0.0), Shininess);
        vec4 specular = Ks*SpecularProduct;
        // discard the specular highlight if the light's behind the vertex
        if( dot(L, N) < 0.0 ) {
	        specular = vec4(0.0, 0.0, 0.0, 1.0);
        }
        fColor = ambient + diffuse + specular; // + interpolatedColor * 0.2;
        fColor.a = 1.0;
    }
    else
    {
        fColor = ambient;
        fColor.a = 1.0;
    }
}
