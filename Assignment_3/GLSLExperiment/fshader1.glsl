#version 150

uniform vec4 lightPosition;
uniform vec3 lightDirection;
uniform float lightAngle;
uniform vec4 AmbientProduct, DiffuseProduct, SpecularProduct;
uniform float Shininess;

uniform int shadingMode;

// per-fragment interpolated values from the vertex shader
in vec3 fNormal;
in vec3 fPosition;
in vec4 fViewPosition;

flat in vec4 flatColor;

out vec4  fColor;

void main() 
{
    if (shadingMode == 0)
    {
        fColor = flatColor;
    }
    else
    {
        // Normalize the input lighting vectors
        vec3 norm = normalize(fNormal);
        vec3 spotToEye = normalize(fViewPosition.xyz - fPosition);
        vec3 spotToLight = normalize(lightPosition.xyz - fPosition);
        // vec3 H = normalize( L + E );
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
            fColor = ambient + diffuse + specular; // + interpolatedColor * 0.2;
            fColor.a = 1.0;
        }
        else
        {
            fColor = ambient;
            fColor.a = 1.0;
        }
    }
}
