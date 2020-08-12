//
// Fragment Shader
//

uniform vec3  iResolution;
uniform vec3  iMouse;
uniform float iTime;
varying vec2  vTextVary;




#define MAX_MARCHES 32
#define MAX_DISTANCE 32.0
#define COLLISION_DISTANCE 0.01
#define Bailout 16.0
#define Iterations 6

// Mandelbulb Distance Estimator
// Adapted from:
// http://blog.hvidtfeldts.net/index.php/2011/09/distance-estimated-3d-fractals-v-the-mandelbulb-different-de-approximations/
float sphere(vec3 pos) {
	float Power = float((sin(iTime * 2.5) * 0.5) + 8.0);
	vec3 z = pos;
	float dr = 1.0;
	float r = 0.0;
	for (int i = 0; i < Iterations ; i++) {
		r = length(z);
		if (r>Bailout) break;
		
		// Convert to Polar Coordinates
		float theta = acos(z.z/r);
		float phi = atan(z.y,z.x);
		dr =  pow( r, Power-1.0)*Power*dr + 1.0;
		
		// Scale and Rotate the Point
		float zr = pow( r,Power);
		theta = theta*Power;
		phi = phi*Power;
		
		// Convert Back to Cartesian Coordinates
		z = zr*vec3(sin(theta)*cos(phi), sin(phi)*sin(theta), cos(theta));
		z+=pos;
	}
	return 0.5*log(r)*r/dr;
}

// Compute/March the Ray
float raymarch(vec3 camerapos, vec3 raydir) {
	float distorigin=0.0;
    
    for(int i=0; i<MAX_MARCHES; i++) {
    	vec3 raypos = camerapos + raydir*distorigin;
        float distsurface = sphere(raypos);
        distorigin += distsurface;
        if(distorigin>MAX_DISTANCE || distsurface<COLLISION_DISTANCE) break;
    }
    
    return distorigin;
}

// Get Normal
vec3 normal(vec3 raypos) {
	float dis = sphere(raypos);
    vec2 e = vec2(.01, 0);
    
    vec3 normal = dis - vec3(
        sphere(raypos-e.xyy),
        sphere(raypos-e.yxy),
        sphere(raypos-e.yyx));
    
    return normalize(normal);
}

// Shade Scene
vec3 shade(vec3 march) {
	// Light Positions
    vec3 lightpositiona = vec3(0, 8, 0);
	vec3 lightpositionb = vec3(0, 0, 0);
    vec3 lightpositionc = vec3(0, -8, 0);
	// Animate Lights
    lightpositiona.xz += vec2(sin(iTime * 4.0), cos(iTime * 4.0))*4.0;
    lightpositionb.xz += vec2(sin(iTime * 4.0), cos(iTime * 4.0))*4.0;
    lightpositionc.xz += vec2(sin(iTime * 4.0), cos(iTime * 4.0))*4.0;
	// Compute Lighting
    vec3 lightinga = normalize(lightpositiona-march);
    vec3 lightingb = normalize(lightpositionb-march);
    vec3 lightingc = normalize(lightpositionc-march);
	// Compute Surface Normal
    vec3 surfacenormal = normal(march);
	// Compute Diffuse
    float diffuseshader = clamp(dot(surfacenormal, lightinga), 0.0, 1.0);
    float diffuseshadeg = clamp(dot(surfacenormal, lightingb), 0.0, 1.0);
    float diffuseshadeb = clamp(dot(surfacenormal, lightingc), 0.0, 1.0);
	// Compute Geometry
    float distancesurfa = raymarch(march+surfacenormal*COLLISION_DISTANCE, lightinga);
    float distancesurfb = raymarch(march+surfacenormal*COLLISION_DISTANCE, lightingb);
    float distancesurfc = raymarch(march+surfacenormal*COLLISION_DISTANCE, lightingc);
	// Shade Geometry
    if(distancesurfa<length(lightpositiona-march)) diffuseshader *= 1.0;
    if(distancesurfa<length(lightpositionb-march)) diffuseshadeg *= 1.0;
    if(distancesurfb<length(lightpositionc-march)) diffuseshadeb *= 1.0;
    // Return Shading
    return vec3(diffuseshader, diffuseshadeg, diffuseshadeb);
}

// ACES Tone Curve
vec3 acesFilm(const vec3 x) {
    const float a = 2.51;
    const float b = 0.03;
    const float c = 2.43;
    const float d = 0.59;
    const float e = 0.14;
    return clamp((x*(a*x+b))/(x*(c*x+d)+e),0.0,1.0);
}

// Render the Image
vec4 mainImage(vec2 fragCoord )
{
	// Camera Orientation
	vec3 xdir = vec3(1,0,0);
	vec3 ydir = vec3(0,1,0);
	vec3 zdir = vec3(0,0,1);
	float FOV = 1.0;
	vec3 camerapos = vec3(0.0, 0.0, -4.5);
    camerapos = vec3(camerapos.x+((-iMouse.x+iResolution.x/2.0)/iResolution.x), camerapos.y+((-iMouse.y+iResolution.y/2.0)/iResolution.x), -4.5);

    // Undistorted Normalized Pixel Coordinates (From 0 to 1)
    vec2 uv = (fragCoord - 0.5*iResolution.xy)/iResolution.x;
	vec3 raydir = normalize(FOV*(uv.x*xdir + uv.y*ydir) + zdir);
	float collide = raymarch(camerapos, raydir);

    // Pixel Color
    vec3 col = vec3(collide / 4.0);

	// Compute and Shade
	float spheredistance = raymarch(camerapos, raydir);
    vec3 march = camerapos + raydir * spheredistance;
    vec3 diffuse = shade(march);
	
	// Finish the Image and Apply Tone Map
    col = vec3(acesFilm(diffuse));

    // Output to Screen
    return vec4(col,1.0);
}


void main()
{
    vec2 fragCoord = vTextVary * iResolution.xy;
    gl_FragColor = mainImage(fragCoord); 
}


