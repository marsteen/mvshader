#version 300 es
//
// Vertex Shader
//

in vec4  aVertAttrib;
in vec2  aTextAttrib;
out vec2 vTextVary;

void main()
{
    vTextVary = aTextAttrib;
    gl_Position = aVertAttrib;
}
