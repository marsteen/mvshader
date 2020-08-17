//***************************************************************************
//
//
// @PROJECT  :	Basic SDL
// @VERSION  :	1.0
// @FILENAME :	NGlobalLog-Desktop.cpp
// @DATE     :	8.1.2015
//
// @AUTHOR   :	Martin Steen
// @EMAIL    :	martin@martin-steen.de
//
//
//***************************************************************************

#
#define GLOBAL_DEBUG_LEVEL    1

// Globales Debug-Object fuer Stream-Ausgabe

extern "C"
{
bool GlobalDebugEnabled = true;
};

#include <iostream>
#include <fstream>
#include <sstream>
static bool DebugInit = false;


std::stringstream gdstr;

using namespace std;


static void StaticLogStr()
{
    ofstream ofile;


    if (DebugInit)
    {
        ofile.open("debug.txt", ios::app);
    }
    else
    {
        ofile.open("debug.txt");
        DebugInit = true;
    }

    ofile << gdstr.str() << endl;
    ofile.close();
}


static void StaticErrStr()
{
    ofstream ofile;

    if (DebugInit)
    {
        ofile.open("debug.txt", ios::app);
    }
    else
    {
        ofile.open("debug.txt");
        DebugInit = true;
    }
    ofile << "*****" << gdstr.str() << endl;
    ofile.close();
}


namespace NGlobalLog
{
void gdlog()
{
    if (GlobalDebugEnabled)
    {
        StaticLogStr();
    }
    gdstr.str("");
    gdstr.clear();
}


void gderr()
{
    if (GlobalDebugEnabled)
    {
        StaticErrStr();
    }
    gdstr.str("");
    gdstr.clear();
}
}
