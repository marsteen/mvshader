//***************************************************************************
//
//
// @PROJECT  :	Basic SDL
// @VERSION  :	1.0
// @FILENAME :	CGL_Shader.cpp
// @DATE     :	8.1.2015
//
// @AUTHOR   :	Martin Steen
// @EMAIL    :	martin@martin-steen.de
//
//
//***************************************************************************

#include <cstdio>
#include <cstdlib>
#include <CGL_Shader.h>
#include <NGlobalLog.h>
#include <CFileIO.h>
#include <template_macros.hpp>
#include <MathDefines.h>

using namespace NGlobalLog;


extern void checkGlError(const char* op, const char* name);
extern bool checkGlError(const char* op);






//---------------------------------------------------------------------------
//
// Klasse:  CGL_Shader
// Methode: CGL_Shader
//
//
//---------------------------------------------------------------------------

CGL_Shader::CGL_Shader() 
{
  mProgram     = 0;
  mVertAttrib  = 0;
  mTextAttrib  = 0;
  mColorAttrib = 0;
  mName = "No Name";
} 

//---------------------------------------------------------------------------
//
// Klasse:  CGL_Shader
// Methode: InitAttribs
//
//
//---------------------------------------------------------------------------


void CGL_Shader::InitAttribs()
{
	mVertAttrib  = glGetAttribLocation(mProgram, "aVertAttrib");
	checkGlError("glGetAttribLocation VertAttrib");
	mTextAttrib  = glGetAttribLocation(mProgram, "aTextAttrib");
	checkGlError("glGetAttribLocation TextAttrib");
	mColorAttrib = glGetAttribLocation(mProgram, "aColorAttrib");		
	checkGlError("glGetAttribLocation ColorAttrib");
	
	
	
	if (mVertAttrib == -1)
	{
	   gdstr << "Attrib not found:" << mVertAttrib;
	   gdlog();
	
	}
	
	gdstr << "mVertAttrib=" << mVertAttrib;
	gdlog();
	gdstr << "mTextAttrib=" << mTextAttrib;
	gdlog();
	gdstr << "mColorAttrib=" << mColorAttrib;
	gdlog();
	
	
}

//---------------------------------------------------------------------------
//
// Klasse:  CGL_Shader
// Methode: InitVars
//
//
//---------------------------------------------------------------------------


void CGL_Shader::InitVars() const
{


}


//---------------------------------------------------------------------------
//
// Klasse:  CGL_Shader
// Methode: InitShaderModes
//
//
//---------------------------------------------------------------------------



void CGL_Shader::InitShaderModes(const char** Uniloc)
{
	gdstr << "CGL_Shader::InitShaderModes START";
	gdlog();

	mShaderLoc.clear();
	
	for (int i = 0; Uniloc[i] != NULL; i++)
	{	
		int uniloc =  glGetUniformLocation(mProgram, Uniloc[i]);
		
		
		gdstr << "  " << Uniloc[i] << " = " << uniloc;
		gdlog();
		
		mShaderLoc.push_back(uniloc);
	}	
	
	gdstr << "CGL_Shader::InitShaderModes OK";
	gdlog();
	
}



//---------------------------------------------------------------------------
//
// Klasse:  CGL_Shader
// Methode: GetUniformLocation
//
//
//---------------------------------------------------------------------------

GLint CGL_Shader::GetUniformLocation(const char* UniName) const
{
	return glGetUniformLocation(mProgram, UniName);
}


//---------------------------------------------------------------------------
//
// Klasse:  CGL_Shader
// Methode: SetUniformVec2
//
//
//---------------------------------------------------------------------------

bool CGL_Shader::SetUniformVec2(const char* UniName, const float* v) const
{
	bool r;
  GLint val = glGetUniformLocation(mProgram, UniName);
	checkGlError("glGetUniformLocation");

  
  if (val == -1)
  {
  	// Fehler
  	gdstr << "Uniform not found:" << UniName;
  	gderr();  	
  	r = false;

  }
  else
  {
  	r = true;
  	glUniform2f(val, v[0], v[1]);
 		checkGlError("glUniform2f");
  }
  return r;
}


