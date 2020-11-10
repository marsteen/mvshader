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

// 抗锯齿开关

#define AA 2

// sdf
float sdPlane(vec3 p) {
	return p.y;
}

float sdSphere(vec3 p, float s){
	return length(p) - s;
}

// sdf的交并补
// subtraction减法
float opS(float d1, float d2){
	return max(-d2, d1);
}

// Union
vec2 opU(vec2 d1, vec2 d2) {
    return ( d1.x < d2.x ) ? d1 : d2;
}

// repetition 周期重复
vec3 opRep(vec3 p, vec3 c){
	return mod(p,c) - 0.5 * c;
}

//twist 扭曲
vec3 opTwist(vec3 p){
	float c = cos(10.0 * p.y + 10.0);
    float s = sin(10.0 * p.y + 10.0);
    mat2 m = mat2(c, -s, s, c);
    return vec3(m * p.xz, p.y);
}

vec2 map(in vec3 pos){
	// res.x 是光线到表面的距离，res.y 影响的是物体材质
	//vec2 res = vec2( sdSphere( pos-vec3( 0.0,0.25, 0.0), 0.25 ), 46.9 );
	vec2 res = opU( vec2( sdPlane( pos), 1.0 ),
			vec2( sdSphere( pos-vec3( 0.0,0.25, 0.0), 0.25 ), 48.0 ) );
    
    return res;

}

// 实际是 Ray-marching
vec2 castRay( in vec3 ro, in vec3 rd )
{
	float tmin = 1.0;
	float tmax = 20.0;
 
#if 1   // 加速 Raymarching
	// bounding volume
	float tp1 = (0.0-ro.y)/rd.y;
	if( tp1>0.0 )
		tmax = min( tmax, tp1 );
 
	float tp2 = (1.6-ro.y)/rd.y;
	if( tp2>0.0 )
	{
		if( ro.y>1.6 )
			tmin = max( tmin, tp2 );
		else
			tmax = min( tmax, tp2 );
	}
#endif
 
	float t = tmin;
	float m = -1.0;
	for( int i=0; i<64; i++ )
	{
		float precis = 0.0005*t;
		vec2 res = map( ro+rd*t );
		if( res.x<precis || t>tmax )
			break;
		t += res.x;
		m = res.y;	// 唯一用到 res.y 的地方，影响材质的计算
	}
 
	if( t>tmax )
		m=-1.0;
	return vec2( t, m );
}
 
// 柔化阴影
// http://www.iquilezles.org/www/articles/rmshadows/rmshadows.htm
float softshadow( in vec3 ro, in vec3 rd, in float mint, in float tmax )
{
	float res = 1.0;
	float t = mint;
	for( int i=0; i<16; i++ )
	{
		float h = map( ro + rd*t ).x;
		res = min( res, 8.0*h/t );
		t += clamp( h, 0.02, 0.10 );
		if( h<0.001 || t>tmax )	// 在[ mint, maxt)范围内进行插值
			break;
	}
	return clamp( res, 0.0, 1.0 );
}
 
// 法线
vec3 calcNormal( in vec3 pos )
{
	vec2 e = vec2(1.0,-1.0)*0.5773*0.0005;
	return normalize( e.xyy*map( pos + e.xyy ).x +
			e.yyx*map( pos + e.yyx ).x +
			e.yxy*map( pos + e.yxy ).x +
			e.xxx*map( pos + e.xxx ).x );
	/*
	 vec3 eps = vec3( 0.0005, 0.0, 0.0 );
	 vec3 nor = vec3(
	 map(pos+eps.xyy).x - map(pos-eps.xyy).x,
	 map(pos+eps.yxy).x - map(pos-eps.yxy).x,
	 map(pos+eps.yyx).x - map(pos-eps.yyx).x );
	 return normalize(nor);
	 */
}
 
// Ambient Occlusion: 环境光吸收/遮蔽
float calcAO( in vec3 pos, in vec3 nor )
{
	float occ = 0.0;
	float sca = 1.0;
	for( int i=0; i<5; i++ )
	{
		float hr = 0.01 + 0.12*float(i)/4.0;
		vec3 aopos = nor * hr + pos;
		float dd = map( aopos ).x;
		occ += -(dd-hr)*sca;
		sca *= 0.95;
	}
	return clamp( 1.0 - 3.0*occ, 0.0, 1.0 );
}
 
