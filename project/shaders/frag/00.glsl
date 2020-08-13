#version 300 es
//
// Fragment Shader
//
#ifdef GL_ES
precision highp float;
#endif

uniform vec3  iResolution;
uniform vec4  iMouse;
uniform float iTime;
in vec2 vTextVary;
out vec4 outputColor;

// ********** REPLACE THIS PART WITH YOUR SHADER **********
//
//            START
//
//*********************************************************

void mainImage( out vec4 fragColor, in vec2 fragCoord)
{
    fragColor = vec4(
        fragColor.x / iResolution.x,
        fragColor.y / iResolution.y,
        0.0,
        1.0);
}

//*********************************************************
//
//            END
//
//*********************************************************

void main()
{
    vec2 fragCoord = vTextVary * iResolution.xy;
    mainImage(outputColor, fragCoord); 
}