//---------------------------------------------------------------------------
//
// Klasse:  CGL_Shader
// Methode: SetUniformVec3
//
//
//---------------------------------------------------------------------------

bool CGL_Shader::SetUniformVec3(const char* UniName, const float* v) const
{
	bool r;
  GLint val = glGetUniformLocation(mProgram, UniName);
	checkGlError("glGetUniformLocation", UniName);

  
  if (val == -1)
  {
  	// Fehler
  	gdstr << "Uniform not found:" << UniName;
  	gderr();  	
  	r = false;

  }
  else
  {
  	r = true;
  	glUniform3f(val, v[0], v[1], v[2]);
 		checkGlError("glUniform2f", UniName);
  }
  return r;
}


//---------------------------------------------------------------------------
//
// Klasse:  CGL_Shader
// Methode: SetUniformVec4
//
//
//---------------------------------------------------------------------------

bool CGL_Shader::SetUniformVec4(const char* UniName, const float* v) const
{
	bool r;
  GLint val = glGetUniformLocation(mProgram, UniName);
	checkGlError("glGetUniformLocation", UniName);

  
  if (val == -1)
  {
  	// Fehler
  	gdstr << "Uniform not found:" << UniName;
  	gderr();  	
  	r = false;

  }
  else
  {
  	r = true;
  	glUniform4f(val, v[0], v[1], v[2], v[3]);
 		checkGlError("SetUniformVec4", UniName);
  }
  return r;
}




//---------------------------------------------------------------------------
//
// Klasse:  CGL_Shader
// Methode: SetUniformInt
//
//
//---------------------------------------------------------------------------

bool CGL_Shader::SetUniformInt(const char* UniName, int i) const
{
	bool r;
  GLint val = glGetUniformLocation(mProgram, UniName);
  checkGlError("glGetUniformLocation", UniName);
  
  if (val == -1)
  {
  	// Fehler
  	gdstr << "Uniform not found:" << UniName;
  	gderr();
  	
  	r = false;

  }
  else
  {
  	r = true;
  	glUniform1i(val, i);
  	checkGlError("SetUniformInt", UniName);
  }
  return r;
}

//---------------------------------------------------------------------------
//
// Klasse:  CGL_Shader
// Methode: SetUniformFloat
//
//
//---------------------------------------------------------------------------

bool CGL_Shader::SetUniformFloat(const char* UniName, float f) const
{
	bool r;
  GLint val = glGetUniformLocation(mProgram, UniName);
  checkGlError("glGetUniformLocation", UniName);
  
  if (val == -1)
  {
  	// Fehler
  	gdstr << "Uniform not found:" << UniName;
  	gderr();
  	
  	r = false;

  }
  else
  {
  	r = true;
  	glUniform1f(val, f);
  	checkGlError("SetUniformInt", UniName);
  }
  return r;
}




void CGL_Shader::SetUniformInt(int uloc, int i) const               
{ 	glUniform1i(mShaderLoc[uloc], i); }

void CGL_Shader::SetUniformMatrix(int uloc, const float* mat) const 
{ 	glUniformMatrix4fv(mShaderLoc[uloc], 1, GL_FALSE, mat); }

void CGL_Shader::SetUniformVec4(int uloc, const float* v) const 
{ 	glUniform4fv(mShaderLoc[uloc], 1, v); }

void CGL_Shader::SetUniformVec3(int uloc, const float* v) const 
{ 	glUniform3fv(mShaderLoc[uloc], 1, v); }

void CGL_Shader::SetUniformVec2(int uloc, const float* v) const 
{ 	glUniform2fv(mShaderLoc[uloc], 1, v); }


void CGL_Shader::SetUniformFloat(int uloc, const float f) const 
{ 	glUniform1f(mShaderLoc[uloc], f); }





//---------------------------------------------------------------------------
//
// Klasse:  CGL_Shader
// Methode: SetUniformMatrix
//
//
//---------------------------------------------------------------------------

