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

// hashes by IQ

// 1d, 2d and 3d hashing functions, used for generating random numbers.
// based on coordinates in order to then generate terrain.
// The hashing functions are a lower dimensional version
// of the 3D variant.
float hash( in float x )
{
    vec3 p = vec3( x, 1.0, 1.0 );
    
    p = 50.0*fract( p*0.3183099 + vec3( 0.71, 0.113, 0.419 ) );
    return -1.0+2.0*fract( p.x*p.y*p.z*( p.x + p.y + p.z ) );
}

// Still the same exact thing, all of those could probably
// be optimized for their respective dimension, but it
// worked well enough.
float hash( in vec2 x )
{
    vec3 p = vec3( x, 1.0 );
    
    p = 50.0*fract( p*0.3183099 + vec3( 0.71, 0.113, 0.419 ) );
    return -1.0+2.0*fract( p.x*p.y*p.z*( p.x + p.y + p.z ) );
}

// The actual 3D version of the hashing function, those
// will later be used for noise function.
float hash( in vec3 x )
{
    vec3 p = x;
    
    p = 50.0*fract( p*0.3183099 + vec3( 0.71, 0.113, 0.419 ) );
    return -1.0+2.0*fract( p.x*p.y*p.z*( p.x + p.y + p.z ) );
}

// 2D and 3D noise surface functions.
// used for constructing fractional brownian motion surfaces.
float noise( in vec2 x )
{
    vec3 p = vec3( floor(x), 0.0 );
    vec3 w = vec3( fract(x), 0.0 );

    vec3 u = w*w*w*(w*(w*6.0-15.0)+10.0);

    float a = hash( p + vec3( 0, 0, 0 ) );
    float b = hash( p + vec3( 1, 0, 0 ) );
    float c = hash( p + vec3( 0, 1, 0 ) );
    float d = hash( p + vec3( 1, 1, 0 ) );
    float e = hash( p + vec3( 0, 0, 1 ) );
    float f = hash( p + vec3( 1, 0, 1 ) );
    float g = hash( p + vec3( 0, 1, 1 ) );
    float h = hash( p + vec3( 1, 1, 1 ) ); 

    float k0 =                               a;
    float k1 =                           b - a;
    float k2 =                           c - a;
    float k3 =                           e - a;
    float k4 =                   a - b - c + d;
    float k5 =                   a - c - e + g;
    float k6 =                   a - b - e + f;
    float k7 = - a + b + c - d + e - f - g + h;

    return float( -1.0+2.0*(k0 + k1*u.x + k2*u.y + k3*u.z + k4*u.x*u.y + k5*u.y*u.z + k6*u.z*u.x + k7*u.x*u.y*u.z) );
}

float noise( in vec3 x )
{
    vec3 p = floor(x);
    vec3 w = fract(x);

    vec3 u = w*w*w*(w*(w*6.0-15.0)+10.0);

    float a = hash( p + vec3( 0, 0, 0 ) );
    float b = hash( p + vec3( 1, 0, 0 ) );
    float c = hash( p + vec3( 0, 1, 0 ) );
    float d = hash( p + vec3( 1, 1, 0 ) );
    float e = hash( p + vec3( 0, 0, 1 ) );
    float f = hash( p + vec3( 1, 0, 1 ) );
    float g = hash( p + vec3( 0, 1, 1 ) );
    float h = hash( p + vec3( 1, 1, 1 ) ); 

    float k0 =                               a;
    float k1 =                           b - a;
    float k2 =                           c - a;
    float k3 =                           e - a;
    float k4 =                   a - b - c + d;
    float k5 =                   a - c - e + g;
    float k6 =                   a - b - e + f;
    float k7 = - a + b + c - d + e - f - g + h;

    return float( -1.0+2.0*(k0 + k1*u.x + k2*u.y + k3*u.z + k4*u.x*u.y + k5*u.y*u.z + k6*u.z*u.x + k7*u.x*u.y*u.z) );
}
// analytic derivatives of the 2D and 3D surface noise functions.
// used for assigning colors to terrain based on steepness.
vec3 noiseD( in vec2 x )
{
    vec3 p = vec3( floor(x), 0.0 );
    vec3 w = vec3( fract(x), 0.0 );

    vec3 u = w*w*w*(w*(w*6.0-15.0)+10.0);
    vec3 du =   30.0*w*w*(w*(w-2.0)+1.0);

    float a = hash( p + vec3( 0, 0, 0 ) );
    float b = hash( p + vec3( 1, 0, 0 ) );
    float c = hash( p + vec3( 0, 1, 0 ) );
    float d = hash( p + vec3( 1, 1, 0 ) );
    float e = hash( p + vec3( 0, 0, 1 ) );
    float f = hash( p + vec3( 1, 0, 1 ) );
    float g = hash( p + vec3( 0, 1, 1 ) );
    float h = hash( p + vec3( 1, 1, 1 ) ); 

    float k0 =                               a;
    float k1 =                           b - a;
    float k2 =                           c - a;
    float k3 =                           e - a;
    float k4 =                   a - b - c + d;
    float k5 =                   a - c - e + g;
    float k6 =                   a - b - e + f;
    float k7 = - a + b + c - d + e - f - g + h;

    return vec3(2.0* du * vec3( k1 + k4*u.y + k6*u.z + k7*u.y*u.z,
                                k2 + k5*u.z + k4*u.x + k7*u.z*u.x,
                            k3 + k6*u.x + k5*u.y + k7*u.x*u.y ) );
}

