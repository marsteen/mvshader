//***************************************************************************
//
//
// @PROJECT  :  Basic SDL
// @VERSION  :  1.0
// @FILENAME :  CSdlApp.cpp
// @DATE     :  8.1.2015
//
// @AUTHOR   :  Martin Steen
// @EMAIL    :  martin@martin-steen.de
//
//
//***************************************************************************


#include <cstdlib>
#include <iostream>
#include <vector>
#include <string>

#include <GL/glew.h>
#include <NStringTool.h>
#include <CSdlApp.h>


using namespace std;


// ---------------------------------------------------------------------------
//
// KLASSE        : CSdlApp
// METHODE       : CSdlApp
//
//
//
// ---------------------------------------------------------------------------

CSdlApp::CSdlApp()
{
    mFullscreen = false;
    //mMouseFaktor       = 10.0;
    mInitFlag = false;
    mSdlWindow = NULL;
    mLeftMouseButton = false;
    mRightMouseButton = false;
    mMiddleMouseButton = false;
}


//---------------------------------------------------------------------------
//
// METHODE       : GlobalShowOpenGLError
//
// KLASSE        : global
//
//---------------------------------------------------------------------------


void GlobalShowOpenGLError(const char* Titel)
{
    int GlError = glGetError();

    if (GlError != GL_NO_ERROR)
    {
        std::string ErrString;
        const char* ErrText = "?";


        switch (GlError)
        {
            case GL_INVALID_ENUM:

                ErrText = "OpenGL Error: GL_INVALID_ENUM";
                break;


            case GL_INVALID_VALUE:

                ErrText = "OpenGL Error: GL_INVALID_VALUE";
                break;

            case GL_INVALID_OPERATION:

                ErrText = "OpenGL Error: GL_INVALID_OPERATION";
                break;

            case GL_STACK_OVERFLOW:

                ErrText = "OpenGL Error: GL_STACK_OVERFLOW";
                break;

            case GL_STACK_UNDERFLOW:

                ErrText = "OpenGL Error: GL_STACK_UNDERFLOW";
                break;

            case GL_OUT_OF_MEMORY:

                ErrText = "OpenGL Error: GL_OUT_OF_MEMORY";
                break;

/*
 *          case GL_TABLE_TOO_LARGE:
 *
 *              ErrText = "OpenGL Error: GL_TABLE_TOO_LARGE";
 *              break;
 */
        }
        if (Titel != NULL)
        {
            ErrString = string(Titel) + " " + ErrText;
        }
        else
        {
            ErrString = ErrText;
        }

/*
 *      GlobalDebug("**********************************************************", DBG_INIT);
 *  GlobalDebug(ErrString.c_str(), DBG_INIT);
 *  GlobalDebug("**********************************************************", DBG_INIT);
 */
    }
}


//---------------------------------------------------------------------------
//
// METHODE       : ShowOpenGLError
//
// KLASSE        : CSdlApp
//
//---------------------------------------------------------------------------


void CSdlApp::ShowOpenGLError(const char* Titel)
{
    int GlError = glGetError();

    if (GlError != GL_NO_ERROR)
    {
        std::string ErrString;
        const char* ErrText = "?";


        switch (GlError)
        {
            case GL_INVALID_ENUM:

                ErrText = "OpenGL Error: GL_INVALID_ENUM";
                break;


            case GL_INVALID_VALUE:

                ErrText = "OpenGL Error: GL_INVALID_VALUE";
                break;

            case GL_INVALID_OPERATION:

                ErrText = "OpenGL Error: GL_INVALID_OPERATION";
                break;

            case GL_STACK_OVERFLOW:

                ErrText = "OpenGL Error: GL_STACK_OVERFLOW";
                break;

            case GL_STACK_UNDERFLOW:

                ErrText = "OpenGL Error: GL_STACK_UNDERFLOW";
                break;

            case GL_OUT_OF_MEMORY:

                ErrText = "OpenGL Error: GL_OUT_OF_MEMORY";
                break;

/*
 *          case GL_TABLE_TOO_LARGE:
 *
 *              ErrText = "OpenGL Error: GL_TABLE_TOO_LARGE";
 *              break;
 */
        }
        if (Titel != NULL)
        {
            ErrString = string(Titel) + " " + ErrText;
        }
        else
        {
            ErrString = ErrText;
        }

/*
 *      GlobalDebug("**********************************************************", DBG_INIT);
 *  GlobalDebug(ErrString.c_str(), DBG_INIT);
 *  GlobalDebug("**********************************************************", DBG_INIT);
 */
    }
}


// ---------------------------------------------------------------------------
//
// KLASSE        : CSdlApp
// METHODE       : ParseArgVec
//
// ---------------------------------------------------------------------------


void CSdlApp::ParseArgVec()
{
    cout << "args=" << mCmdArgVec.size() << endl;
    for (int i = 0; i < mCmdArgVec.size(); i++)
    {
        cout << "argstr=" << mCmdArgVec[i] << endl;
        if (mCmdArgVec[i] == "-w1280x720")
        {
            mFullscreen = false;
            mXres = 1280;
            mYres = 720;
        }
        else
        if (mCmdArgVec[i] == "-w1920x1080")
        {
            mFullscreen = false;
            mXres = 1920;
            mYres = 1080;
        }
    }
}