bool CGL_Shader::SetUniformMatrix(const char* UniName, const float* mat) const
{
	bool r;
  GLint val = glGetUniformLocation(mProgram, UniName);
 	checkGlError("glGetUniformLocation", UniName);

  if (val == -1)
  {
  	gdstr << "Uniform not found:" << UniName;
  	gderr();  
  	// Fehler
  	r = false;
  }
  else
  {
  	r = true;
  	glUniformMatrix4fv(val, 1, GL_FALSE, mat);
  	checkGlError("glUniformMatrix4fv", UniName);
  }
  return r;

}



//---------------------------------------------------------------------------
//
// Klasse:  CGL_Shader
// Methode: UseProgram
//
//
//---------------------------------------------------------------------------

void CGL_Shader::UseProgram() const
{
	glUseProgram(mProgram);
	checkGlError("glUseProgram");
}

//---------------------------------------------------------------------------
//
// Klasse:  CGL_Shader
// Methode: DeleteProgram
//
//
//---------------------------------------------------------------------------

void CGL_Shader::DeleteProgram()
{
	glDeleteProgram(mProgram);
	checkGlError("glDeleteProgram");

}
	
//---------------------------------------------------------------------------
//
// Klasse:  CGL_Shader
// Methode: LoadShaderFile
//
//
//---------------------------------------------------------------------------

std::string CGL_Shader::LoadShaderFile(const std::string& ShaderFilename)
{
    const char* ShaderFileData = (const char*) CFileIO::ReadFile(ShaderFilename.c_str(), nullptr);
    std::string ShaderFileStr = ShaderFileData;
    delete[] ShaderFileData;
    return ShaderFileStr;
} 

 
//  

//---------------------------------------------------------------------------
//
// Klasse:  CGL_Shader
// Methode: LoadShader
// 
//
//---------------------------------------------------------------------------

GLuint CGL_Shader::LoadShader(GLenum shaderType, const char* ShaderSource) 
{

  gdstr << "CGL_Shader::LoadShader START type=" << shaderType << " size=" << strlen(ShaderSource);
  gdlog();
  
  glewInit();

	GLuint shader = glCreateShader(shaderType);
	if (shader) 
	{
		glShaderSource(shader, 1, &ShaderSource, NULL);
		glCompileShader(shader);
		GLint compiled = 0;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
		if (!compiled)
		{
			GLint infoLen = 0;
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
			if (infoLen) 
			{
				char* buf = (char*) malloc(infoLen);
				if (buf) 
				{
					glGetShaderInfoLog(shader, infoLen, NULL, buf);
				  gdstr << "CGL_Shader::LoadShader: " << ShaderSource;
				  gderr();
					
					gdstr << "Could not compile shader: " << shaderType << " " << buf;
					gderr();
					free(buf);
				}
				glDeleteShader(shader);
				shader = 0;
			}
		}
		else
		{
		  gdstr << "compile ok!";
		  gdlog();
		
		}
	}
	checkGlError("LoadShader", ShaderSource);

  gdstr << "CGL_Shader::LoadShader OK";
  gdlog();
	
	
	return shader;
}

//---------------------------------------------------------------------------
//
// Klasse:	CGL_Shader
// Methode: CreateProgram
//
//
//---------------------------------------------------------------------------

GLuint CGL_Shader::CreateProgram(const char* pVertexSource, const char* pFragmentSource) 
{
	gdstr << "CGL_Shader::CreateProgram START";
	gdlog();
	
	
	if (pVertexSource == NULL)
	{
	  gdstr << "Vertex Shader Source = NULL";
	  gdlog;	
	}
 
	
	
    GLuint vertexShader = LoadShader(GL_VERTEX_SHADER, pVertexSource);
    if (!vertexShader) 
		{
      return 0;
    }

   gdstr << "LoadShader vertexShader ok!";
	 gdlog();
  
    
    GLuint pixelShader = LoadShader(GL_FRAGMENT_SHADER, pFragmentSource);
    if (!pixelShader) 
		{
      return 0;
    }

    gdstr << "LoadShader pixelShader ok!";
  	gdlog();


    GLuint program = glCreateProgram();
    if (program)
		{
			glAttachShader(program, vertexShader);
			checkGlError("glAttachShader", pVertexSource);
			glAttachShader(program, pixelShader);
			checkGlError("glAttachShader", pFragmentSource);
			glLinkProgram(program);
			GLint linkStatus = GL_FALSE;
			glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);
			if (linkStatus != GL_TRUE) 
			{
				GLint bufLength = 0;
				glGetProgramiv(program, GL_INFO_LOG_LENGTH, &bufLength);
				if (bufLength) 
				{
					char* buf = (char*) malloc(bufLength);
					if (buf) 
					{
						glGetProgramInfoLog(program, bufLength, NULL, buf);
						gdstr << "Could not link program: "  << buf;
						gderr();
						free(buf);
					}
				}
				glDeleteProgram(program);
				program = 0;
      }
    }
  	checkGlError("CreateProgram");

		gdstr << "CGL_Shader::CreateProgram OK";
		gdlog();

    return program;
}

