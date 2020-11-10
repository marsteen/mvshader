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

/**
 * Simulation of a colored cube moving on a checkerboard.
 *
 * The cube move one step forward, then one step rightward, then one step forward, ...
 *
 * Although the cube seems to rotate infinitely, it only rotates by 90° (either forward or
 * rightward) during [0, PI] seconds then go back to its initial rotation. The cube always stays
 * at position (0, 0, 0) but move a little upward during its rotation.
 *
 * The floor moves one tile backward or leftward during [0, PI] seconds then go back to its
 * initial position.
 *
 * So we have an infinite loop where everything move during [0, PI] seconds then go back to
 * the initial state.
 *
 *
 * The forward movement follows a periodic function looking like:
 *
 *     /         /         /
 *    /         /         /
 *   /         /         /
 *  /         /         /
 * /    _____/    _____/
 * 0    PI   2PI  3PI  4PI
 *
 *
 * The rightward movement follows a periodic function looking like:
 *
 *          /         /         /
 *         /         /         /
 *        /         /         /
 *       /         /         /
 * _____/    _____/    _____/
 * 0    PI   2PI  3PI  4PI
 *
 * So every PI seconds, the cube is moving either forward or rightward.
 *
 * To create the illusion that the cube is moving one tile forward and one tile rightward each
 * step, many equations include an offset of 1.0 if sin(time) > 0.0. You can set ENABLE_FLOOR_OFFSET
 * to 0.0 to disable this offset for the floor and you will see that its position is reset each time.
 */
#if HW_PERFORMANCE==0
#define AA 1
#else
#define AA 2   // make this 2 or 3 for antialiasing
#endif
#define SPEED 1.0 // how fast is the simulation
#define ENABLE_FLOOR_OFFSET 1.0
#define PI 3.1415926535897932384626433832795
#define PI2 6.28318530718

//------------------------------------------------------------------

// possible shapes
#define SHAPE_VOID 0.0
#define SHAPE_FLOOR 1.0
#define SHAPE_CUBEFRAME 2.0
// cube faces are arranged in the order they will touch the floor
#define SHAPE_CUBEFACEDOWN 3.0
#define SHAPE_CUBEFACEFRONT 4.0
#define SHAPE_CUBEFACERIGHT 5.0
#define SHAPE_CUBEFACEUP 6.0
#define SHAPE_CUBEFACEBACK 7.0
#define SHAPE_CUBEFACELEFT 8.0

// intersected shape
struct Shape
{        
    float d; // distance
    float t; // type of shape
    vec3 c; // color
};

// colors of each face of the cube
const vec3 CubeFaces[6] = vec3[6](
    vec3(1.0, 0.65, 0.0), // yellow, down
    vec3(0.14, 1.0, 0.0), // green, front
    vec3(1.0, 0.0, 0.0), // red, right
    vec3(1.0, 1.0, 1.0), // white, up
    vec3(0.0, 0.55, 1.0), // blue, back
    vec3(1.0, 0.31, 0.0) // orange, left
);

//------------------------------------------------------------------

// create a quaternion
vec4 quat(in vec3 v, in float a)
{
    return vec4(v * sin(a / 2.0), cos(a / 2.0));
}

vec4 quat_inv(in vec4 q)
{
    return vec4(-q.xyz, q.w);
}

vec4 p2q(in vec3 p)
{
    return vec4(p, 0);
}

vec4 q_mul(in vec4 q1, in vec4 q2)
{
    return vec4(q1.w*q2.x + q1.x*q2.w + q1.y*q2.z - q1.z*q2.y, 
                q1.w*q2.y - q1.x*q2.z + q1.y*q2.w + q1.z*q2.x, 
                q1.w*q2.z + q1.x*q2.y - q1.y*q2.x + q1.z*q2.w, 
                q1.w*q2.w - q1.x*q2.x - q1.y*q2.y - q1.z*q2.z);
}

vec3 rotate(in vec3 p, in vec4 q)
{
    return q_mul(q_mul(q, p2q(p)), quat_inv(q)).xyz;
}

Shape opU( Shape d1, Shape d2 )
{
    if (d1.d<d2.d) {
        return d1;
    }
    return d2;
}

float opSubtraction( float d1, float d2 ) { return max(-d1,d2); }


// test intersection with a box https://www.iquilezles.org/www/articles/distfunctions/distfunctions.htm
float sdBox( vec3 p, vec3 b )
{
    vec3 d = abs(p) - b;
    return min(max(d.x,max(d.y,d.z)),0.0) + length(max(d,0.0));
}

