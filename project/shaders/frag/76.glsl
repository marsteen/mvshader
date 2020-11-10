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

// "Truchet Passages" by dr2 - 2020
// License: Creative Commons Attribution-NonCommercial-ShareAlike 3.0 Unported License

#define AA  0   // optional antialiasing

float PrSphDf (vec3 p, float r);
float PrCylDf (vec3 p, float r, float h);
vec2 PixToHex (vec2 p);
vec2 HexToPix (vec2 h);
float Minv3 (vec3 p);
float SmoothMax (float a, float b, float r);
float SmoothBump (float lo, float hi, float w, float x);
mat3 StdVuMat (float el, float az);
vec2 Rot2D (vec2 q, float a);
float Hashfv2 (vec2 p);
float Fbm1 (float p);
float Fbm2 (vec2 p);
vec3 VaryNf (vec3 p, vec3 n, float f);

vec3 sunDir;
vec2 gId, cMid;
float tCur, dstFar, hgSize, cDir, trWidf;
int idObj;
const int idPath = 1, idRail = 2, idBod = 3, idSup = 4, idBas = 5, idCone = 6,
   idRfC = 7, idRfT = 8, idGrnd = 9;
const float pi = 3.1415927, sqrt3 = 1.7320508;

#define VAR_ZERO min (iFrame, 0)

#define DMIN(id) if (d < dMin) { dMin = d;  idObj = id; }

float ObjDf (vec3 p)
{
  vec3 q;
  vec2 qa, qb;
  float dMin, d, wThk, r, s1, s2;
  dMin = dstFar;
  wThk = 0.005;
  p.xz -= cMid;
  p /= hgSize;
  p.xz = Rot2D (p.xz, cDir * pi / 6.);
  p.xz = Rot2D (p.xz, 2. * pi * floor (3. * atan (p.z, - p.x) / (2. * pi) + 0.5) / 3.);
  q = p;
  q.x += 1.;
  r = length (q.xz);
  d = max (abs (r - 0.5) - trWidf, abs (q.y - 0.1) - 0.02);
  DMIN (idPath);
  d = length (vec2 (abs (r - 0.5) - trWidf + 0.02, q.y - 0.27)) - 0.007;
  DMIN (idRail);
  q = p;
  q.x += 1.;
  q.xz = Rot2D (q.xz, 2. * pi * floor (12. * atan (q.z, - q.x) / (2. * pi) + 0.5) / 12.);
  q.y -= 0.17;
  q.x = abs (q.x + 0.5) - trWidf + 0.02;
  d = PrCylDf (q.xzy, 0.007, 0.1);
  DMIN (idRail);
  q = p;
  q.x += 0.46 * sqrt3 - 0.02;
  q.y -= 0.51;
  d = 0.9 * PrCylDf (q.xzy, 0.01 * (1. - 0.6 * q.y), 0.51);
  DMIN (idSup);
  q = p;
  d = PrCylDf (q.xzy, 0.46 * sqrt3, 0.08);
  DMIN (idBas);
  q.y -= 0.2;
  d = PrCylDf (q.xzy, 0.1 - 0.2 * q.y, 0.2);
  DMIN (idCone);
  q = p;
  q.y -= 1.;
  s1 = length (vec2 (abs (r - 0.5), abs (q.y))) - 0.8 * trWidf;
  s2 = length (vec2 (abs (length (q.xz) - 0.46 * sqrt3 + trWidf), abs (q.y))) - 0.8 * trWidf;
  d = SmoothMax (max (abs (s1), - s2) - wThk, - q.y, 0.02);
  DMIN (idRfT);
  d = SmoothMax (max (abs (s2), - s1) - wThk, - q.y, 0.02);
  DMIN (idRfC);
  q = p;
  d = q.y;
  DMIN (idGrnd);
  q = p;
  q.x += 1.;
  q.xz = Rot2D (q.xz, mod (0.5 * cDir * tCur + pi / 3., 2. * pi / 3.) - pi / 3.);
  qa = 0.5 * q.xz - vec2 (0.5, 0.);
  qb = 0.5 * sqrt3 * q.zx * vec2 (1., -1.);
  r = sqrt (dot (qa, qa) + dot (qb, qb) - 2. * abs (dot (qa, qb)));
  d = min (max (r, abs (q.y - 0.21) - 0.09), length (vec2 (r, q.y - 0.34))) - 0.025;
  DMIN (idBod);
  return dMin;
}

void SetTrConf ()
{
  cMid = HexToPix (gId * hgSize);
  cDir = 2. * step (Hashfv2 (gId), 0.5) - 1.;
}