vec3 render( in vec3 ro, in vec3 rd )
{
	vec3 col = vec3(0.7, 0.9, 1.0) +rd.y*0.8;
	vec2 res = castRay(ro,rd);
	float t = res.x;
	float m = res.y;
	if( m>-0.5 )
	{
		vec3 pos = ro + t*rd;		// 步进的光线位置
		vec3 nor = calcNormal( pos );	// 法线
		vec3 ref = reflect( rd, nor );	// 反光
 
		// material
		col = 0.45 + 0.35*sin( vec3(0.05,0.08,0.10)*(m-1.0) );
		// 如果是地板的话
		if( m<1.5 )
		{
			// 格子地砖
			float f = mod( floor(5.0*pos.z) + floor(5.0*pos.x), 2.0);
			col = 0.3 + 0.1*f*vec3(1.0);
		}
 
		// 光照模型的计算
		float occ = calcAO( pos, nor );
		vec3 lig = normalize( vec3(-0.4, 0.7, -0.6) );
		float amb = clamp( 0.5+0.5*nor.y, 0.0, 1.0 );
		float dif = clamp( dot( nor, lig ), 0.0, 1.0 );
		float bac = clamp( dot( nor, normalize(vec3(-lig.x,0.0,-lig.z))), 0.0, 1.0 )*clamp( 1.0-pos.y,0.0,1.0);
		float dom = smoothstep( -0.1, 0.1, ref.y );
		float fre = pow( clamp(1.0+dot(nor,rd),0.0,1.0), 2.0 );
		float spe = pow(clamp( dot( ref, lig ), 0.0, 1.0 ),16.0);
 
		// 散射阴影
		dif *= softshadow( pos, lig, 0.02, 2.5 );
		// 反光阴影
		dom *= softshadow( pos, ref, 0.02, 2.5 );
 
		// 注意物体底座的阴影变化
		// occ = 1.;
		vec3 lin = vec3(0.0);
		lin += 1.30*dif*vec3(1.00,0.80,0.55);
		lin += 2.00*spe*vec3(1.00,0.90,0.70)*dif;
		lin += 0.40*amb*vec3(0.40,0.60,1.00)*occ;
		lin += 0.50*dom*vec3(0.40,0.60,1.00)*occ;
		lin += 0.50*bac*vec3(0.25,0.25,0.25)*occ;
		lin += 0.25*fre*vec3(1.00,1.00,1.00)*occ;
		col = col*lin;
 
		col = mix( col, vec3(0.8,0.9,1.0), 1.0-exp( -0.0002*t*t*t ) );
	}
 
	return vec3( clamp(col,0.0,1.0) );
}
 
mat3 setCamera( in vec3 ro, in vec3 ta, float cr )
{
	vec3 cw = normalize(ta-ro);		// look
	vec3 cp = vec3(sin(cr), cos(cr),0.0);	// XY Space
	vec3 cu = normalize( cross(cw,cp) );	// right
	vec3 cv = normalize( cross(cu,cw) );	// up
 
	// right,
	// up,	* world = camera
	// look
	// 注意： glsl 是按列存储，所以可以直接右乘相机坐标系的点，结果就是世界坐标系下的点
	return mat3( cu, cv, cw );	// right, up, look
}
 

uniform float iGlobalTime;
 


void mainImage( out vec4 fragColor, in vec2 fragCoord )
{
    vec2 mo = iMouse.xy / iResolution.xy;
        float time = 15.0 + iTime;

        vec3 tot = vec3(0.0);
    #if AA>1	// 开启 AA
        for (int m = 0; m < AA; m++)
        {
            for (int n = 0; n < AA; n++)
            {
                // pixel coordinates
                vec2 o = vec2(float(m), float(n)) / float(AA) - 0.5;
                vec2 p = (-iResolution.xy + 2.0 * (fragCoord + o)) / iResolution.y;
    #else
                vec2 p = (-iResolution.xy + 2.0*fragCoord)/iResolution.y;
    #endif

                // camera
                vec3 ro = vec3(-0.5 + 3.5 * cos(0.1 * time + 6.0 * mo.x),
                        1.0 + 2.0 * mo.y, 0.5 + 4.0 * sin(0.1 * time + 6.0 * mo.x));
                vec3 ta = vec3(-0.5, -0.4, 0.5);
                // camera-to-world transformation
                mat3 ca = setCamera(ro, ta, 0.0);
                // ray direction
                vec3 rd = ca * normalize(vec3(p.xy, 2.0));

                // render
                vec3 col = render(ro, rd);

                // 针对电子屏幕的 gamma 矫正（否则颜色偏暗）
                col = pow(col, vec3(0.4545));

                tot += col;
    #if AA>1
            }
        }
        tot /= float(AA * AA);
    #endif

        fragColor = vec4(tot, 1.0);

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