vec3 noiseD( in vec3 x )
{
    vec3 p = floor(x);
    vec3 w = fract(x);

    vec3 u = w*w*w*(w*(w*6.0-15.0)+10.0);
    vec3 du =   30.0*w*w*(w*(w-2.0)+1.0);

    float a = hash( p + vec3( 0, 0, 0 ) );
    float b = hash( p + vec3( 1, 0, 0 ) );
    float c = hash( p + vec3( 0, 1, 0 ) );
    float d = hash( p + vec3( 1, 1, 0 ) );
    float e = hash( p + vec3( 0, 0, 1 ) );
    float f = hash( p + vec3( 1, 0, 1 ) );
    float g = hash( p + vec3( 0, 1, 1 ) );
    float h = hash( p + vec3( 1, 1, 1 ) ); 

    float k0 =                               a;
    float k1 =                           b - a;
    float k2 =                           c - a;
    float k3 =                           e - a;
    float k4 =                   a - b - c + d;
    float k5 =                   a - c - e + g;
    float k6 =                   a - b - e + f;
    float k7 = - a + b + c - d + e - f - g + h;

    return vec3(2.0* du * vec3( k1 + k4*u.y + k6*u.z + k7*u.y*u.z,
                                k2 + k5*u.z + k4*u.x + k7*u.z*u.x,
                            k3 + k6*u.x + k5*u.y + k7*u.x*u.y ) );
}

// fractional brownian motion based noise modification with d octaves for 2D and 3D noise. 
// used for generating more realistic terrain.
float fbm( in vec2 p, in int d )
{
    float n = 0.0;
    
    for( int i = 0; i < d; i++ )
    {
    	n += pow( 1.5, -float( i ) )*noise( p*( pow( 2.14, float( i ) ) ) );
    }
    
    return n;
}

float fbm( in vec3 p, in int d )
{
    float n = 0.0;
    
    for( int i = 0; i < d; i++ )
    {
    	n += pow( 1.5, -float( i ) )*noise( p*( pow( 2.14, float( i ) ) ) );
    }
    
    return n;
}

// fractional brownian motion based noise analytic derivative modification with d octaves for 2D and 3D noise/
// used for getting the derivative of the realistic terrain.
vec3 fbmD( in vec2 p, in int d )
{
    vec3 n = vec3(0.0);
    
    for( int i = 0; i < d; i++ )
    {
    	n += pow( 1.5, -float( i ) )*noiseD( p*( pow( 2.14, float( i ) ) ) );
    }
    
    return n;
}

vec3 fbmD( in vec3 p, in int d )
{
    vec3 n = vec3(0.0);
    
    for( int i = 0; i < d; i++ )
    {
    	n += pow( 1.5, -float( i ) )*noiseD( p*( pow( 2.14, float( i ) ) ) );
    }
    
    return n;
}

// warped fractional brownian motion surface.
// works by re-entering input into the fbm.
float wFbm( in vec2 p, in int w, in int d )
{
    vec2 x = vec2( fbm( p, d ), fbm( p*4.568934, d ) );
    
    for( int i = 0; i < w; i++ )
    {
    	float t1 = 0.0;
    	float t2 = 0.0;
        
    	t1 =          fbm( x, 1 );
        t2 = fbm( x*4.568934, 1 );
        
        x = vec2( t1, t2 );
    }
    
    return float( x );
}

float wFbm( in vec3 p, in int w, in int d )
{
    vec2 x = vec2( fbm( p, d ), fbm( p*4.568934, d ) );
    
    for( int i = 0; i < w; i++ )
    {
    	float t1 = 0.0;
    	float t2 = 0.0;
        
    	t1 =          fbm( vec3( x, 1.0 ), d );
        t2 = fbm( vec3( x*4.568934, 1.0 ), d );
        
        x = vec2( t1, t2 );
    }
    
    return float( x );
}

float wFbmD( in vec2 p, in int w, in int d )
{
    vec2 x = vec2( fbmD( p, d ).x, fbmD( p*4.568934, d ).y );
    
    for( int i = 0; i < w; i++ )
    {
    	float t1 = 0.0;
    	float t2 = 0.0;
        
    	t1 =          fbmD( x, 1 ).x;
        t2 = fbmD( x*4.568934, 1 ).y;
        
        x = vec2( t1, t2 );
    }
    
    return float( x );
}

float wFbmD( in vec3 p, in int w, in int d )
{
    vec2 x = vec2( fbmD( p, d ).x, fbmD( p*4.568934, d ).y );
    
    for( int i = 0; i < w; i++ )
    {
    	float t1 = 0.0;
    	float t2 = 0.0;
        
    	t1 =          fbmD( vec3( x, 1.0 ), d ).x;
        t2 = fbmD( vec3( x*4.568934, 1.0 ), 1 ).y;
        
        x = vec2( t1, t2 );
    }
    
    return float( x );
}


#define ZOOM 1.0
#define BOUND 1.0
#define DARKNESS 1.0

void mainImage( out vec4 fragColor, in vec2 fragCoord )
{
    // zooming and positioning
    vec2 uv = ( ( fragCoord/iResolution.xy + iTime / 4.0 )/ZOOM + 1.0 );
	
    // coloring
    vec3 col = vec3( wFbm( vec3( uv, iTime / 10.0 ), 2, 2 ) + BOUND)/ DARKNESS;
	
    // tinting
    col *= vec3( 0.7, 0.2, 0.4 );
    col += 0.8;
    
    // Output to screen
    fragColor = vec4( col ,1.0 );
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