float ObjRay (vec3 ro, vec3 rd)
{
  vec3 vri, vf, hv, p;
  vec2 edN[3], pM, gIdP;
  float dHit, d, s, eps;
  if (rd.x == 0.) rd.x = 0.0001;
  if (rd.z == 0.) rd.z = 0.0001;
  eps = 0.0005;
  edN[0] = vec2 (1., 0.);
  edN[1] = 0.5 * vec2 (1., sqrt3);
  edN[2] = 0.5 * vec2 (1., - sqrt3);
  for (int k = 0; k < 3; k ++) edN[k] *= sign (dot (edN[k], rd.xz));
  vri = hgSize / vec3 (dot (rd.xz, edN[0]), dot (rd.xz, edN[1]), dot (rd.xz, edN[2]));
  vf = 0.5 * sqrt3 - vec3 (dot (ro.xz, edN[0]), dot (ro.xz, edN[1]),
     dot (ro.xz, edN[2])) / hgSize;
  pM = HexToPix (PixToHex (ro.xz / hgSize));
  hv = (vf + vec3 (dot (pM, edN[0]), dot (pM, edN[1]), dot (pM, edN[2]))) * vri;
  s = Minv3 (hv);
  gIdP = vec2 (-999.);
  dHit = 0.;
  for (int j = VAR_ZERO; j < 240; j ++) {
    p = ro + dHit * rd;
    gId = PixToHex (p.xz / hgSize);
    if (gId != gIdP) {
      gIdP = gId;
      SetTrConf ();
    }
    d = ObjDf (p);
    if (dHit + d < s) {
      dHit += d;
    } else {
      dHit = s + eps;
      pM += sqrt3 * ((s == hv.x) ? edN[0] : ((s == hv.y) ? edN[1] : edN[2]));
      hv = (vf + vec3 (dot (pM, edN[0]), dot (pM, edN[1]), dot (pM, edN[2]))) * vri;
      s = Minv3 (hv);
    }
    if (d < eps || dHit > dstFar) break;
  }
  if (d >= eps) dHit = dstFar;
  return dHit;
}

vec3 ObjNf (vec3 p)
{
  vec4 v;
  vec2 e;
  e = vec2 (0.001, -0.001);
  for (int j = VAR_ZERO; j < 4; j ++) {
    v[j] = ObjDf (p + ((j < 2) ? ((j == 0) ? e.xxx : e.xyy) : ((j == 2) ? e.yxy : e.yyx)));
  }
  v.x = - v.x;
  return normalize (2. * v.yzw - dot (v, vec4 (1.)));
}

float ObjSShadow (vec3 ro, vec3 rd)
{
  vec3 p;
  vec2 gIdP;
  float sh, d, h;
  sh = 1.;
  d = 0.01;
  gIdP = vec2 (-99.);
  for (int j = VAR_ZERO; j < 50; j ++) {
    p = ro + d * rd;
    gId = PixToHex (p.xz / hgSize);
    if (gId != gIdP) {
      gIdP = gId;
      SetTrConf ();
    }
    h = ObjDf (p);
    sh = min (sh, smoothstep (0., 0.05 * d, h));
    d += clamp (h, 0.02, 0.5);
    if (sh < 0.05 ) break;
  }
  return 0.4 + 0.6 * sh;
}

vec4 PathCol (vec2 p)
{
  vec4 col4;
  vec2 q;
  float a, d, f;
  p = Rot2D (p, cDir * pi / 6.);
  p = Rot2D (p, 2. * pi * floor (3. * atan (p.y, - p.x) / (2. * pi) + 0.5) / 3.);
  p.x += 1.;
  col4 = vec4 (0.9, 0.9, 0.95, 0.1);
  col4 *= 0.3 + 0.7 * smoothstep (0.05, 0.1, abs (mod (72. * atan (p.y, - p.x) / (2. * pi) + 0.5, 1.) - 0.5));
  f = abs (length (p) - 0.5);
  if (f < trWidf - 0.04) {
    q = Rot2D (p, mod (0.5 * cDir * tCur + pi / 3., 2. * pi / 3.) - pi / 3.);
    a = mod (15. * atan (cDir * q.y, - q.x) / (2. * pi), 1.) - 0.5;
    for (float s = 1.; s >= 0.; s -= 1.) {
      d = 1.;
      if (abs (a - 0.2) - 0.2 < 0.006 * s) d = min (d, smoothstep (0., 0.002,
         f - 0.015 * (1. - a / 0.2) - 0.002 * s));
      else if (abs (a + 0.2) - 0.2 < 0.006 * s) d = min (d, smoothstep (0., 0.002, f - 0.005 - 0.003 * s));
      if (abs (mod (4. * a + 0.5, 1.) - 0.5) - 0.3 < 0.03 * s)
         d = min (d, smoothstep (0., 0.002, abs (f - 0.05) - 0.004 - 0.003 * s));
      col4 = mix (vec4 (1. - 0.9 * s, 0., 0., -1.), col4, d);
    }
  }
  return col4;
}

