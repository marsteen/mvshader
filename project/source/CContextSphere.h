//***************************************************************************
//
//
// @PROJECT  :  Basic SDL
// @VERSION  :  1.0
// @FILENAME :  CContextSphere.h
// @DATE     :  22.5.2020
//
// @AUTHOR   :  Martin Steen
// @EMAIL    :  martin@martin-steen.de
//
//
//***************************************************************************

#ifndef CContextSphere_H
#define CContextSphere_H

#include <vector>
#include <string>
#include <CGL_Context.h>
#include <CGL_Shader_Sphere.h>

class CContextSphere : public CGL_Context
{
public:

    CContextSphere();

    void Draw2D();
    bool Init(int w, int h, const std::vector<std::string>* cmdlineparams = nullptr);
    void MouseMove(int x, int y, bool lb, bool rb);
    void MouseMoveAbs(int x, int y, bool lb, bool rb);
    void KeyPress(int key, bool down);
    void MouseWheel(bool up);
    void Timer();

protected:

    void ParseParams(const std::vector<std::string>* cmdlineparams);

    CGL_Shader_Sphere* mSphereShader;
    CGL_Shader_Sphere* mTexturShader;
    std::string mVertShaderFile;
    std::vector<std::string> mFragShaderFileVec;
    int mAktShaderNr;

    CFrameBuffer* mFrameBuffer;
    int mFbResW;
    int mFbResH;
    bool mUseFrameBuffer;
};

#endif
