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

//por jorge2017a1-
#define MAX_STEPS 100
#define MAX_DIST 100.
#define MIN_DIST 0.001
#define EPSILON 0.001

vec3 light_pos1   ;
vec3 light_color1 ;
vec3 light_pos2   ;
vec3 light_color2 ;
//operacion de Union
#define opU3(d1, d2) ( d1.x < d2.x ? d1 : d2 )

float sdBox( vec3 p, vec3 b )
	{ vec3 d = abs(p) - b;   return length(max(d,0.0))+ min(max(d.x,max(d.y,d.z)),0.0); }

///----------Operacion de Distancia--------
float intersectSDF(float distA, float distB)
	{ return max(distA, distB);}

float unionSDF(float distA, float distB)
	{ return min(distA, distB);}

float differenceSDF(float distA, float distB)
	{ return max(distA, -distB);}
//-------------------------------------------

///------------------------------------
vec2 rotatev2(vec2 p, float ang)
{
    float c = cos(ang);
    float s = sin(ang);
    return vec2(p.x*c - p.y*s, p.x*s + p.y*c);
}
///------------------------------------
vec3 GetDist(vec3 p  ) 
{	
	vec3 res;
    vec3 pp=p;
    res = vec3(9999.0, -1.0,-1.0);
    
    float planeDist1 = p.y+0.0;  //piso inf
    res =opU3(res, vec3(planeDist1,-1.0,7.0)); //inf
   
    p.y=p.y-5.0;
    
    
	vec3 q=p;  //repetir
    float cz=34.0;
    q.z = mod(q.z+0.5*cz,cz)-0.5*cz;
    p=q;
	
    float anchox=19.0;
    float anchoz=17.0;
    float gruesopared=0.4;
    float sdbLatIzq= sdBox(p-vec3(-anchox,0.0,0.0), vec3(gruesopared,22.0,22.0) );
    float sdbLatDer= sdBox(p-vec3( anchox,0.0,0.0), vec3(gruesopared,22.0,22.0) );
    float sdbParedAtras=    sdBox(p-vec3(0.0,0.0,-anchoz), vec3(22.0,22.0,gruesopared) );
    float sdbParedAdelante= sdBox(p-vec3(0.0,0.0, anchoz), vec3(22.0,22.0,gruesopared) );
    
    float sdbPuertaDif= sdBox(p-vec3(0.0,0.0, 0.0), vec3(5.0,10.0,anchoz+2.0) );
    sdbParedAtras= differenceSDF(sdbParedAtras,sdbPuertaDif);
    sdbParedAdelante= differenceSDF(sdbParedAdelante,sdbPuertaDif);

    float posxmov;
    //control de puerta
    if (mod(itime, 20.0) <11.0)
     posxmov=mod(iTime,11.0)+1.5;
    
    float sdbPuertaAbreA= sdBox(p-vec3(posxmov,0.0, anchoz), vec3(5.0,10.0,0.05) );
    float sdbPuertaAbreB= sdBox(p-vec3(posxmov,0.0, -anchoz), vec3(5.0,10.0,0.05) );
    
    res =opU3(res, vec3( sdbLatIzq,-1.0,6.0)); 
    res =opU3(res, vec3( sdbLatDer,-1.0,6.0)); 
	res =opU3(res, vec3( sdbParedAtras,-1.0,5.0)); 
	res =opU3(res, vec3( sdbParedAdelante,-1.0,5.0)); 
    res =opU3(res, vec3( sdbPuertaAbreA,-1.0,9.0)); 
    res =opU3(res, vec3( sdbPuertaAbreB,-1.0,9.0)); 
   
    //res =opU3(res, vec3(1.0,0.0,MATERIAL_NO)); 
    //return (dist, id_color, id_material)
    return res;
}

///-----------------------------------------
vec3 LightShading(vec3 Normal,vec3 toLight,vec3 toEye,vec3 color)
{
    vec3 toReflectedLight=reflect(-toLight, Normal);
    vec3 diffuse = max(0.,dot(Normal,-toLight))*color;
    float specularf=max(dot(toReflectedLight, toEye),0.0);
    specularf=pow(specularf, 100.0);
    vec3 specular =specularf*color;  //corregido por shane
    return diffuse + specular;
}
//------------------------------------------------
vec3 GetNormal(vec3 p)
{
	float d = GetDist(p).x;
    vec2 e = vec2(.001, 0);
    vec3 n = d - vec3(
        GetDist(p-e.xyy).x,
        GetDist(p-e.yxy).x,
        GetDist(p-e.yyx).x);
    return normalize(n);
}
//---------actualizacion por Shane---28-may-2020    ...gracias
float RayMarch(vec3 ro, vec3 rd){
    float dO = 0.; 
    vec3 dS=vec3(9999.0,-1.0,-1.0);
    float marchCount = 0.0;
    vec3 p;
    for(int i=0; i<MAX_STEPS; i++) 
    {
    	p = ro + rd*dO;
        dS = GetDist(p);
       
        if(dO>MAX_DIST )
        {	
            mObj.dist = -1.0;
    		mObj.id_color = -1.0;
    		mObj.marchCount=marchCount;
    		mObj.id_material=-1.0;
            //return -1.0;
            break;
        }
        
        if (abs(dS.x)<MIN_DIST) 
        { break;  }
        dO += dS.x;
        marchCount++;       
    }
    
    mObj.dist = dO;
    mObj.id_color = dS.y;
    mObj.marchCount=marchCount;
    mObj.id_material=dS.z;
    mObj.normal=GetNormal(p);
    return dO;
}
//---------------------------------------------------
float GetShadow(vec3 p, vec3 plig)
{
    vec3 lightPos = plig;
    vec3 l = normalize(lightPos-p);
    vec3 n = GetNormal(p);
    float dif = clamp(dot(n, l), 0., 1.);
    float d = RayMarch(p+n*MIN_DIST*2., l );
    if(d<length(lightPos-p)) dif *= .1;
    return dif;
}