//---------------------------------------------------------------------------
//
// Klasse:  CGL_Shader
// Methode: InitShader
//
//
//---------------------------------------------------------------------------


bool CGL_Shader::InitShader(const char* VertexShader, const char* FragmentShader)
{
  bool r;
  
  
  gdstr << "Init Shader: " << mName;
  gdlog();


	mProgram = CreateProgram(VertexShader, FragmentShader);
	if (!mProgram)
	{
		gdstr << "Could not create program";
		gderr();
		r = false;
	}
	else
	{
   	  InitAttribs();
	  r = true;
	}		
	
	return r;
}

//---------------------------------------------------------------------------
//
// Klasse:  CGL_Shader
// Methode: 
//
//
//---------------------------------------------------------------------------

void CGL_Shader::SetUniformVec2(const char* UniName, float x, float y) const
{
  float v[2];
  v[0] = x;
  v[1] = y; 
  SetUniformVec2(UniName, v);
}

//---------------------------------------------------------------------------
//
// Klasse:  CGL_Shader
// Methode: 
//
//
//---------------------------------------------------------------------------

void CGL_Shader::SetUniformVec3(const char* UniName, float x, float y, float z) const
{
  float v[3];
  v[0] = x;
  v[1] = y;
  v[2] = z;  
  SetUniformVec3(UniName, v);
}


//---------------------------------------------------------------------------
//
// Klasse:  CGL_Shader
// Methode: SetRotation
//
//
//---------------------------------------------------------------------------


void CGL_Shader::SetRotation(float deg_angle) const
{
  float rad_angle = DEG_TO_RAD(deg_angle);  
  SetUniformVec2("uRotate",  sin(rad_angle), cos(rad_angle));
}


//---------------------------------------------------------------------------
//
// Klasse:  CGL_Shader
// Methode: SetPosition
//
//
//---------------------------------------------------------------------------

void CGL_Shader::SetPosition(float x, float y) const
{
  SetUniformVec2("uPosition",  x, y); 
}


//---------------------------------------------------------------------------
//
// Klasse:  CGL_Shader
// Methode: SetScale
//
//
//---------------------------------------------------------------------------

void CGL_Shader::SetScale(float x, float y) const
{
  SetUniformVec2("uScale",  x, y); 
}


//---------------------------------------------------------------------------
//
// Klasse:  CGL_Shader
// Methode: SetViewScale
//
//
//---------------------------------------------------------------------------

void CGL_Shader::SetViewScale(int Width, int Height) const
{
   float ViewScale[2];
   
   ViewScale[0] = 2.0f / Width;
   ViewScale[1] = 2.0f / Height;
   
   //ViewScale[0] = 1.0f;
   //ViewScale[1] = 1.0f;
   
   
   UseProgram();
   SetUniformVec2("uViewScale",  ViewScale);  
}

//---------------------------------------------------------------------------
//
// Klasse:  CGL_Shader
// Methode: SetColor
//
//
//---------------------------------------------------------------------------

void CGL_Shader::SetColor(float r, float g, float b, float a) const
{
  float Color[4];
  Color[0] = r;
  Color[1] = g;
  Color[2] = b;
  Color[3] = a;
  SetUniformVec4("uColor",  Color);
}