// test intersection with the black frame of the cube
float sdCubeFrame(vec3 p, vec3 b)
{    
    return opSubtraction(
        sdBox(p, b * vec3(0.9, 0.9, 1.1)),
        opSubtraction(
            sdBox(p, b * vec3(0.9, 1.1, 0.9)),
            opSubtraction(
                sdBox(p, b * vec3(1.1, 0.9, 0.9)),
                sdBox(p, b)
            )
        )
    );
}

// test intersection with the colored cube
Shape sdColoredCube( in vec3 pos )
{
    Shape res = Shape( 1e10, SHAPE_VOID, vec3(0.0, 0.0, 0.0) );

    {
      res = opU( res, Shape( sdCubeFrame( pos, vec3(0.25, 0.25, 0.25) ), SHAPE_CUBEFRAME, vec3(0.0, 0.0, 0.0 )) );
      res = opU( res, Shape( sdBox( pos+vec3(0.0, 0.24, 0.0), vec3(0.225, 0.01, 0.225)), SHAPE_CUBEFACEDOWN, vec3(0.0, 0.0, 0.0 ) )); // yellow, down
      res = opU( res, Shape( sdBox( pos+vec3(-0.24, 0.0, 0.0), vec3(0.01, 0.225, 0.225)), SHAPE_CUBEFACEFRONT, vec3(0.0, 0.0, 0.0)) ); // green, front
      res = opU( res, Shape( sdBox( pos+vec3(0.0, 0.0, 0.24), vec3(0.225, 0.225, 0.01)), SHAPE_CUBEFACERIGHT, vec3(0.0, 0.0, 0.0 ) )); // red, right
      res = opU( res, Shape( sdBox( pos+vec3(0.0, 0.0, -0.24), vec3(0.225, 0.225, 0.01)), SHAPE_CUBEFACELEFT, vec3(0.0, 0.0, 0.0 ) )); // orange, left
      res = opU( res, Shape( sdBox( pos+vec3(0.24, 0.0, 0.0), vec3(0.01, 0.225, 0.225)), SHAPE_CUBEFACEBACK, vec3(0.0, 0.0, 0.0 ) )); // blue, back
      res = opU( res, Shape( sdBox( pos+vec3(0.0, -0.24, 0.0), vec3(0.225, 0.01, 0.225)), SHAPE_CUBEFACEUP, vec3(0.0, 0.0, 0.0 ) )); // white, up
    }
    
    return res;
}

//------------------------------------------------------------------

/**
 * calculate the forward movement
 * 
 *     /         /         /
 *    /         /         /
 *   /         /         /
 *  /         /         /
 * /    _____/    _____/
 * 0    PI   2PI  3PI  4PI
 */
float advanceX(in float time)
{
    return mod(min(time - floor(time / PI2) * PI, PI * ceil(time / PI2)) * ceil(sin(time)), PI) / 2.0;
}

/**
 * same as advanceX but without the reset every [PI, 2PI]
 *
 * This is used to compute the forward movement when time == PI without having the
 * reset to 0.
 */
float advanceX2(in float time)
{
    return (time - floor(time / PI2) * PI) / 2.0;
}

/**
 * calculate the rightward movement
 * 
 *          /         /         /
 *         /         /         /
 *        /         /         /
 *       /         /         /
 * _____/    _____/    _____/
 * 0    PI   2PI  3PI  4PI
 */
float advanceZ(in float time)
{
    return mod(min(time - PI - floor((time - PI) / PI2) * PI, PI * ceil((time- PI) / PI2)) * (1.0 - ceil(sin(time))), PI) / 2.0;
}

// test intersection with the scene
Shape map( in float time, in vec3 pos )
{
    Shape res = Shape( 1e10, SHAPE_VOID, vec3(0.0, 0.0, 0.0) );

    {
        pos += vec3(0.0, -0.25, 0.0);
        pos += vec3(0.0, -0.10, 0.0) * abs(sin(time));
    	pos = rotate(pos, q_mul(
            quat(vec3(0.0, 0.0, 1.0), advanceX(time)),
        	quat(vec3(1.0, 0.0, 0.0), advanceZ(time))
       	));
    	res = opU( res, sdColoredCube(pos) );
    }
    
    return res;
}

//------------------------------------------------------------------

#define ZERO (min(iFrame,0))

