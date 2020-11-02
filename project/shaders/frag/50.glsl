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


#define MAT_S5_BLOB 13.0
#define MAT_S5_FLOOR 14.0
#define MAT_S5_PILLAR 15.0


#define EPS 0.01



float sphere(vec3 p, float r)
{
	return length(p) - r;
}


float sdBox( vec3 p, vec3 b )
{
  vec3 d = abs(p) - b;
  return min(max(d.x,max(d.y,d.z)),0.0) +
         length(max(d,0.0));
}


float sdCylinder( vec3 p, vec3 c )
{
  return length(p.xz-c.xy)-c.z;
}


float udRoundBox( vec3 p, vec3 b, float r )
{
  return length(max(abs(p)-b,0.0))-r;
}



vec2 un(vec2 a, vec2 b)
{
	return a.x < b.x ? a : b;
}


vec2 scene(vec3 p, vec3 rd)
{
	float t = iTime;
	vec2 res = vec2(99999, -1);
	vec3 s = vec3(1);
	vec3 q = mod(p, s) - 0.5 * s;
	ivec3 n = ivec3(floor(p / s));//TODO! ivec3(trunc(p / s));
	
	float blob = sphere(p, 4.0);
	blob +=  0.15 * sin(- t * 2.0 + p.y * 0.75 ) +
	  	0.1 * sin( t * 1.5 + p.x*1.5) +
		0.2 * sin( t * 1.0 + p.z);
	res = un(res, vec2(blob, MAT_S5_BLOB));
	
	
	float roomDis = -sdBox(vec3(n), vec3(40.0, 15.0, 40.0));
	if (roomDis < 0.0) {
		float box = udRoundBox(q, vec3(0.4), 0.1);
		res = un(res, vec2(box, MAT_S5_FLOOR));
	} else {
		vec3 d = (s * 0.5 -  sign(rd)* q) / abs(rd);
		float b = min(d.x, min(d.y, d.z));
		float a = max(roomDis - 1.73, b + EPS); // TODO 1.73 kan vara for mycket 
		res = un(res, vec2(max(EPS, a), -1));
	}
	
	{
		vec3 s2 = vec3(30, 10, 30);
		vec3 q2 = mod(vec3(n), s2) - 0.5 * s2;
		float pDis = sdCylinder(vec3(q2.x, n.y, q2.z), vec3(0, 0, 4));
		if (pDis < 0.0) {
			float box = udRoundBox(q, vec3(0.4), 0.1);
			res = un(res, vec2(box, MAT_S5_PILLAR));
		} else {
			vec3 d = (s * 0.5 -  sign(rd)* q) / abs(rd);
			float b = min(d.x, min(d.y, d.z));
			float a = max(pDis - 1.73, b + EPS); // TODO 1.73 kan vara for mycket 
			res = un(res, vec2(max(EPS, a), -1));
		}
	}
	
	return res;
}


vec3 getNormal(vec3 p, vec3 rd, vec3 ro)
{
    vec3 normal;
    vec3 ep = vec3(0.01, 0, 0);
    normal.x = scene(p + ep.xyz, rd).x - scene(p - ep.xyz, rd).x;
    normal.y = scene(p + ep.yxz, rd).x - scene(p - ep.yxz, rd).x;
    normal.z = scene(p + ep.yzx, rd).x - scene(p - ep.yzx, rd).x;
    return normalize(normal);
}


float specular(vec3 normal, vec3 light, vec3 viewdir, float s)
{
	float nrm = (s + 8.0) / (3.1415 * 8.0);
	float k = max(0.0, dot(viewdir, reflect(light, normal)));
    return  pow(k, s);
}


void mainImage( out vec4 fragColor, in vec2 fragCoord )
{
	const int jumps = 2;
	const int imax = 600;
	float tmax = 800.0;
	vec3 skyColor = vec3(0);
	bool cubicDis = false;
	vec3 rollV = vec3(0, 1, 0);
	
    float time = iTime;
    vec3 eye = vec3(15.0*sin(time/5.0), 3.0, 15.0*cos(time/5.0)); 
    vec3 tar = vec3(0, 0, 0);
    float r = 6.0; 
    float azi = time;
    float pol = 0.5 * time;
    float x = r*cos(pol)*sin(azi);
    float y = r*sin(pol)*sin(azi);
    float z = r*cos(azi);

    vec3 light = vec3(x, y, z);
    float shadowAmbient = 0.7;
    float lightInvSize = 30.0;
    float lightIntensity = 0.01;

    float refJumpDistance = 1.0;
    
	vec3 dir = normalize(tar - eye);
	vec3 right = normalize(cross(rollV, dir)); 
 	vec3 up = cross(dir, right);
    
    float u = (fragCoord.x / iResolution.x) * 2.0 - 1.0;
    float v = ((fragCoord.y / iResolution.y) * 2.0 - 1.0) * (iResolution.y/iResolution.x);

    vec3 color = skyColor;
      
    float t = 0.0;
	vec3 ro = eye;
	vec3 rd = normalize(dir + right*u + up*v);
	
	float ref = 1.0;
	float lightAura = 0.0;
    float breakVar = 0.0;
	for(int j = 0; j < 2; ++j)
    {
        if (breakVar >= 0.5) {
        	break;
        }
    	t = 0.0;
        for(int i = 0; i < imax; ++i)
   		 {
             if (t >= tmax) {
                 break;
             }
	        vec3 p = ro + rd * t;
	        vec2 dm = scene(p, rd);
	        float d = dm.x;
	        float m = dm.y;
			
			
	        if(d < EPS || i == imax || t >= tmax) 
	        {
	        	vec3 x0 = light;
	        	vec3 x1 = ro;
	        	vec3 x2 = ro + rd;
	        	float ldis = pow(length(cross(x2 - x1, x1 - x0)),2.0) / pow( distance(x2, x1), 2.0); 
	        	vec3 normal = getNormal(p, rd, ro);
				
				vec3 invLight = normalize(light - p);
	        	float diffuse = max(0.,dot(invLight, normal));
	        	vec3 refrd = reflect(rd, normal);
	        	

				vec3 c = vec3(0.5);
				
				if (m == MAT_S5_BLOB) {
					c = vec3(1);
				} else if (m == MAT_S5_FLOOR) {
					c = vec3(0.9);
				} else if (m == MAT_S5_PILLAR) {
					c = vec3(0.8, 0, 0);
				} 
                c = 0.7*c* (1.0 + diffuse);        
                c += specular(normal, -invLight, normalize(eye - p), 70.0);
	        	
	            
	            float dis = length(light - p);
	            float disFact = 1.0 / (1.0 + lightIntensity*dis*dis * (cubicDis ? dis : 1.0 ));
	            c *= disFact;
				
					        	
	        	float tl = -dot(x1 - x0, x2 - x1)/pow(distance(x2,x1),2.0);
	        	if(tl > 0.0 && (distance(eye, light) < distance(eye, p))){
	        		lightAura = max(lightAura, 1.0/(0.01 + lightInvSize*ldis));
	        	}

				color = mix(color, c, ref);
				
				
                rd = reflect(rd, normal);
	         	ro = p + rd*refJumpDistance;
				
	        	
	        	 if (m == MAT_S5_BLOB) {
	        		ref = 0.7;
	        	} else {
		        	ref = 0.0;
	        	}
	        	if (ref <= 0.01) {
	        		breakVar = 1.0;       		
	        	} 
	           	break;
	        }
	
	        t += d;
    	}
    }
    
   
    fragColor = vec4(color + vec3(lightAura),  1.0);



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