//-------------------------------
vec3 Getluz(vec3 p, vec3 ro, vec3 rd, vec3 nor , vec3 colobj ,vec3 plight_pos)
{
    vec3 lightPos = plight_pos;
    float intensity=1.0;
	const float shininess = 64.0;
	vec3 l = normalize( p-lightPos);
    vec3 v = normalize( p-ro);
    vec3 h = normalize(v + l);
    float diff = dot(nor, l);
    
    vec3 result = LightShading(nor,l,v, colobj)*intensity;
    
    if (mObj.blnShadow==true)
    {
    	float fhadow=GetShadow(p,plight_pos);
    	return result*fhadow;
     }
    else
    {  return result; }
}
//------------------------------------------------
vec3 getMaterial( vec3 pp, float id_material)
{
  vec3 col=vec3(1.0);
  vec3 p=pp;
    
        
    if (id_material==5.0)
    {   vec2 uv;
		uv=(p.xy)*17.0;
     	return  Ladrillos(uv )*0.8;
    }
    
    if (id_material==6.0)
    { 
     vec2 uv=p.zy*17.0; return  Ladrillos(uv )*0.8;
    }
    
    if (id_material==7.0)
    { 
     vec2 uv=p.zx*20.0; return pisov2(uv );
    }
    
    if (id_material==8.0)
    { 
     vec2 uv=p.xy*10.0;
     return colorfmb(uv);
    }
     
    
    if (id_material==9.0)
    { 
      vec2 uv=p.xy*5.0;
      return  mix (vec3 (0.9, 0.4, 0.2), vec3 (0.55, 0.25, 0.1),fbm2 (p.xy));
        
    }
    
}
//-------------------------------------------------

vec3 GetColorYMaterial(vec3 p,  vec3 n, vec3 ro,  vec3 rd, int id_color, float id_material)
{
   	vec3 colobj; 
    if (id_color<100)
		{ colobj=getColor(int( id_color)); }

    if (id_material>-1.0 && id_color==-1)
        { 
            colobj=vec3(0.5);
            colobj*=getMaterial(p, id_material); 
            return colobj;
        }
    return colobj;
}




///---------------------------------------------
void mainImage( out vec4 fragColor, in vec2 fragCoord )
{
   vec2 uv = (fragCoord-.5*iResolution.xy)/iResolution.y;
   mObj.uv=uv;
    float t;
    t=mod(iTime*1.0,360.0);
    itime=t;
	//mObj.blnShadow=false;
    mObj.blnShadow=true;
        
 	light_pos1= vec3(10.0, 6.0, 10.0 );  light_color1 = vec3( 1.0 );
 	light_pos2= vec3( -5.0, 6.0, -5.0 ); light_color2 = vec3( 1.0, 1.0, 1.0 );
 
    
   	float  posxmov;
     posxmov=iTime*2.0;
    
    vec3 ro=vec3(0.0,7.0,-40.0+posxmov);
    vec3 rd=normalize(vec3(uv,1.0));
    
    
    light_pos1+=ro;
    light_pos2+=ro;
    
    
    vec3 col = vec3(0);
    TObj Obj;
    mObj.rd=rd;
    mObj.ro=ro;
    
    float d = RayMarch(ro, rd);
    Obj=mObj;
    vec3 p = (ro + rd * d ); 
    
    mObj.p=p;
    mObj.dist =d;
    vec3 nor=mObj.normal;

    vec3 colobj;
    colobj=GetColorYMaterial( p, nor, ro, rd,  int( Obj.id_color), Obj.id_material);
    
    
	float dif1=1.0;
   	vec3 result;
    result=  Getluz( p,ro,rd, nor, colobj ,light_pos1);
    result+= Getluz( p,ro,rd, nor, colobj ,light_pos2);
    result/=1.25;
    col= result*dif1;
    
  	//sugerencia por dean_the_coder,
    //col *= 1.0 - pow(d / 100.0, 1.5);
    col *= 1.0 - pow(d /(MAX_DIST) , 3.5);    
    col = pow(col, vec3(1.0/2.2));  
    fragColor = vec4(col,1.0);
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