// http://iquilezles.org/www/articles/boxfunctions/boxfunctions.htm
vec2 iBox( in vec3 ro, in vec3 rd, in vec3 rad ) 
{
    vec3 m = 1.0/rd;
    vec3 n = m*ro;
    vec3 k = abs(m)*rad;
    vec3 t1 = -n - k;
    vec3 t2 = -n + k;
	return vec2( max( max( t1.x, t1.y ), t1.z ),
	             min( min( t2.x, t2.y ), t2.z ) );
}

Shape raycast( in float time, in vec3 ro, in vec3 rd )
{
    Shape res = Shape(-1.0,SHAPE_VOID,vec3(0.0,0.0, 0.0));

    float tmin = 1.0;
    float tmax = 20.0;

    // raytrace floor plane
    float tp1 = (0.0-ro.y)/rd.y;
    if( tp1>0.0 )
    {
        tmax = min( tmax, tp1 );
        res = Shape( tp1, SHAPE_FLOOR, vec3(1.0, 1.0, 1.0) );
    }
    //else return res;
    
    // raymarch primitives   
    vec2 tb = iBox( ro-vec3(0.0,0.4,-0.5), rd, vec3(2.5,0.41,3.0) );
    if( tb.x<tb.y && tb.y>0.0 && tb.x<tmax)
    {
        //return vec2(tb.x,2.0);
        tmin = max(tb.x,tmin);
        tmax = min(tb.y,tmax);

        float t = tmin;
        for( int i=0; i<70 && t<tmax; i++ )
        {
            Shape h = map( time, ro+rd*t );
            if( abs(h.d)<(0.0001*t) )
            { 
                res = Shape(t,h.t, h.c); 
                break;
            }
            t += h.d;
        }
    }
    
    return res;
}

// http://iquilezles.org/www/articles/rmshadows/rmshadows.htm
float calcSoftshadow( in float time, in vec3 ro, in vec3 rd, in float mint, in float tmax )
{
    // bounding volume
    float tp = (0.8-ro.y)/rd.y; if( tp>0.0 ) tmax = min( tmax, tp );

    float res = 1.0;
    float t = mint;
    for( int i=ZERO; i<24; i++ )
    {
		float h = map( time, ro + rd*t ).d;
        float s = clamp(8.0*h/t,0.0,1.0);
        res = min( res, s*s*(3.0-2.0*s) );
        t += clamp( h, 0.02, 0.2 );
        if( res<0.004 || t>tmax ) break;
    }
    return clamp( res, 0.0, 1.0 );
}

// http://iquilezles.org/www/articles/normalsSDF/normalsSDF.htm
vec3 calcNormal( in float time, in vec3 pos )
{
#if 0
    vec2 e = vec2(1.0,-1.0)*0.5773*0.0005;
    return normalize( e.xyy*map( pos + e.xyy ).x + 
					  e.yyx*map( pos + e.yyx ).x + 
					  e.yxy*map( pos + e.yxy ).x + 
					  e.xxx*map( pos + e.xxx ).x );
#else
    // inspired by tdhooper and klems - a way to prevent the compiler from inlining map() 4 times
    vec3 n = vec3(0.0);
    for( int i=ZERO; i<4; i++ )
    {
        vec3 e = 0.5773*(2.0*vec3((((i+3)>>1)&1),((i>>1)&1),(i&1))-1.0);
        n += e*map(time, pos+0.0005*e).d;
      //if( n.x+n.y+n.z>100.0 ) break;
    }
    return normalize(n);
#endif    
}

float calcAO( in float time, in vec3 pos, in vec3 nor )
{
	float occ = 0.0;
    float sca = 1.0;
    for( int i=ZERO; i<5; i++ )
    {
        float h = 0.01 + 0.12*float(i)/4.0;
        float d = map(time, pos + h*nor ).d;
        occ += (h-d)*sca;
        sca *= 0.95;
        if( occ>0.35 ) break;
    }
    return clamp( 1.0 - 3.0*occ, 0.0, 1.0 ) * (0.5+0.5*nor.y);
}

// http://iquilezles.org/www/articles/checkerfiltering/checkerfiltering.htm
float checkersGradBox( in float time, in vec2 p, in vec2 dpdx, in vec2 dpdy )
{
    // filter kernel
    vec2 w = abs(dpdx)+abs(dpdy) + 0.001;
    // analytical integral (box filter)
    vec2 i = 2.0*(abs(fract((p-0.5*w)*0.5)-0.5)-abs(fract((p+0.5*w)*0.5)-0.5))/w;
    // xor pattern
    return 0.5 - 0.5*i.x*i.y;                  
}