vec3 SkyHrzCol (vec3 ro, vec3 rd)
{
  vec3 col, clCol;
  vec2 q;
  float f, fd, ff;
  if (rd.y > -0.05 && rd.y < 0.03 * Fbm1 (16. * atan (rd.z, - rd.x))) {
    col = mix (vec3 (0.1, 0.35, 0.1), vec3 (0.3, 0.35, 0.35),
       smoothstep (-0.05, 0.01, rd.y));
  } else if (rd.y > 0.) {
    q = 0.005 * (ro.xz + tCur * vec2 (0.5, 2.) + ((200. - ro.y) / rd.y) * rd.xz);
    ff = Fbm2 (q);
    f = smoothstep (0.1, 0.8, ff);
    fd = smoothstep (0.1, 0.8, Fbm2 (q + 0.01 * sunDir.xz)) - f;
    clCol = (0.8 + 0.5 * ff) * (vec3 (0.7) - 0.7 * vec3 (0.3, 0.3, 0.2) * sign (fd) *
       smoothstep (0., 0.05, abs (fd)));
    fd = smoothstep (0.01, 0.1, rd.y);
    col = mix (mix (vec3 (0.8, 0.8, 0.75), vec3 (0.4, 0.5, 0.8), 0.3 + 0.7 * fd), clCol,
       0.1 + 0.9 * f * fd);
  }
  return 0.5 * col;
}

vec3 ShowScene (vec3 ro, vec3 rd)
{
  vec4 col4;
  vec3 col, vn;
  vec2 q, vf;
  float dstObj, sh, nDotL, s;
  dstObj = ObjRay (ro, rd);
  if (dstObj < dstFar) {
    ro += dstObj * rd;
    vn = ObjNf (ro);
    vf = vec2 (0.);
    q = (ro.xz - cMid) / hgSize;
    if (idObj == idPath) {
     col4 = PathCol (q);
    } else if (idObj == idRail) {
      col4 = vec4 (0.8, 0.7, 0.2, 0.2);
    } else if (idObj == idBod) {
      col4 = mix (vec4 (0.2, 0.7, 0.2, 0.2), vec4 (0.2, 1., 0.2, -1.), step (0.66, ro.y));
    } else if (idObj == idSup) {
      col4 = vec4 (0.8, 0.8, 0.85, 0.1);
    } else if (idObj == idBas) {
      s = length (q);
      col4 = vec4 (0.55, 0.5, 0.5, 0.1) * (0.8 + 0.2 * smoothstep (0.12, 0.15, s));
      col4 = mix (vec4 (0., 0., 1., 0.1), col4, 0.5 + 0.5 * smoothstep (0.12, 0.18, s));
      vf = vec2 (64., 0.2);
    } else if (idObj == idCone) {
      col4 = vec4 (0.1, 0.1, 0.6, -1.);
    } else if (idObj == idRfT) {
      if (vn.y < 0.) {
        s = abs (length (q) - 0.46 * sqrt3 + trWidf);
        q = Rot2D (q, cDir * pi / 6.);
        q = Rot2D (q, 2. * pi * floor (3. * atan (q.y, - q.x) / (2. * pi) + 0.5) / 3.);
        q.x += 1.;
        s = min (s, abs (abs (length (q) - 0.5)));
      }
    } else  if (idObj == idRfC) {
      if (vn.y < 0.) s = abs (length (q) - 0.46 * sqrt3 + trWidf);
    } else  if (idObj == idGrnd) {
      col4 = vec4 (0.3, 0.4, 0.3, 0.);
      vf = vec2 (32., 1.);
    }
    if (idObj == idRfT || idObj == idRfC) {
      col4 = vec4 (0.5, 0.5, 0.55, 0.1);
      if (vn.y < 0.) col4 = mix (vec4 (1., 1., 0.9, -1.), col4, smoothstep (0.004, 0.008, s));
      if (vn.y > 0. || s > 0.008) vf = vec2 (64., 0.5);
    } else if (idObj == idGrnd || idObj == idBas && abs (vn.y) < 0.1) {
      q = Rot2D (q, cDir * pi / 6.);
      q = Rot2D (q, 2. * pi * floor (3. * atan (q.y, - q.x) / (2. * pi) + 0.5) / 3.);
      q.x += 1.;
      col4 = mix (vec4 (0., 1., 0., 0.1), col4, smoothstep (0.02, 0.12, abs (abs (length (q) - 0.5))));
    }
    if (vf.x > 0.) vn = VaryNf (vf.x * ro, vn, vf.y);
    nDotL = max (dot (vn, sunDir), 0.);
    if (idObj == idRail || idObj == idSup) nDotL *= nDotL;
    sh = ObjSShadow (ro, sunDir);
    if (col4.a >= 0.) col = col4.rgb * (0.2 + 0.8 * sh * nDotL) +
       col4.a * step (0.95, sh) * pow (max (dot (normalize (sunDir - rd), vn), 0.), 32.);
    else col = col4.rgb * (0.6 - 0.4 * dot (vn, rd));
    col = mix (col, vec3 (0.1, 0.2, 0.1), 1. - max (0., 1. - 2. * smoothstep (0., 1.,
       dstObj / (2. * dstFar))));
  } else col = SkyHrzCol (ro, rd);
  return clamp (col, 0., 1.);
}

