//---------------------------------------------------------------------------
//
// PROJECT : Basic SDL
//
//
// AUTOR   : Martin Steen
//           email: martin@martin-steen.de
//
//
//----------------------------------------------------------------------------

#include <CSDLblurline.h>
#include <CGL_Context.h>

using namespace std;

extern const char* GlobalWindowName;


// ---------------------------------------------------------------------------
//
// KLASSE        : CSDLblurline
// METHODE       : CSDLblurline
//
//
//
// ---------------------------------------------------------------------------

CSDLblurline::CSDLblurline()
{
    mContext = NULL;
}



// ---------------------------------------------------------------------------
//
// KLASSE        : CSDLblurline
// METHODE       : ParseArgVec
//
//
//
// ---------------------------------------------------------------------------

void CSDLblurline::ParseArgVec()
{
    CSdlApp::ParseArgVec();
}

// ---------------------------------------------------------------------------
//
// KLASSE        : CSDLblurline
// METHODE       : ParseMouseRel
//
//
//
// ---------------------------------------------------------------------------

void CSDLblurline::ParseMouseRel(int xrel, int yrel)
{
    mContext->MouseMove(xrel, yrel, mLeftMouseButton, mRightMouseButton);
}

// ---------------------------------------------------------------------------
//
// KLASSE        : CSDLblurline
// METHODE       : ParseMouseAbs
//
//
//
// ---------------------------------------------------------------------------

void CSDLblurline::ParseMouseAbs(int xabs, int yabs)
{
    mContext->MouseMoveAbs(xabs, yabs, mLeftMouseButton, mRightMouseButton);
}


// ---------------------------------------------------------------------------
//
// KLASSE        : CSDLblurline
// METHODE       : SetUpDrawing
//
//
//
// ---------------------------------------------------------------------------

void CSDLblurline::SetUpDrawing(float StereoOffset)
{
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    //
    // 2D Operationen
    //

}

// ---------------------------------------------------------------------------
//
// KLASSE        : CSDLblurline
// METHODE       : Timer
//
//
//
// ---------------------------------------------------------------------------

void CSDLblurline::Timer()
{
    if (mContext != nullptr)
    {
       mContext->Timer();
    }
}

// ---------------------------------------------------------------------------
//
// KLASSE        : CSDLblurline
// METHODE       : MouseWheel
//
//
//
// ---------------------------------------------------------------------------

void CSDLblurline::MouseWheel(bool up)
{
    mContext->MouseWheel(up);

}

// ---------------------------------------------------------------------------
//
// KLASSE        : CSDLblurline
// METHODE       : GameLoop
//
//
//
// ---------------------------------------------------------------------------

void CSDLblurline::GameLoop()
{


    if (mInitFlag)
    {
        glClearColor(0.0, 0.0, 0.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glDisable(GL_DEPTH_TEST);


        //
        // 2D
        //


        //StartProjectionView();

        //cout << "GameLoop" << endl;



        mContext->Draw2D();


        SDL_GL_SwapWindow(mSdlWindow);
    }
}

// ---------------------------------------------------------------------------
//
// KLASSE        : CSDLblurline
// METHODE       : InitGame
//
//
//
// ---------------------------------------------------------------------------

void CSDLblurline::InitGame()
{
  glewInit();
  mContext = CGL_Context::CreateContext();
  mContext->Init(mXres, mYres, &mCmdArgVec);

    //SDL_WM_SetCaption(GlobalWindowName, GlobalWindowName);
}


// ---------------------------------------------------------------------------
//
// KLASSE        : CSDLblurline
// METHODE       : ParseKeys
//
// ---------------------------------------------------------------------------

bool CSDLblurline::ParseKeys(int key, bool down)
{
    bool r = true;
    if (down)
    {
        switch (key)
        {
            case SDLK_ESCAPE:

                r = false; // Programm beenden
                if (mContext != NULL)
                {
                    mContext->Finish();
                }
                break;


        }

        if ((key >= SDLK_a) && (key <= SDLK_z))
        {
            mContext->KeyPress(key - SDLK_a + 'a', down);
        }
        else
        if ((key >= SDLK_0) && (key <= SDLK_9))
        {
            mContext->KeyPress(key - SDLK_0 + '0', down);
        }

    }

    return r;
}

// ---------------------------------------------------------------------------
//
// KLASSE        : CSDLblurline
// METHODE       : SetResolution
//
//
//
// ---------------------------------------------------------------------------

void CSDLblurline::SetResolution(int w, int h)
{
  if (!mInitFlag)
  {
    mInitFlag = true;
  }
}

