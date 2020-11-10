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

// ----- Ray marching options ----- //
#define AA_SAMPLES 1 // antialisaing
// #define LOW_QUALITY // if your computer isn't powerful enough
#ifdef LOW_QUALITY
    #define MAX_STEPS 70
#else
    #define MAX_STEPS 150
#endif
#define MAX_DIST 50.
#define SURF_DIST 0.001
#define NORMAL_DELTA 0.0001

#define FBM_MAX_ITER 10

// ----- easingFunct -----//
float quadin(float t) { return t*t;}
float quadOut(float t) { return -t*(t-2.0);}
float cubicIn(float t) { return t*t*t;}
float cubicOut(float t) { return -t*t*t+1.0;}
float circleOut(float t) { return pow(1.0-(1.0-t)*(1.0-t), 0.5); }
float circleIn(float t) { return 1.0- pow(1.0-t*t, 0.5); }
float gauss(float t, float s) { return exp(-(t*t)/(2.*s*s)); }
// sub interpolation used in smoothstep
#define hermiteInter(t) t * t * (3.0 - 2.0 * t)

// ----- UsefulConstants ----- //
#define PI  3.14159265358979323846264338327

// ----- Useful functions ----- //
#define rot2(a) mat2(cos(a), -sin(a), sin(a), cos(a))
float maxComp(vec2 v) { return max(v.x , v.y); }
float maxComp(vec3 v) { return max(max(v.x , v.y), v.z); }
float cro(vec2 a,vec2 b) { return a.x*b.y - a.y*b.x; }
float map(float t, float a, float b) {return a + t * (b - a); } // considering that t is in [0-1]
float map(float t, float a, float b, float c, float d) { return c + (t - a) * (d - c) / (b - a); }
float mult(vec2 v) { return v.x*v.y; }
float mult(vec3 v) { return v.x*v.y*v.z; }
float sum(vec2 v) { return v.x+v.y; }
float sum(vec3 v) { return v.x+v.y+v.z; }
#define saturate(v) clamp(v, 0., 1.)


// ----- distance functions modifiers ----- //
// source: https://www.iquilezles.org/www/articles/smin/smin.htm

float smin( float a, float b, float k ) {
    float h = max(k-abs(a-b), 0.0)/k;
    return min(a, b) - h*h*0.25;
}

// polynomial smooth min (k = 0.1);
float polysmin( float a, float b, float k ) {
    float h = clamp( 0.5+0.5*(b-a)/k, 0.0, 1.0 );
    return mix( b, a, h ) - k*h*(1.0-h);
}

float Cubic( float a, float b, float k ) {
    float h = max( k-abs(a-b), 0.0)/k;
    return min(a, b) - h*h*h*k*(1.0/6.0);
}

float boxSDF(vec3 p, vec3 boxDim) {
    vec3 q = abs(p) - boxDim;
    return length(max(q, 0.0)) + min(maxComp(q), 0.0);
}

float subOp( float d1, float d2) { return max(-d1, d2); }

float planeSDF(vec3 p, vec4 plane) { return dot(p - plane.w, plane.xyz); }

vec3 arrayOp(vec3 p, vec3 offset) {
    return mod(p + 0.5 * offset, offset) - 0.5 * offset;
}

vec4 orb = vec4(0.);

//Apollonian Fractal Distance
float apollonian(vec3 p, float radius) {
	float scale = 1.0;
	p.xy = p.yx; // flip axis
	orb = vec4(1e20);
	for( int i=0; i<10;i++ ) {
		p = -1. + 2.0*fract(0.5*p+0.5);
		float r2 = dot(p,p)*1.;
		float k = radius/r2;
		orb = min( orb, vec4(abs(p),r2) );
		p     *= k;
		scale *= k;
	}
	
	return 0.25*abs(p.x)/scale;
	
}

// source: http://lolengine.net/blog/2013/07/27/rgb-to-hsv-in-glsl
// All components are in the range [0, 1], including hue.
vec3 rgb2hsv(vec3 c) {
    vec4 K = vec4(0.0, -1.0 / 3.0, 2.0 / 3.0, -1.0);
    vec4 p = mix(vec4(c.bg, K.wz), vec4(c.gb, K.xy), step(c.b, c.g));
    vec4 q = mix(vec4(p.xyw, c.r), vec4(c.r, p.yzx), step(p.x, c.r));

    float d = q.x - min(q.w, q.y);
    float e = 1.0e-10;
    return vec3(abs(q.z + (q.w - q.y) / (6.0 * d + e)), d / (q.x + e), q.x);
}

// All components are in the range [0, 1], including hue.
vec3 hsv2rgb(vec3 c)
{
    vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
    vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
    return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);
}

float scene(vec3 p) {
	// return sphereSDF(p, 1.0);
	float d = MAX_DIST;
	
	d= min(d, apollonian(p, 1. + 0.72+ sin(2.*PI*iTime/20.)*0.3));
	
	vec3 q = p;
	q.x = fract(-abs(q.x-1.));
	q.yz = mod(q.yz, 2.);
	float planeGrid = boxSDF(q-vec3(0.,1.,1.), vec3(0.02, 1., 1.));
	// d = subOp(planeGrid*100., d);
	
	return d;
}

// Iq's technique
// source: https://www.shadertoy.com/view/3lsSzf
float calcOcclusion(vec3 pos, vec3 nor) {
	float occ = 0.0;
    float sca = 1.0;
    for( int i=0; i<5; i++) {
        float h = 0.01 + 0.11*float(i)/4.0;
        vec3 opos = pos + h*nor;
        float d = scene(opos);
        occ += (h-d)*sca;
        sca *= 0.95;
    }
    return clamp( 1.0 - 2.0*occ, 0.0, 1.0 );
}