vec2 TrkPath (float t)
{
  vec2 r;
  float tt;
  tt = mod (t, 4.);
  if (tt < 1.) r = mix (vec2 (sqrt3/2., -0.5), vec2 (sqrt3/2., 0.5), tt);
  else if (tt < 2.) r = mix (vec2 (sqrt3/2., 0.5), vec2 (0., 1.), tt - 1.);
  else if (tt < 3.) r = mix (vec2 (0., 1.), vec2 (0., 2.), tt - 2.);
  else r = mix (vec2 (0., 2.), vec2 (sqrt3/2., 2.5), tt - 3.);
  r += vec2 (0.001, 3. * floor (t / 4.));
  return r * hgSize;
}

void mainImage (out vec4 fragColor, in vec2 fragCoord)
{
  mat3 vuMat;
  vec4 mPtr;
  vec3 ro, rd, col;
  vec2 canvas, uv, uvv, p1, p2, vd;
  float el, az, zmFac, asp, sr, vel, tCyc, s;
  canvas = iResolution.xy;
  uv = 2. * fragCoord.xy / canvas - 1.;
  uv.x *= canvas.x / canvas.y;
  tCur = iTime;
  mPtr = iMouse;
  mPtr.xy = mPtr.xy / canvas - 0.5;
  asp = canvas.x / canvas.y;
  hgSize = 2.;
  trWidf = 0.1;
  vel = 0.2;
  tCyc = 4. / vel;
  p1 = 0.5 * (TrkPath (vel * tCur + 0.05) + TrkPath (vel * tCur + 0.15));
  p2 = 0.5 * (TrkPath (vel * tCur - 0.05) + TrkPath (vel * tCur - 0.15));
  s = SmoothBump (0.25, 0.75, 0.05, mod (tCur / (1. * tCyc), 1.));
  ro.xz = 0.5 * (p1 + p2);
  ro.x += 0.2 * (2. * SmoothBump (0.25, 0.75, 0.15, mod (0.07 * vel * tCur, 1.)) - 1.);
  ro.y = 1.2 + 1.8 * s;
  vd = p1 - p2;
  az = atan (vd.x, vd.y);
  el = -0.2 * pi * (s + 0.3);
  if (mPtr.z > 0.) {
    az += 2. * pi * mPtr.x;
    el += pi * mPtr.y;
  }
  vuMat = StdVuMat (el, az);
  zmFac = 2.5;
  dstFar = 60.;
  sunDir = normalize (vec3 (1., 1.5, -1.));
#if ! AA
  const float naa = 1.;
#else
  const float naa = 3.;
#endif  
  col = vec3 (0.);
  sr = 2. * mod (dot (mod (floor (0.5 * (uv + 1.) * canvas), 2.), vec2 (1.)), 2.) - 1.;
  for (float a = float (VAR_ZERO); a < naa; a ++) {
    uvv = (uv + step (1.5, naa) * Rot2D (vec2 (0.5 / canvas.y, 0.), sr * (0.667 * a + 0.5) * pi)) / zmFac;
    rd = vuMat * normalize (vec3 ((2. * tan (0.5 * atan (uvv.x / asp))) * asp, uvv.y, 1.));
    col += (1. / naa) * ShowScene (ro, rd);
  }
  fragColor = vec4 (col, 1.);
}

