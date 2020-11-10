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



mat4 rotationX( in float angle ) {
	return mat4(	1.0,		0,			0,			0,
			 		0, 	cos(angle),	-sin(angle),		0,
					0, 	sin(angle),	 cos(angle),		0,
					0, 			0,			  0, 		1);
}

mat4 rotationY( in float angle ) {
	return mat4(	cos(angle),		0,		sin(angle),	0,
			 				0,		1.0,			 0,	0,
					-sin(angle),	0,		cos(angle),	0,
							0, 		0,				0,	1);
}

mat4 rotationZ( in float angle ) {
	return mat4(	cos(angle),		-sin(angle),	0,	0,
			 		sin(angle),		cos(angle),		0,	0,
							0,				0,		1,	0,
							0,				0,		0,	1);
}

float sphereSDF(vec3 p, float r) {
    return length(p) - r;
}

float boxSDF(vec3 a, vec3 b, float r) {
    return length(max(abs(a)-b, 0.)) - r;
}

float torus( vec3 p, vec2 t )
{
  vec2 q = vec2(length(p.xz)-t.x,p.y);
  return length(q)-t.y;
}

float noise(vec3 p) //Thx to Las^Mercury
{
	vec3 i = floor(p);
	vec4 a = dot(i, vec3(1., 57., 21.)) + vec4(0., 57., 21., 78.);
	vec3 f = cos((p-i)*acos(-1.))*(-.5)+.5;
	a = mix(sin(cos(a)*a),sin(cos(1.+a)*(1.+a)), f.x);
	a.xy = mix(a.xz, a.yw, f.y);
	return mix(a.x, a.y, f.z);
}

float sphere(vec3 p, vec4 spr)
{
	return length(spr.xyz-p) - spr.w;
}

float flame(vec3 p, vec3 cone)
{
	float d = sphere(p*cone, vec4(.0,-1.,.0,1.));
	float fl = abs(d + (noise(p+vec3(1.,iTime*.75,1.0)) + noise(p*3.)*.5)*.25*(p.y)) ;
    return min(20.-d , fl );
}

vec3 rayDirection(float fieldOfView, vec2 size, vec2 fragCoord) {
    vec2 xy = fragCoord - size / 2.0;
    float z = size.y / tan(radians(fieldOfView) / 2.0);
    return normalize(vec3(xy, -z));
}

float sdCappedTorus(in vec3 p, in vec2 sc, in float ra, in float rb)
{
  p.x = abs(p.x);
  float k = (sc.y*p.x>sc.x*p.y) ? dot(p.xy,sc) : length(p.xy);
  return sqrt( dot(p,p) + ra*ra - 2.0*ra*k ) - rb;
}

const float MAX = 200.;

float opSmoothUnion( float d1, float d2, float k ) {
    float h = clamp( 0.5 + 0.5*(d2-d1)/k, 0.0, 1.0 );
    return mix( d2, d1, h ) - k*h*(1.0-h); }

float opSmoothSubtraction( float d1, float d2, float k ) {
    float h = clamp( 0.5 - 0.5*(d2+d1)/k, 0.0, 1.0 );
    return mix( d2, -d1, h ) + k*h*(1.0-h); }

float scene(in vec3 pos) {
	float box = boxSDF(pos,vec3(3.5,0.25,3.5), .2);
    
    mat4 r = rotationX(3.14/2.0);
    vec4 rotated = vec4(pos, 1.0)*r;
    float limit = 3.14;
    const float step = 0.04;
    float thicc = .65;
    float final = torus(rotated.xyz, vec2(1.2, thicc));
    for(float i = step; i < .63; i += step)
    {
        vec4 new = rotated*rotationZ(3.14/i);
    	//final = opSmoothUnion(final, torus(new.xyz, vec2(1.2f, thicc)), 0.07);
        final = min(final, torus(new.xyz, vec2(1.2, thicc)) );
    }
    
    {
    	vec3 toTheLeft = pos;
    	toTheLeft.x -= .8;
    	toTheLeft.z -= 2.;
    	float sphere = sphereSDF(toTheLeft, .4);
    	final = opSmoothSubtraction(sphere, final, 0.01);
    }
    
    {
    	vec3 toTheLeft = pos;
    	toTheLeft.x += .85;
    	toTheLeft.z -= 2.0;
    	float sphere = sphereSDF(toTheLeft, .4);
    	final = opSmoothSubtraction(sphere, final, 0.01);
    }
    
     {
    	vec3 toTheLeft = (vec4(pos, 1.)*rotationZ(3.14)).xyz;
    	toTheLeft.y += .1;
    	toTheLeft.z -= 1.7;
    	float sphere = sdCappedTorus(toTheLeft, vec2(0.2, 0.1),1., .2);
    	final = opSmoothSubtraction(sphere, final, 0.01);
    }

    {
        vec3 toTheLeft = (vec4(pos, 1.)*rotationZ(3.14)).xyz;
        final = opSmoothUnion(final, flame(toTheLeft, vec3(1., .4, 1.)), .07);
    }
    {
        vec3 toTheLeft = (vec4(pos, 1.)*rotationZ(3.14)).xyz;
        final = opSmoothUnion(final, flame(toTheLeft, vec3(1.75, .4, 1.75)), .07);
    }
    
    
    return final;
}

vec3 calcNormal( in vec3 pos )
{
    vec2 e = vec2(1.0,-1.0)*0.5773*0.0005;
    return normalize( e.xyy*scene( pos + e.xyy ) + 
					  e.yyx*scene( pos + e.yyx ) + 
					  e.yxy*scene( pos + e.yxy ) + 
					  e.xxx*scene( pos + e.xxx ) );
}

vec4 raymarch(vec3 org, vec3 dir)
{
	float d = 0.0, glow = 0.0, eps = 0.01;
	vec3  p = org;
	bool glowed = false;
	
	for(int i=0; i<64; i++)
	{
		d = scene(p) + eps;
		p += d * dir;
		if( d>eps )
		{
			if(scene(p) < 0.)
				glowed=true;
			if(glowed)
       			glow = float(i)/64.;
		}
	}
	return vec4(p,glow);
}

void mainImage( out vec4 fragColor, in vec2 fragCoord )
{

    vec3 eye = (vec4(0, 2.1, 10., 1)).xyz;//*rotationZ(iTime)).xyz;
    vec3 rayDir = rayDirection(90., iResolution.xy, fragCoord);
    vec4 p = raymarch(eye, rayDir);
	float glow = p.w;
	
    vec3  lig = normalize( vec3(-0.0, -5., 0.) );
    vec3  lig2 = normalize( vec3(-0.0, 5., 4.) );
    vec3 hal = lig - rayDir;
    
    vec3 normal = calcNormal(p.xyz);
    
    float dif = clamp( dot( normal, lig ), 0.0, 1. );
    float dif2 = clamp( dot( normal, lig2 ), 0.0, 1. );
    vec3 mate = vec3(0.3);
    vec3 col = mate*dif*4.*vec3(.9,0.4,0.1);
    vec3 col2 = mate*dif2*4.*vec3(.9,0.1,0.1);
    
    vec4 blah =  vec4(1.,.5,.1, 1.);//mix(vec3(0.), vec3(0.7,0.0,0.0), p.y*.04);
    fragColor = mix(vec4(col, 1.), blah, pow(glow*2.,4.));
    fragColor = mix(fragColor, vec4(col2, 1.), 0.5);
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


