#version 300 es
//
// Vertex Shader
//

in vec4  aVertAttrib;
in vec2  aTextAttrib;
out vec2 vTextVary;
uniform float uScale;

void main()
{
    vTextVary = aTextAttrib;
    vec4 vert = aVertAttrib;
    vert.xy *= uScale;
    gl_Position = vert;
}