// ---------------------------------------------------------------------------
//
// KLASSE        : CSdlApp
// METHODE       : ParseArgs
//
// ---------------------------------------------------------------------------

void CSdlApp::ParseArgs(int argc, char* argv[])
{
    mFullscreen = true;
    for (int i = 1; i < argc; i++)
    {
        mCmdArgVec.push_back(argv[i]);
    }
    ParseArgVec();
}


// ---------------------------------------------------------------------------
//
// KLASSE        : CSdlApp
// METHODE       : ParseWinArgs
//
//
//
// ---------------------------------------------------------------------------

void CSdlApp::ParseWinArgs(const char* Commandline)
{
    mFullscreen = true;
    NStringTool::Split(Commandline, &mCmdArgVec, ' ');

    if (mCmdArgVec.size() == 0)
    {
        mCmdArgVec.push_back(Commandline);
    }

    //GlobalDebugT("Commandline=", Commandline, DBG_INIT);
    //GlobalDebugT("args=", SplitResult.size(), DBG_INIT);



    ParseArgVec();
    cout << "args:" << mCmdArgVec.size() << endl;
    for (int i = 0; i < mCmdArgVec.size(); i++)
    {
        cout << "    " << mCmdArgVec[i] << endl;
    }


    cout << "fullscreen=" << mFullscreen << endl;

    /*
     * for (int i = 0; i < SplitResult.size(); i++)
     * {
     *  ParseArg(SplitResult[i]);
     * }
     */
}


// ---------------------------------------------------------------------------
//
// KLASSE        : CSdlApp
// METHODE       : GameLoop
//
//
//
// ---------------------------------------------------------------------------

void CSdlApp::GameLoop()
{
}


// ---------------------------------------------------------------------------
//
// KLASSE        : CSdlApp
// METHODE       :
//
//
//
// ---------------------------------------------------------------------------

static Uint32 sTimerCallback(Uint32 interval, void* param)
{
    CSdlApp* SdlApp = (CSdlApp*)param;

    SdlApp->Timer();

    return interval;
}


// ---------------------------------------------------------------------------
//
// KLASSE        : CSdlApp
// METHODE       : Timer
//
//
//
// ---------------------------------------------------------------------------

void CSdlApp::Timer()
{
}


// ---------------------------------------------------------------------------
//
// KLASSE        : CSdlApp
// METHODE       : SetResolution
//
//
//
// ---------------------------------------------------------------------------

void CSdlApp::SetResolution(int w, int h)
{
}


// ---------------------------------------------------------------------------
//
// KLASSE        : CSdlApp
// METHODE       :
//
//
//
// ---------------------------------------------------------------------------

void CSdlApp::InitGame()
{
}


// ---------------------------------------------------------------------------
//
// KLASSE        : CSdlApp
// METHODE       : FinishGame
//
//
//
// ---------------------------------------------------------------------------

void CSdlApp::FinishGame()
{
    SDL_ShowCursor(true);
}


// ---------------------------------------------------------------------------
//
// KLASSE        : CSdlApp
// METHODE       :
//
//
//
// ---------------------------------------------------------------------------

bool CSdlApp::Init()
{
    bool r = true;

    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        //GlobalDebug("***** SDL konnte nicht initialisiert werden", DBG_INIT);
        r = false;
    }
    SDL_ShowCursor(false); // Mousecursor verstecken
    SDL_SetRelativeMouseMode(SDL_TRUE);


    /*cout << "GL_VERSION :" << glGetString(GL_VERSION) << endl;
    cout << "GL_VENDOR  :" << glGetString(GL_VENDOR) << endl;
    cout << "GL_RENDERER:" << glGetString(GL_RENDERER) << endl;
    */

    return r;
}


// ---------------------------------------------------------------------------
//
// KLASSE        : CSdlApp
// METHODE       : SetViewport
//
//
//
// ---------------------------------------------------------------------------

void CSdlApp::SetViewport(int w, int h)
{
    mWindowWidth = w;
    mWindowHeight = h;

    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

    //glViewport(0, 0, mWindowWidth, mWindowHeight);
    mAspect = ((float)mWindowWidth) / mWindowHeight;

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDisable(GL_DEPTH_TEST);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
}


// ---------------------------------------------------------------------------
//
// KLASSE        : CSdlApp
// METHODE       : InitScreen
//
//
//
// ---------------------------------------------------------------------------

