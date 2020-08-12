//***************************************************************************
//
//
// @PROJECT  :	Diercke Digital PAD
// @VERSION  :	1.0
// @FILENAME :	CFrameBuffer.cpp
// @DATE     :	12.1.2015
//
// @AUTHOR   :	Martin Steen
// @EMAIL    :	msteen@imagon.de
//
//
//***************************************************************************

#include <iostream>
#include <CGL_Shader.h>
#include <CGL_Basic.h>
#include <CFrameBuffer.h>
#include <NGlobalLog.h>

using namespace std;
using namespace NGlobalLog; 


extern void checkGlError(const char* func);
extern void checkGlErrorCritical(const char* func);


//---------------------------------------------------------------------------
//
// Klasse:  CFrameBuffer
// Methode: CreateFramebufferObject
//
//
//---------------------------------------------------------------------------

void CFrameBuffer::CreateFramebufferObject(int Width, int Height) 
{
  GlobalDebugEnabled = true;

	gdstr << "CFrameBuffer::CreateFramebufferObject START";
	gdlog();
  
	gdstr << "w=" << Width << " h=" << Height;
	gdlog();
  
  mWidth  = Width;
  mHeight = Height;
  
	mFBhandle = 0;
	
  glGenFramebuffers(1, &mFBhandle);
	checkGlErrorCritical("glGenFramebuffers");
  glBindFramebuffer(GL_FRAMEBUFFER, mFBhandle);  
  checkGlErrorCritical("glBindFramebuffer");

  //Create a color renderbuffer, allocate storage for it, and attach it to the framebuffer’s color attachment point.
  //GLuint colorRenderbuffer;

  glGenRenderbuffers(1, &mColorRenderbuffer);
	checkGlErrorCritical("glGenRenderbuffers");
  glBindRenderbuffer(GL_RENDERBUFFER, mColorRenderbuffer);
	checkGlErrorCritical("glBindRenderbuffer");
  glRenderbufferStorage(GL_RENDERBUFFER, GL_RGBA4, Width, Height);
	checkGlErrorCritical("glRenderbufferStorage");
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, mColorRenderbuffer);
	checkGlErrorCritical("glFramebufferRenderbuffer");

#if 0
  
  //Create a depth or depth/stencil renderbuffer, allocate storage for it, and attach it to the framebuffers depth attachment point.

  glGenRenderbuffers(1, &mDepthRenderbuffer);  
  glBindRenderbuffer(GL_RENDERBUFFER, mDepthRenderbuffer);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, Width, Height);  
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, mDepthRenderbuffer);
  
  
#endif  
   // Textur erzeugen

  glGenTextures(1, &mTexture);
	checkGlErrorCritical("glGenTextures");
  glBindTexture(GL_TEXTURE_2D, mTexture);
	checkGlErrorCritical("glBindTexture");
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);	   // GL_NEAREST
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);    // GL_NEAREST
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,  Width, Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	checkGlErrorCritical("glTexImage2D");
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mTexture, 0);
	checkGlErrorCritical("glFramebufferTexture2D");
  
  //Test the framebuffer for completeness. This test only needs to be performed when the framebuffers configuration changes.
  
  GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
  
  if (status != GL_FRAMEBUFFER_COMPLETE) 
  {
    GlobalDebugEnabled = true;  
		gdstr << "GL_FRAMEBUFFER_COMPLETE failed: Status=" <<  status;
		gderr();
		
    //Debug("***** GL_FRAMEBUFFER_COMPLETE failed: Status=", status);
  }
  else
  {
		gdstr << "GL_FRAMEBUFFER_COMPLETE ok";
		gdlog();
		
  }
  
 
  // Textur und Framebuffer deaktivieren:   
  
  glBindTexture(GL_TEXTURE_2D, 0);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
   
	gdstr << "CFrameBuffer::CreateFramebufferObject OK";
	gdlog();
  
  
  GlobalDebugEnabled = false;

 // Debug("CreateFramebufferObject OK");
}

//---------------------------------------------------------------------------
//
// Klasse:  CFrameBuffer
// Methode: DeleteFramebuffer
//
//
//---------------------------------------------------------------------------

void CFrameBuffer::DeleteFramebuffer()
{
	glDeleteFramebuffers(1, &mFBhandle);
	mFBhandle = 0;
	glDeleteTextures(1, &mTexture);
	mTexture = 0;
}
  	


//---------------------------------------------------------------------------
//
// Klasse:  CFrameBuffer
// Methode: DrawToFrameBuffer
//
//
//---------------------------------------------------------------------------

// Zeichnen in den Framebuffer aktivieren:

void CFrameBuffer::DrawToFrameBuffer(bool Enable)
{
  if (Enable)
  {  
  	glGetIntegerv(GL_FRAMEBUFFER_BINDING, &mOldFramebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, mFBhandle);
		
  }
  else
  {
    //glBindTexture(GL_TEXTURE_2D, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, mOldFramebuffer);   
  }
} 

//---------------------------------------------------------------------------
//
// Klasse:  CFrameBuffer
// Methode: ClearBuffer
//
//
//---------------------------------------------------------------------------

void CFrameBuffer::ClearBuffer() const
{
  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
  glClear(GL_COLOR_BUFFER_BIT);
}



//---------------------------------------------------------------------------
//
// Klasse:  CFrameBuffer
// Methode: DrawTexture
//
//
//---------------------------------------------------------------------------


// Textur zeichnen

void CFrameBuffer::DrawTexture(const CGL_Shader* glcon, float xpos, float ypos) const
{	
	glBindTexture(GL_TEXTURE_2D, mTexture); 
	CGL_Basic::DrawTextureQuad(glcon, xpos, ypos, xpos + mWidth, ypos + mHeight);
}

//---------------------------------------------------------------------------
//
// Klasse:  CFrameBuffer
// Methode: DrawTexture
//
//
//---------------------------------------------------------------------------

// Textur zeichnen

void CFrameBuffer::DrawTexture(const CGL_Shader* glcon, float x1, float y1, float x2, float y2) const
{	
	glBindTexture(GL_TEXTURE_2D, mTexture); 
	CGL_Basic::DrawTextureQuad(glcon, x1, y1, x2, y2);
}

//---------------------------------------------------------------------------
//
// Klasse:  CFrameBuffer
// Methode: SetViewport
//
//
//---------------------------------------------------------------------------

void CFrameBuffer::SetViewport()
{	
  glGetIntegerv(GL_VIEWPORT, mOldVp);
  glViewport(0, 0, mWidth, mHeight);		
}


//---------------------------------------------------------------------------
//
// Klasse:  CFrameBuffer
// Methode: RestoreViewport
//
//
//---------------------------------------------------------------------------

void CFrameBuffer::RestoreViewport() const
{	
  glViewport(mOldVp[0], mOldVp[1], mOldVp[2], mOldVp[3]);		
}






//---------------------------------------------------------------------------
//
// Klasse:  CFrameBuffer
// Methode: DrawTextureRotated
//
//
//---------------------------------------------------------------------------


void CFrameBuffer::DrawTextureRotated(const CGL_Shader* glcon, float xpos, float ypos) const
{	
	glBindTexture(GL_TEXTURE_2D, mTexture);
	CGL_Basic::DrawTextureQuadRotated(glcon, xpos, ypos, xpos + mScale * mWidth, ypos + mScale * mHeight);
}

