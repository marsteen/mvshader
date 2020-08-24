#version 410

#ifdef GL_ES
precision highp float;
#endif

//
// Fragment Textur Shader
//

in vec2 vTextVary;
out vec4 outputColor;
uniform sampler2D uTexture;

void main()
{
    outputColor = texture(uTexture, vTextVary);
}