bool CSdlApp::InitScreen()
{
    bool r = true;

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);



    if (mFullscreen)
    {
        mSdlWindow = SDL_CreateWindow(
            "basicsdl",
            0,
            0,
            0, 0,
            SDL_WINDOW_FULLSCREEN_DESKTOP | SDL_WINDOW_OPENGL);
    }
    else
    {
        mSdlWindow = SDL_CreateWindow(
            "basicsdl",
            100,
            100,
            mXres,
            mYres,
            SDL_WINDOW_OPENGL);
    }


    SDL_GLContext maincontext = SDL_GL_CreateContext(mSdlWindow);


    if (mFullscreen)
    {
        SDL_DisplayMode DisplayMode;
        SDL_GetCurrentDisplayMode(0, &DisplayMode);
        mXres = DisplayMode.w;
        mYres = DisplayMode.h;

        cout << "mXres=" << mXres << endl;
        cout << "mYres=" << mYres << endl;
    }

    if (mSdlWindow == NULL)
    {
        //GlobalDebug("***** mSdlWindow == NULL", DBG_INIT);
    }
    SDL_GL_SetSwapInterval(1);

    SetViewport(mXres, mYres);
    mInitFlag = true;

    if (mSdlWindow == NULL)
    {
        //GlobalDebugT("Fehler beim Umschalten der Aufloesung ", SDL_GetError(), DBG_INIT);
        r = false;
    }
    InitGame();
    mTimer = SDL_AddTimer(10, sTimerCallback, this);

    return r;
}


// ---------------------------------------------------------------------------
//
// KLASSE        : CSdlApp
// METHODE       : DisableKeyRepeat
//
//
//
// ---------------------------------------------------------------------------

void CSdlApp::DisableKeyRepeat()
{
    //SDL_EnableKeyRepeat(0, 0);
}


// ---------------------------------------------------------------------------
//
// KLASSE        : CSdlApp
// METHODE       : ParseKeys
//
//
//
// ---------------------------------------------------------------------------

bool CSdlApp::ParseKeys(int key, bool down)
{
    return true;
}


// ---------------------------------------------------------------------------
//
// KLASSE        : CSdlApp
// METHODE       : MainLoop
//
//
//
// ---------------------------------------------------------------------------

void CSdlApp::MainLoop()
{
    while (1)
    {
        Uint32 StartTime = SDL_GetTicks();
        EventLoop();
        GameLoop();

        Uint32 EndTime = SDL_GetTicks();
        int DelayTime = 20 - (EndTime - StartTime);
        if (DelayTime > 0)
        {
            //cout << "DelayTime=" << DelayTime << endl;
            SDL_Delay(DelayTime);
        }
    }
}


// ---------------------------------------------------------------------------
//
// KLASSE        : CSdlApp
// METHODE       : ParseMouseRel
//
//
//
// ---------------------------------------------------------------------------

void CSdlApp::ParseMouseRel(int xrel, int yrel)
{
}


// ---------------------------------------------------------------------------
//
// KLASSE        : CSdlApp
// METHODE       : ParseMouseAbs
//
//
//
// ---------------------------------------------------------------------------

void CSdlApp::ParseMouseAbs(int xrel, int yrel)
{
}


// ---------------------------------------------------------------------------
//
// KLASSE        : CSdlApp
// METHODE       : EventLoop
//
//
//
// ---------------------------------------------------------------------------

void CSdlApp::EventLoop()
{
    SDL_Event event;

    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
            case SDL_USEREVENT:

                //HandleUserEvents(&event);
                break;

            case SDL_KEYDOWN:

                ParseKeys(event.key.keysym.sym, true);
                break;

            case SDL_KEYUP:

                if (!ParseKeys(event.key.keysym.sym, false))
                {
                    FinishGame();
                    SDL_Quit();
                    exit(0);
                }
                break;

            case SDL_MOUSEMOTION:
            {
                int xrel = event.motion.xrel;
                int yrel = event.motion.yrel;
                ParseMouseRel(xrel, yrel);
                
                //cout << "xrel=" << xrel << " yrel=" << yrel << endl;

                int xabs = event.motion.x;
                int yabs = event.motion.y;
                ParseMouseAbs(xabs, yabs);
            }
            break;

            case SDL_MOUSEWHEEL:

                MouseWheel(event.wheel.y < 0);
                break;


            case SDL_MOUSEBUTTONDOWN:

                if (event.button.button == SDL_BUTTON_LEFT)
                {
                    mLeftMouseButton = true;
                    LeftMouseButtonAction(true);
                }
                else
                if (event.button.button == SDL_BUTTON_RIGHT)
                {
                    mRightMouseButton = true;
                    RightMouseButtonAction(true);
                }
                else
                if (event.button.button == SDL_BUTTON_MIDDLE)
                {
                    mMiddleMouseButton = true;
                    MiddleMouseButtonAction(true);
                }

                // Handle mouse clicks here.
                break;

            case SDL_MOUSEBUTTONUP:

                if (event.button.button == SDL_BUTTON_LEFT)
                {
                    mLeftMouseButton = false;
                    LeftMouseButtonAction(false);
                }
                else
                if (event.button.button == SDL_BUTTON_RIGHT)
                {
                    mRightMouseButton = false;
                    RightMouseButtonAction(false);
                }
                else
                if (event.button.button == SDL_BUTTON_MIDDLE)
                {
                    mMiddleMouseButton = false;
                    MiddleMouseButtonAction(false);
                }
                break;

            case SDL_QUIT:

                FinishGame();
                SDL_Quit();
                exit(0);
                //done = true;
                break;

            default:

                break;
        }   // End switch
    }       // End while
}