// source: https://iquilezles.org/www/articles/normalsSDF/normalsSDF.htm
vec3 getNormal(vec3 p) {
    const float h = NORMAL_DELTA;
	const vec2 k = vec2(1., -1.);
    return normalize( k.xyy * scene(p + k.xyy*h) + 
                      k.yyx * scene(p + k.yyx*h) + 
                      k.yxy * scene(p + k.yxy*h) + 
                      k.xxx * scene(p + k.xxx*h) );
}

// return dist, marchingCount
vec2 rayMarching(vec3 O, vec3 D) { // ray origin and dir
    float t = 0.0;
    float marchingCount = 0.0;
    for(int i = 0; i < MAX_STEPS; i++) {
    	vec3 p = O + D * t;
        float d = scene(p);
        
        // If we are very close to the object, consider it as a hit and exit this loop
        if( t > MAX_DIST || abs(d) < SURF_DIST*0.99) break;
        t += d * 1.; // precision handling
        ++marchingCount;
    }
    if(t > MAX_DIST) t=-1.0;
    return vec2(t, marchingCount);
}

vec3 blendColor(float t, vec3 a, vec3 b) {
	return sqrt((1. - t) * pow(a, vec3(2.)) + t * pow(b, vec3(2.)));
}
    
vec3 render(vec3 O, vec3 D) { // ray origin and dir
	
	vec3 blue = vec3(2.,43.,58.)/255.0;
	vec3 orange = vec3(255.,160.,60.)/255.0;
    
    vec3 col = vec3(0.);
    
    vec2 rm = rayMarching(O, D);
    float mc = rm.y;
    float d = rm.x;
    if( d > 0. ) {
	    vec3 p = O + D * d;
	    vec3 normal = getNormal(p);

		// lighting
        vec3  backLight = vec3(  0.577, 0.577, -0.577 );
        vec3  keyLigh = vec3( -0.707, 0.000,  0.707 );
        float key = saturate(dot(keyLigh, normal));
        float bac = saturate(dot(backLight, normal));
        float amb = (0.7+0.3*normal.y);
		float ao = calcOcclusion(p, normal);
		ao = mix(ao, pow( saturate(orb.w*2.0), 1.2 ), 0.1);
		
        vec3 brdf  = 3.*vec3(0.2)*amb*ao;
        brdf += 0.5*vec3(1., 1., 0.8)*key*ao;
        brdf += 0.5*vec3(1., 1., 0.8)*bac*ao;
		// brdf *= 2.5;
		
        // material		
        vec3 rgb = vec3(0.2);
        rgb = blendColor(clamp(2.0*orb.y,0.0,1.0), rgb, orange);
        rgb = mix(rgb, blue, pow(clamp(1.0-2.0*orb.z,0.0,1.0),2.0) );
        // color // brdf
        col = blendColor(0.5,rgb, brdf)*exp(-0.2*d);
        col = rgb*brdf*exp(-0.2*d);
        // col = vec3(orb.x*orb.x);
        col += blue* pow(mc, 0.8)/100.;
    } else {
    	col = blue;
    }
    
	return vec3(saturate(col));
}

vec3 computeCamDir(vec2 uv, vec3 camPos, vec3 camUp, vec3 lookAtPos) {
	vec3 camVec = normalize(lookAtPos - camPos);
	vec3 sideNorm = normalize(cross(camUp, camVec));
	vec3 upNorm = cross(camVec, sideNorm);
	vec3 worldFacing = (camPos + camVec);
	vec3 worldPix = worldFacing + uv.x * sideNorm + uv.y * upNorm;
	return normalize(worldPix - camPos);
}

void mainImage(out vec4 fragColor, in vec2 fragCoord) {

	vec2 uv = (fragCoord - .5*iResolution.xy)/iResolution.y;
    uv *= 1.5; // zoom

	vec3 O = vec3(0.5*cos(iTime/5.)+1., 1. + sin(iTime/2.)*0.3, 0.5*sin(iTime/10.)); // origin

	vec3 finalColor = vec3(0.);
#if AA_SAMPLES > 1
    for (float i = 0.0; i < AA_SAMPLES; i++) {
    	for (float j = 0.0; j < AA_SAMPLES; j++) {
	        vec2 deltaUV = (vec2(i, j) / float(AA_SAMPLES) *2.0 - 1.0) / iResolution.y;
	        uv += deltaUV;
#endif
			vec3 D = computeCamDir(uv, O, vec3(0.,1.,0.), O + vec3(cos(iTime/4.), 0.2*cos(iTime/3.), sin (iTime/4.))); // dir
	    	finalColor += render(O, D);
#if AA_SAMPLES > 1
    	}
    }
    finalColor /= float(AA_SAMPLES * AA_SAMPLES); // Average samples
#endif
    
    
    // gamma corection
    finalColor = pow(finalColor, vec3(1./2.2));
    
    vec3 hsv = rgb2hsv(finalColor);
    hsv.x += -0.01;
    hsv.y *= 1.8; // saturate
    hsv.z *= 1.3;
    finalColor = hsv2rgb(saturate(hsv));
    
    // color grading
    finalColor *= vec3(1.07 ,0.92, 0.95);
    
	fragColor = vec4(finalColor,1.);
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