vec3 render( in float time, in vec3 ro, in vec3 rd, in vec3 rdx, in vec3 rdy )
{ 
    // background
    vec3 col = vec3(0.7, 0.7, 0.9) - max(rd.y,0.0)*0.3;
    
    // raycast scene
    Shape res = raycast(time, ro,rd);
    float t = res.d;
    if( res.t != SHAPE_VOID )
    {
        vec3 pos = ro + t*rd;
        vec3 nor = (res.t == SHAPE_FLOOR) ? vec3(0.0,1.0,0.0) : calcNormal(time, pos );
        vec3 ref = reflect( rd, nor );
        
        // material        
        col = res.c;
        float ks = 1.0;
        
        /**
         * Every PI time, our cube is moving forward or rightward.
         * 
         * shift is used to shift the first row to the left if == 1
         */
        float shift = ceil(sin(time)) * ENABLE_FLOOR_OFFSET;
        float face = floor(time / PI);
        if (res.t > SHAPE_CUBEFRAME)
        {
            if (res.t == SHAPE_CUBEFACEUP)
            {
            	col = CubeFaces[int(mod(face + 3.0, 6.0))];   
            }
            else if (res.t == SHAPE_CUBEFACEDOWN)
            {
            	col = CubeFaces[int(mod(face, 6.0))];   
            }
            else if (res.t == SHAPE_CUBEFACEFRONT)
            {
            	col = CubeFaces[int(mod(face + 1.0 + mod(face, 2.0), 6.0))];   
            }
            else if (res.t == SHAPE_CUBEFACEBACK)
            {// 1 - 3, 2-3; 3-5, 4-5, 5-1, 6-1
            	col = CubeFaces[int(mod(face + 4.0 + mod(face, 2.0), 6.0))];   
            }
            else if (res.t == SHAPE_CUBEFACELEFT)
            {
            	col = CubeFaces[int(mod(face + 4.0 + mod(face + 1.0, 2.0), 6.0))];   
            }
            else if (res.t == SHAPE_CUBEFACERIGHT)
            {
            	col = CubeFaces[int(mod(face + 1.0 + mod(face + 1.0, 2.0), 6.0))];   
            }
        }
        else if(res.t == SHAPE_FLOOR)
        {
            // project pixel footprint into the plane
            vec3 dpdx = ro.y*(rd/rd.y-rdx/rdx.y);
            vec3 dpdy = ro.y*(rd/rd.y-rdy/rdy.y);
            
            vec3 pos2 = pos;
       		pos2 += vec3(-0.25, 0.0, -0.25);
            pos2 += vec3(0.5, 0.0, 0.0) * advanceX(time) / advanceX2(PI);
       		pos2 += vec3(0.0, 0.0, -0.5) * advanceZ(time) / advanceX2(PI);
            float tileX = floor((pos2.x + 0.5) / 0.5f);
            float tileZ = floor(-pos2.z / 0.5f) + shift;
       		//pos += vec3(0.0, 0.0, -1.0) * time * max(ceil(-1.0*sin(time)), 0.0) / 5.0;
            float f = checkersGradBox( time, 1.7*pos.xz, 1.7*dpdx.xz, 2.5*dpdy.xz /* blur */);
            col = 0.15 + f*vec3(0.05); // luminance + f * contrast
            ks = 0.4; // godray ?
            float f1 = floor(mod((pos2.x + 0.5 * shift) / 0.5, 2.0));
            float f2 = clamp(floor(mod((pos2.z + 0.5 * f1) / 0.5, 2.0)), 0.15, 0.25);
            col = vec3(f2, f2, f2);
            /**
             * tileX <= 0.0 && tileZ <= 0.0 : only tiles behind the cube
             * abs(tileX - tileZ) <= 1.0 : only the diagonal
             * tileZ >= tileX : one tile up, then one left, then one up, ...
             */
            if (tileX <= 0.0 && tileZ <= 0.0 && abs(tileX - tileZ) <= 1.0 && tileZ >= tileX)
            {
                /**
                 * tileX + tileZ : initial color of the tile
                 * + face : shift by the current cube face that is down
                 * - shift : shift by one for odd rows
                 */
             	col = CubeFaces[int(mod(tileX + tileZ + face - shift, 6.0))];   
            }
        }

        // lighting
        float occ = calcAO( time, pos, nor );
        
		vec3 lin = vec3(0.0);

        // sun
        {
            vec3  lig = normalize( vec3(-0.5, 0.4, -0.6) );
            vec3  hal = normalize( lig-rd );
            float dif = clamp( dot( nor, lig ), 0.0, 1.0 );
          //if( dif>0.0001 )
        	      dif *= calcSoftshadow(time, pos, lig, 0.02, 2.5 );
			float spe = pow( clamp( dot( nor, hal ), 0.0, 1.0 ),16.0);
                  spe *= dif;
                  spe *= 0.04+0.96*pow(clamp(1.0-dot(hal,lig),0.0,1.0),5.0);
            lin += col*2.20*dif*vec3(1.30,1.00,0.70);
            lin +=     5.00*spe*vec3(1.30,1.00,0.70)*ks;
        }
        // sky
        {
            float dif = sqrt(clamp( 0.5+0.5*nor.y, 0.0, 1.0 ));
                  dif *= occ;
            float spe = smoothstep( -0.2, 0.2, ref.y );
                  spe *= dif;
                  spe *= 0.04+0.96*pow(clamp(1.0+dot(nor,rd),0.0,1.0), 5.0 );
          //if( spe>0.001 )
                  spe *= calcSoftshadow(time, pos, ref, 0.02, 2.5 );
            lin += col*0.60*dif*vec3(0.40,0.60,1.15);
            lin +=     2.00*spe*vec3(0.40,0.60,1.30)*ks;
        }
        // back
        {
        	float dif = clamp( dot( nor, normalize(vec3(0.5,0.0,0.6))), 0.0, 1.0 )*clamp( 1.0-pos.y,0.0,1.0);
                  dif *= occ;
        	lin += col*0.55*dif*vec3(0.25,0.25,0.25);
        }
        // sss
        {
            float dif = pow(clamp(1.0+dot(nor,rd),0.0,1.0),2.0);
                  dif *= occ;
        	lin += col*0.25*dif*vec3(1.00,1.00,1.00);
        }
        
		col = lin;

        col = mix( col, vec3(0.7,0.7,0.9), 1.0-exp( -0.0001*t*t*t ) );
    }

	return vec3( clamp(col,0.0,1.0) );
}

