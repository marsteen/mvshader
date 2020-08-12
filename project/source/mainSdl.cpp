//***************************************************************************
//
//
// @PROJECT  :	Basic SDL
// @VERSION  :	1.0
// @FILENAME :	mainSdl.cpp
// @DATE     :	8.1.2015
//
// @AUTHOR   :	Martin Steen
// @EMAIL    :	martin@martin-steen.de
//
//
//***************************************************************************

#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <SDL2/SDL.h>
#include <iostream>
#include <NStringTool.h>
#include <CSDLblurline.h>

const char* GlobalArg0;
const char* GlobalWindowName = "SDLlissa";


using namespace std;


// ---------------------------------------------------------------------------
//
//
//
//
// ---------------------------------------------------------------------------

#ifdef _WIN32

// ---------------------------------------------------------------------------
//
// WINDOWS
//
// ---------------------------------------------------------------------------


#include <windows.h>
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
				   LPSTR lpCmdLine, int iCmdShow)
{
	CSdlApp* SdlApp = new CSDLblurline;
	static string CmdStr = NStringTool::Trim(lpCmdLine, "\"");

	GlobalArg0 = CmdStr.c_str();

	if (SdlApp->Init())
	{
		SdlApp->ParseWinArgs(GlobalArg0);
		if (SdlApp->InitScreen())
		{
			SdlApp->MainLoop();
		}
	}
	return 0;
}

#else

// ---------------------------------------------------------------------------
//
// UNIX
//
// ---------------------------------------------------------------------------

int main(int argc, char* argv[])
{
	CSdlApp* SdlApp = new CSDLblurline;

	GlobalArg0 = argv[0];

	//GlobalDebugInit();

	//GlobalDebugT("argv[0]=", argv[0], DBG_MAPS);

	#if 1
	#ifdef __APPLE__
	{
		char pa[512];
		strcpy(pa, argv[0]);
		*strrchr(pa, '/') = 0;
		string AppFolder = string(pa) + "/../AppData";
		chdir(AppFolder.c_str());
		//GlobalDebugT("AppFolder=", AppFolder, DBG_MAPS);
	}
	#endif
	#endif

	if (SdlApp->Init())
	{
		SdlApp->ParseArgs(argc, argv);
		if (SdlApp->InitScreen())
		{
			SdlApp->MainLoop();
		}
	}
	return 0;
}

#endif
