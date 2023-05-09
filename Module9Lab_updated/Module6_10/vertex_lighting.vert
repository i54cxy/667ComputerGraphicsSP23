#version 150

// Per vertex lighting - vertex shader. Outputs a varying (intepolated) color to the 
// fragment shader
smooth out vec3 fvPosition;
smooth out vec3 fvNormal;
smooth out vec2 fvTexCoord;

// Incoming vertex and normal attributes
in vec3 vertexPosition;		// Vertex position attribute
in vec3 vertexNormal;		// Vertex normal attribute
in vec2 texCoord;           // Vertex texture coordinate

uniform mat4 pvm;		// Composite projection, view, model matrix

// Simple shader for per vertex lighting.
void main()
{
    fvPosition = vertexPosition;
    fvNormal = vertexNormal;
    fvTexCoord = texCoord;
    // Convert position to clip coordinates and pass along
    gl_Position = pvm * vec4(vertexPosition, 1.0);
}
