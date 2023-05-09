#version 150

out vec4 fragColor;

// Incoming color - interpolated
smooth in vec3 fvPosition;
smooth in vec3 fvNormal;
smooth in vec2 fvTexCoord;

// Uniforms for material properties
uniform vec4   materialAmbient;
uniform vec4   materialDiffuse;
uniform vec4   materialSpecular;
uniform vec4   materialEmission;
uniform float  materialShininess;

// Global lighting environment ambient intensity
uniform vec4  globalLightAmbient;

// Global camera position
uniform vec3  cameraPosition;

// Uniforms for matrices
uniform mat4 pvm;		// Composite projection, view, model matrix
uniform mat4 modelMatrix;	// Modeling  matrix
uniform mat4 normalMatrix;	// Normal transformation matrix

// Uniform to constrian the number of lights the application uses
uniform int numLights;

// texture samplers
uniform int useTexture;
uniform sampler2D texture;

// Structure for a light source. Allow up to 8 lights. No attenuation or spotlight
// support yet.
const int MAX_LIGHTS = 8; 
struct LightSource
{
    int  enabled;
    vec4 position;
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
    vec3 spotDirection;
    float spotCutoff;
    float spotExponent;
};
uniform LightSource lights[MAX_LIGHTS]; 
   
void main()
{
    // Transform normal and position to world coords. 
    vec3 N = normalize(vec3(normalMatrix * normalize(vec4(fvNormal, 0.0))));
    vec3 v = vec3((modelMatrix * vec4(fvPosition, 1.0)));

    // Construct a unit length vector from the vertex to the camera 
    vec3 V = normalize(cameraPosition - v);

    // Iterate through all lights to determine the illumination striking this vertex
    vec4 ambient  = vec4(0.0);
    vec4 diffuse  = vec4(0.0);
    vec4 specular = vec4(0.0);
    vec4 spotlight = vec4(0.0);
    float attenuation = 1.0f;
    for (int i = 0; i < numLights; i++)
    {
        if (lights[i].enabled == 1)
        {
            vec3 L;
            if (lights[i].position.w == 0.0) {
                // Directional Light
                L = normalize(lights[i].position.xyz);
            } else {
                // Construct a vector from the vertex to the light source. Normalize that vector (L)
                L = normalize(lights[i].position.xyz - v);
            }

            // Spotlight
            if (lights[i].spotCutoff > 0.0f) {
                float spotEffect = max(dot(lights[i].spotDirection, -L), 0);
                if (spotEffect >= cos(lights[i].spotCutoff)) {
                    attenuation *= pow(spotEffect, lights[i].spotExponent);
                } else {
                    attenuation = 0.0;
                }
            }

            // Add the light source ambient contribution
            ambient += lights[i].ambient * attenuation;

            // Determine dot product of normal with L. If < 0 the light is not 
            // incident on the front face of the surface.
            float nDotL = dot(N, L);
            if (nDotL > 0.0)
            {
                // Add diffuse contribution of this light source
                diffuse  += lights[i].diffuse * attenuation * nDotL;
      
                // Construct the halfway vector
                vec3 H = normalize(L + V);
      
                // Add specular contribution (if N dot H > 0)
                float nDotH = dot(N, H);
                if (nDotH > 0.0) {
                    specular += lights[i].specular * attenuation * pow(nDotH, materialShininess);
                }
            }

        }
    }

    // Compute color. Emmission + global ambient contribution + light sources ambient, diffuse,
    // and specular contributions
    fragColor = materialEmission + globalLightAmbient * materialAmbient +
            ambient  * materialAmbient + diffuse  * materialDiffuse + specular * materialSpecular;

    if (useTexture == 1) {
        fragColor = texture2D(texture, fvTexCoord) * fragColor;
    }
}
