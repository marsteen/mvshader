#version 410
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

// Created by inigo quilez - iq/2013
// License Creative Commons Attribution-NonCommercial-ShareAlike 3.0 Unported License.

// Some geometric orbit traps in the Mandelbrot set. More info:
//
// https://iquilezles.org/www/articles/ftrapsgeometric/ftrapsgeometric.htm



// antialiasing level (squared)
#define AA 3

void mainImage( out vec4 fragColor, in vec2 fragCoord )
{
    vec3 col = vec3(0.0);
    
    for( int m=0; m<AA; m++ )
    for( int n=0; n<AA; n++ )
    {
        
    vec2 p = (2.0*(fragCoord+vec2(float(m),float(n))/float(AA))-iResolution.xy) / iResolution.y;

	float zoo = 1.0/(350.0 - 250.0*sin(0.25*iTime-0.3));
	
	vec2 cc = vec2(-0.533516,0.526141) + p*zoo;

	vec2 t2c = vec2(-0.5,2.0);
	t2c += 0.5*vec2( cos(0.13*(iTime-10.0)), sin(0.13*(iTime-10.0)) );
		
    // iterate
    vec2 z  = vec2(0.0);
    vec2 dz = vec2(0.0);
	float trap1 = 0.0;
	float trap2 = 1e20;
	float co2 = 0.0;
    for( int i=0; i<150; i++ )
    {
		// z' -> 2·z·z' + 1
        dz = 2.0*vec2(z.x*dz.x-z.y*dz.y, z.x*dz.y + z.y*dz.x ) + vec2(1.0,0.0);
			
        // z -> z² + c			
        z = cc + vec2( z.x*z.x - z.y*z.y, 2.0*z.x*z.y );
			
        // trap 1
		float d1 = abs(dot(z-vec2(0.0,1.0),vec2(0.707)));
		float ff = 1.0-smoothstep(0.6,1.4,d1);
		co2 += ff;
		trap1 += ff*d1;

		//trap2
		trap2 = min( trap2, dot(z-t2c,z-t2c) );

        if( dot(z,z)>1024.0 ) break;
    }

    // distance, d(c) = |z|·log|z|/|z'|
	float d = sqrt( dot(z,z)/dot(dz,dz) )*log(dot(z,z));
	
	float c1 = pow( clamp( 2.00*d/zoo,    0.0, 1.0 ), 0.5 );
	float c2 = pow( clamp( 1.5*trap1/co2, 0.0, 1.0 ), 2.0 );
	float c3 = pow( clamp( 0.4*trap2, 0.0, 1.0 ), 0.25 );

	vec3 col1 = 0.5 + 0.5*sin( 3.0 + 4.0*c2 + vec3(0.0,0.5,1.0) );
	vec3 col2 = 0.5 + 0.5*sin( 4.1 + 2.0*c3 + vec3(1.0,0.5,0.0) );
	col += 2.0*sqrt(c1*col1*col2);
    }
	col /= float(AA*AA);
    
	fragColor = vec4( col, 1.0 );
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