float PrSphDf (vec3 p, float r)
{
  return length (p) - r;
}

float PrCylDf (vec3 p, float r, float h)
{
  return max (length (p.xy) - r, abs (p.z) - h);
}

vec2 PixToHex (vec2 p)
{
  vec3 c, r, dr;
  c.xz = vec2 ((1./sqrt3) * p.x - (1./3.) * p.y, (2./3.) * p.y);
  c.y = - c.x - c.z;
  r = floor (c + 0.5);
  dr = abs (r - c);
  r -= step (dr.yzx, dr) * step (dr.zxy, dr) * dot (r, vec3 (1.));
  return r.xz;
}

vec2 HexToPix (vec2 h)
{
  return vec2 (sqrt3 * (h.x + 0.5 * h.y), (3./2.) * h.y);
}

float Minv3 (vec3 p)
{
  return min (p.x, min (p.y, p.z));
}

float SmoothMin (float a, float b, float r)
{
  float h;
  h = clamp (0.5 + 0.5 * (b - a) / r, 0., 1.);
  return mix (b, a, h) - r * h * (1. - h);
}

float SmoothMax (float a, float b, float r)
{
  return - SmoothMin (- a, - b, r);
}

float SmoothBump (float lo, float hi, float w, float x)
{
  return (1. - smoothstep (hi - w, hi + w, x)) * smoothstep (lo - w, lo + w, x);
}

mat3 StdVuMat (float el, float az)
{
  vec2 ori, ca, sa;
  ori = vec2 (el, az);
  ca = cos (ori);
  sa = sin (ori);
  return mat3 (ca.y, 0., - sa.y, 0., 1., 0., sa.y, 0., ca.y) *
         mat3 (1., 0., 0., 0., ca.x, - sa.x, 0., sa.x, ca.x);
}

vec2 Rot2D (vec2 q, float a)
{
  vec2 cs;
  cs = sin (a + vec2 (0.5 * pi, 0.));
  return vec2 (dot (q, vec2 (cs.x, - cs.y)), dot (q.yx, cs));
}

const float cHashM = 43758.54;

float Hashfv2 (vec2 p)
{
  return fract (sin (dot (p, vec2 (37., 39.))) * cHashM);
}

vec2 Hashv2f (float p)
{
  return fract (sin (p + vec2 (0., 1.)) * cHashM);
}

vec2 Hashv2v2 (vec2 p)
{
  vec2 cHashVA2 = vec2 (37., 39.);
  return fract (sin (dot (p, cHashVA2) + vec2 (0., cHashVA2.x)) * cHashM);
}

float Noiseff (float p)
{
  vec2 t;
  float ip, fp;
  ip = floor (p);
  fp = fract (p);
  fp = fp * fp * (3. - 2. * fp);
  t = Hashv2f (ip);
  return mix (t.x, t.y, fp);
}

float Noisefv2 (vec2 p)
{
  vec2 t, ip, fp;
  ip = floor (p);  
  fp = fract (p);
  fp = fp * fp * (3. - 2. * fp);
  t = mix (Hashv2v2 (ip), Hashv2v2 (ip + vec2 (0., 1.)), fp.y);
  return mix (t.x, t.y, fp.x);
}

float Fbm1 (float p)
{
  float f, a;
  f = 0.;
  a = 1.;
  for (int j = 0; j < 5; j ++) {
    f += a * Noiseff (p);
    a *= 0.5;
    p *= 2.;
  }
  return f * (1. / 1.9375);
}

float Fbm2 (vec2 p)
{
  float f, a;
  f = 0.;
  a = 1.;
  for (int j = 0; j < 5; j ++) {
    f += a * Noisefv2 (p);
    a *= 0.5;
    p *= 2.;
  }
  return f * (1. / 1.9375);
}

float Fbmn (vec3 p, vec3 n)
{
  vec3 s;
  float a;
  s = vec3 (0.);
  a = 1.;
  for (int j = 0; j < 5; j ++) {
    s += a * vec3 (Noisefv2 (p.yz), Noisefv2 (p.zx), Noisefv2 (p.xy));
    a *= 0.5;
    p *= 2.;
  }
  return dot (s, abs (n));
}

vec3 VaryNf (vec3 p, vec3 n, float f)
{
  vec3 g;
  vec2 e = vec2 (0.1, 0.);
  g = vec3 (Fbmn (p + e.xyy, n), Fbmn (p + e.yxy, n), Fbmn (p + e.yyx, n)) - Fbmn (p, n);
  return normalize (n + f * (g - n * dot (n, g)));
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


