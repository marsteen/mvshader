//
// Vertex Shader
//

attribute vec4  aVertAttrib;
attribute vec2  aTextAttrib;
varying vec2    vTextVary;

void main()
{
    vTextVary = aTextAttrib;
    gl_Position  = aVertAttrib;
}
