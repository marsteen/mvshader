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



const float PERIOD = 3.5;

const float RADIUS = 0.8;

const float LINE_WIDTH = 3.;

const int NUM_STEPS = 50;

const int TILE_MIN = -5;
const int TILE_MAX = 5;

//const float PI = 4. * atan(1.);
const float PI = 3.141592654;

// Smooth HSV to RGB conversion
// https://www.shadertoy.com/view/MsS3Wc
vec3 hsv2rgb_smooth(float hue, float saturation, float value) {
    vec3 rgb = clamp(abs(mod(hue*6.0+vec3(0.0,4.0,2.0),6.0)-3.0)-1.0, 0.0, 1.0);

	rgb = rgb*rgb*(3.0-2.0*rgb);  // Cubic smoothing

	return value * mix(vec3(1.0), rgb, saturation);
}

vec2 curve(float t, float a, float b, float d) {
	return vec2(
        sin(a * t + d) * cos(t + d),
        cos(b * t) * sin(t + d)
    );
}

vec4 mainImage(vec2 fragCoord)
{
    const int numTiles = TILE_MAX - TILE_MIN + 1;

    float minRes = min(iResolution.x, iResolution.y);
    float scale = float(numTiles) / minRes;

    ivec2 tileCoord = ivec2(round((fragCoord - iResolution.xy / 2.) * scale));

	vec2 uv = 2. * fragCoord * scale - 2. * vec2(tileCoord) - iResolution.xy * scale;

    // Flip tiles with positive a Y coordinate for symmetry.
    if (tileCoord.y > 0) {
        uv.y = -uv.y;
    }



	float d = (iMouse.w > 0. ? iMouse.y / iResolution.y : iTime / PERIOD) * 2. * PI;
    float hueOffset = (iMouse.w > 0. ? 4. * iMouse.x / iResolution.x : 0.);

    // If the tile coordinates have different parity,
    // only half of the period is needed.
    bool halfPeriod = abs((tileCoord.x * tileCoord.y) % 2) == 1;
	float tPeriod = halfPeriod ? PI : 2. * PI;
    vec2 p1 = curve(0., float(tileCoord.x), float(tileCoord.y), d) * RADIUS;
	float minDist = 1.0;
    float minDistI;
    int numSteps = halfPeriod ? NUM_STEPS : 2 * NUM_STEPS;
    for (int i = 1; i <= numSteps; i++) {
        float t = float(i) / float(numSteps) * tPeriod;
        vec2 p2 = curve(t, float(tileCoord.x), float(tileCoord.y), d) * RADIUS;

		// Distance to line
        vec2 pa = uv - p2;
        vec2 ba = p1 - p2;

        float h = clamp(dot(pa, ba) / dot(ba, ba), 0.0, 1.0);

        vec2 q = pa - ba * h;

        float dist = dot(q, q);
        if (dist < minDist) {
            minDist = dist;
            minDistI = (float(i) - h);
        }
        p1 = p2;
    }

    float hue = fract(hueOffset + minDistI / float(numSteps));
//    float dist = length(curve(hue * tPeriod, float(tileCoord.x), float(tileCoord.y), d) * RADIUS - uv);

	float v = smoothstep(LINE_WIDTH * scale, 0.0, sqrt(minDist));
	return vec4(hsv2rgb_smooth(hue, 1., v), 1.0);
}

void main()
{
    vec2 fragCoord = vTextVary * iResolution.xy;
    outputColor = mainImage(fragCoord);
}