mat3 setCamera( in vec3 ro, in vec3 ta, float cr )
{
	vec3 cw = normalize(ta-ro);
	vec3 cp = vec3(sin(cr), cos(cr),0.0);
	vec3 cu = normalize( cross(cw,cp) );
	vec3 cv =          ( cross(cu,cw) );
    return mat3( cu, cv, cw );
}

void mainImage( out vec4 fragColor, in vec2 fragCoord )
{
	float time = iTime*4.0*SPEED;

    // camera	
    vec3 ta = vec3( 0.0, 0.0, 0.0 );
    vec3 ro = ta + vec3( 2.5*cos(0.1/**time*/), 1.3, 2.5*sin(0.1/**time */) );
    // camera-to-world transformation
    mat3 ca = setCamera( ro, ta, 0.0 );

    vec3 tot = vec3(0.0);
#if AA>1
    for( int m=ZERO; m<AA; m++ )
    for( int n=ZERO; n<AA; n++ )
    {
        // pixel coordinates
        vec2 o = vec2(float(m),float(n)) / float(AA) - 0.5;
        vec2 p = (2.0*(fragCoord+o)-iResolution.xy)/iResolution.y;
#else    
        vec2 p = (2.0*fragCoord-iResolution.xy)/iResolution.y;
#endif

        // ray direction
        vec3 rd = ca * normalize( vec3(p,2.5) );

         // ray differentials
        vec2 px = (2.0*(fragCoord+vec2(1.0,0.0))-iResolution.xy)/iResolution.y;
        vec2 py = (2.0*(fragCoord+vec2(0.0,1.0))-iResolution.xy)/iResolution.y;
        vec3 rdx = ca * normalize( vec3(px,2.5) );
        vec3 rdy = ca * normalize( vec3(py,2.5) );
        
        // render	
        vec3 col = render( time, ro, rd, rdx, rdy );

        // gain
        // col = col*3.0/(2.5+col);
        
		// gamma
        col = pow( col, vec3(0.4545) );

        tot += col;
#if AA>1
    }
    tot /= float(AA*AA);
#endif
    
    fragColor = vec4( tot, 1.0 );
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


