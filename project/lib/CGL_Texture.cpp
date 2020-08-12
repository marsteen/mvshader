//***************************************************************************
//
//
// @PROJECT  :	Basic SDL
// @VERSION  :	1.0
// @FILENAME :	CGL_Texture.cpp
// @DATE     :	8.1.2015
//
// @AUTHOR   :	Martin Steen
// @EMAIL    :	martin@martin-steen.de
//
//
//***************************************************************************


#include <iostream>
#include <fstream>

#include <GLinclude.h>
#include <CGL_Texture.h>
#include <NGlobalLog.h>

using namespace std;
using namespace NGlobalLog;



extern void checkGlError(const char* func);


//---------------------------------------------------------------------------
//
// Klasse:    CGL_Texture
// Methode:		DeleteTexture
//
// Parameter:
//
// Return:
//
//---------------------------------------------------------------------------

void CGL_Texture::DeleteTexture()
{
	if (mTexHandle != 0)
	{
		glDeleteTextures(1, &mTexHandle);
		mTexHandle = 0;
	}
}

#if 0

void CGL_Texture::SetTextureData24Bit(int width, int height, const void* data)
{
  	int	internalFormat  = GL_RGB;
		int	format          = GL_RGB; 			
    int type            = GL_UNSIGNED_BYTE;

  
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

		
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		checkGlError("glTexParameteri (GL_TEXTURE_MIN_FILTER)");


		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); //
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); //
		checkGlError("glTexParameteri (GL_TEXTURE_WRAP_T)");


		glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, type, data);
}
#endif

//---------------------------------------------------------------------------
//
// Klasse:    CGL_Texture
// Methode:		CreateTexture
//
// Parameter:
//
// Return:
//
//---------------------------------------------------------------------------

bool CGL_Texture::CreateTexture(const CDataRect* glib)
{
	//glEnable(GL_TEXTURE_2D); // Veraltet
  //checkGlError("glEnable(GL_TEXTURE_2D)");
  
  int internalFormat;
  int format;
	int type;
	
  bool r = true;

	glGenTextures(1, &mTexHandle);				// Create One Texture
  checkGlError("glGenTextures");


	//glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	
  checkGlError("glBindTexture");
	
	switch (glib->mBits)
	{
		case 16:
			
			internalFormat  = GL_R16F; //GL_R16;
			format          = GL_RED;
			type            = GL_UNSIGNED_SHORT;
			break;	
		
		
		case 24: 
			
			internalFormat  = GL_RGB;
			format          = GL_RGB; 			
			type            = GL_UNSIGNED_BYTE;
		  break;
			
		case 32: 
			
			internalFormat  = GL_RGBA;
			format          = GL_RGBA;
			type            = GL_UNSIGNED_BYTE;
			break;
		
		default:
		
		
		  //gdstr << "Invalid texture format:" << glib->mBits;
		  //gderr();
			
			r = false;
			break;
	}

	if (r)
	{
		//       GL_LINEAR
		// oder  GL_NEAREST
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

		
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		checkGlError("glTexParameteri (GL_TEXTURE_MIN_FILTER)");


		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); //
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); //
		checkGlError("glTexParameteri (GL_TEXTURE_WRAP_T)");


		glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, glib->mWidth, glib->mHeight, 0, format, type, glib->mData);
		checkGlError("glTexImage2D");
		CopyDataFrom(glib);
	}
	
	
	return r;

}




