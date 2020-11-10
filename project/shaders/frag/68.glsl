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

float sphereSDF(vec3 p, float r) {
    return length(p) - r;
}

float boxSDF(vec3 a, vec3 b, float r) {
    return length(max(abs(a)-b, 0.)) - r;
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

float fl2(vec3 p, float scene)
{
    float direction = noise(p+vec3(1.,iTime*0.75,1.0));
    float height = 0.1; //dont bend! ascend! by going above about 0.3
    float f = (scene + direction*height);
 	float a = abs(f);
    return min(10.-f, a);
}

vec3 rayDirection(float fieldOfView, vec2 size, vec2 fragCoord) {
    vec2 xy = fragCoord - size / 2.0;
    float z = size.y / tan(radians(fieldOfView) / 2.0);
    return normalize(vec3(xy, -z));
}

float scene(in vec3 pos) {
	float box = boxSDF(pos,vec3(3.5,0.25,3.5), .2);
    float s = sphereSDF(pos, 2.);
    return fl2(pos,min(box, s));
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

#define KUROSAWA 0
void mainImage( out vec4 fragColor, in vec2 fragCoord )
{
    vec3 eye = vec3(0, 2.1, 10.);
    vec3 rayDir = rayDirection(90., iResolution.xy, fragCoord);
    vec4 p = raymarch(eye, rayDir);
	float glow = p.w;
	
    vec3  lig = normalize( vec3(-0.0, -5., 0.) );
    vec3  lig2 = normalize( vec3(-0.0, 5., 4.) );
    vec3 hal = lig - rayDir;
    
    vec3 normal = calcNormal(p.xyz);
    
    float dif = clamp( dot( normal, lig ), 0.0, 1. );
    float dif2 = clamp( dot( normal, lig2 ), 0.0, 1. );
    vec3 mate = vec3(1.2);
#if KUROSAWA 
    vec3 lig1Colour = vec3(1.);
    vec3 col = mate*dif*lig1Colour;
    vec3 col2 = mate*dif2*lig1Colour;
    
    fragColor = mix(vec4(col, 1.), vec4(0.3), pow(glow*2.,4.));
  	fragColor = mix(fragColor, vec4(col2, 1.), 0.5);
#else    
    vec3 lig1Colour = vec3(.9,0.4,0.1);
    vec3 lig2Colour = vec3(.9,0.1,0.1);
    vec3 col = mate*dif*lig1Colour;
    vec3 col2 = mate*dif2*lig2Colour;
    
    fragColor = mix(vec4(col, 1.), vec4(1.,.5,.1, 1.), pow(glow*2.,4.));
  	fragColor = mix(fragColor, vec4(col2, 1.), 0.5);
#endif
    

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


