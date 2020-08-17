#ifndef CSDLMAPVIEWER_H
#define CSDLMAPVIEWER_H

#include <CSdlApp.h>
#include <CGL_Context.h>

class CSDLblurline : public CSdlApp
{
public:

    CSDLblurline();

    void GameLoop();
    virtual void InitGame();

protected:

    bool ParseKeys(int key, bool down);
    void SetResolution(int w, int h);
    void SetUpDrawing(float StereoOffset);
    void ParseMouseRel(int xrel, int yrel);
    void ParseMouseAbs(int xrel, int yrel);
    void ParseArgVec();
    void MouseWheel(bool up);
    void Zoom(float z);
    void Timer();

    CGL_Context* mContext;
};


#endif
