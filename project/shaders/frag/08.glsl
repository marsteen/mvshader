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

float rand(vec2 co){
    return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}

void mainImage( out vec4 fragColor, in vec2 fragCoord )
{
    vec3 col = vec3(0);
    
    float scale = 10.;
    vec2 uv = fragCoord/iResolution.y + iTime * .05;
    
    vec2 id = floor(uv * scale);
    uv = fract(uv * scale) - .5;
    //col += vec3(abs(uv), 0.);
    
    if (rand(id) < .5 * (1. + sin(iTime))) uv.x *= -1.;
    //if (rand(id) < .5) uv.x *= -1.;
    
    float d = distance(uv, vec2(.5, .5) * sign(uv.x + uv.y + .0001));
    col += smoothstep(.02, -.02, abs(d - .5) - .05);
    
    //col += 1. - smoothstep(-.01, .01, abs(uv.x + uv.y + (uv.x + uv.y < 0. ? .5 : -.5)) - .1);
    //col += floor(uv.x + uv.y + 1.) - .5;
    
    bool border = max(abs(uv.x), abs(uv.y)) > .48;
    //if (border) col = vec3(1, 0, 0);
    
    fragColor = vec4(col,1.0);
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


