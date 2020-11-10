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

// Copyright 2020 Ricky Reusser
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
// The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//
// This function draws a contour plot, using the logarithmic (relative)
// gradient to select locally appropriate contour spacing. It then blends
// across multiple octaves to achieve a band-limited fractal effect.
//
// For a single-octave line contour version, see: https://www.shadertoy.com/view/tsycWV
// For line contours blended over multiple octaves, see: https://www.shadertoy.com/view/wsGyDV

const float minSpacing = 2.0;
const float divisions = 6.0;
const float rampPower = 2.0;
const float antialiasWidth = 1.0;
const int octaves = 5;

// A function which sends contrast to the edges, in the style of an unsharp mask
float contrastFunction(float x, float power) {
  x = 2.0 * x - 1.0;
  return 0.5 + 0.5 * pow(abs(x), power) * sign(x);
}

// Draws contours which adjust to the local relative rate of change
//              f: input to be contoured
//       gradient: screen-space gradient of the input f
//     minSpacing: Smallest contour spacing, in (approximate) pixels
//      divisions: Number of divisions per size increment
//   antialiasing: Width of antialiasing blur
//      rampPower: Power of the contrast ramp
float shadedContours (float f, vec2 gradient, float minSpacing, float divisions, float antialiasWidth, float rampPower) {
  float screenSpaceLogGrad = hypot(gradient) / f;
  float localOctave = log2(screenSpaceLogGrad * minSpacing) / log2(divisions);
  float contourSpacing = pow(divisions, ceil(localOctave));
  
  float plotVar = log2(f) / contourSpacing;
  float widthScale = contourSpacing / screenSpaceLogGrad;

  float contourSum = 0.0;
  float weightSum = 0.0;
  for(int i = 0; i < octaves; i++) {
    // A weight which fades in the smallest octave and fades out the largest
    float t = float(i + 1) - fract(localOctave);
    float weight = smoothstep(0.0, 1.0, t) * smoothstep(float(octaves), float(octaves) - 1.0, t);

    // Shading for this octave is the contrast ramp with a chunk of fixed pixel width
    // cut out of the corner for antialiasing
    float y = fract(plotVar);
    weightSum += weight;
    contourSum += weight * min(
      contrastFunction(y, rampPower),
      (1.0 - y) * 0.5 * widthScale / antialiasWidth
    );

    // Adjust scales for the next octave
    widthScale *= divisions;
    plotVar /= divisions;
  }

  return 1.0 - contourSum / weightSum;
}

// A complex function with two zeros and a pole
vec2 sampleFunction (vec2 z, vec2 zMouse) {
  return cmul(cdiv(z - vec2(1, 0), z + vec2(1, 0)), z - zMouse);
}

// Frag coord to some nice plot range
vec2 viewport (vec2 ij) {
    return 4.0 * vec2(1, iResolution.y / iResolution.x) * (ij / iResolution.xy - 0.5);
}

void mainImage(out vec4 fragColor, in vec2 fragCoord) {
    vec2 z = viewport(fragCoord);
    vec2 zMouse = iMouse.w < 0.5 ? vec2(1.2 * cos(0.5 * iTime), 0.5 * sin(iTime)) : viewport(iMouse.xy);

    vec2 f = sampleFunction(z, zMouse);
    
    // The gradient and its magnitude:
    float fMag = hypot(f);
    vec2 fMagGradient = vec2(dFdx(fMag), dFdy(fMag));
    
    float contour = shadedContours(fMag, fMagGradient, minSpacing, divisions, antialiasWidth, rampPower);

    fragColor = vec4(vec3(pow(contour, 0.454)),1.0);
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


