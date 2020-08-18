//***************************************************************************
//
//
// @PROJECT  :	Basic SDL
// @VERSION  :	1.0
// @FILENAME :	CGL_Context.cpp
// @DATE     :	8.1.2015
//
// @AUTHOR   :	Martin Steen
// @EMAIL    :	martin@martin-steen.de
//
//
//***************************************************************************

#include <cstdio>
#include <cstdlib>
#include <CGL_Context.h>
#include <NGlobalLog.h>
#include <CFileIO.h>
#include <template_macros.hpp>
//#include "shader/shader_vert_test.cpp"
//#include "shader/shader_frag_test.cpp"


//extern const char* shader_vert;
//extern const char* shader_frag;

using namespace NGlobalLog;

// ---------------------------------------------------------------------------
//
// KLASSE        : static
// METHODE       : printGLString
//
// ---------------------------------------------------------------------------


static void printGLString(const char* name, GLenum s)
{
    const char* v = (const char*)glGetString(s);

    gdstr << "GL " << name << "=" << v;
    gdlog();
}


static const char* StaticOpenGLError(int e)
{
    switch (e)
    {
        case 0x0500:
            return "GL_INVALID_ENUM​";

            break;

        case 0x0501:
            return "GL_INVALID_VALUE​";

            break;

        case 0x0502:
            return "GL_INVALID_OPERATION​";

            break;

        case 0x0503:
            return "GL_STACK_OVERFLOW​";

            break;

        case 0x0504:
            return "GL_STACK_UNDERFLOW​";

            break;

        case 0x0505:
            return "GL_OUT_OF_MEMORY​";

            break;

        case 0x0506:
            return "GL_INVALID_FRAMEBUFFER_OPERATION​";

            break;

        case 0x8031:
            return "GL_TABLE_TOO_LARGE​1";

            break;
    }
    return "unknown";
}


// ---------------------------------------------------------------------------
//
// KLASSE        : global
// METHODE       : checkGlError
//
// ---------------------------------------------------------------------------

bool checkGlError(const char* op)
{
    bool r = false;

    for (GLint error = glGetError(); error; error = glGetError())
    {
        gdstr.setf(std::ios::hex, std::ios::basefield);
        gdstr	<< "after " << op << " glError: 0x" << error
                << " (" << StaticOpenGLError(error) << ") ";
        gdlog();
        gdstr.setf(std::ios::dec, std::ios::basefield);

        r = true;
    }
    return r;
}


// ---------------------------------------------------------------------------
//
// KLASSE        : global
// METHODE       : checkGlErrorCritical
//
// Nach der Ausgabe eines Fehlers wird die Fehlerausgabe deaktiviert
//
// ---------------------------------------------------------------------------

void checkGlErrorCritical(const char* op)
{
    if (GlobalDebugEnabled)
    {
        if (checkGlError(op))
        {
            GlobalDebugEnabled = false;
        }
    }
}


// ---------------------------------------------------------------------------
//
// KLASSE        : global
// METHODE       : checkGlError
//
// ---------------------------------------------------------------------------

void checkGlError(const char* op, const char* name)
{
    for (GLint error = glGetError(); error; error = glGetError())
    {
        gdstr.setf(std::ios::hex);
        gdstr << "after " << op << " glError: 0x" << error << " Name=" << name;
        gdstr.setf(std::ios::dec);

        gdlog();
    }
}


//---------------------------------------------------------------------------
//
// Klasse:  CGL_Context
// Methode: CGL_Context
//
//
//---------------------------------------------------------------------------

CGL_Context::CGL_Context()
{
    mWidth = 0;
    mHeight = 0;

    mTransformAkt = &mTransformMap;

    mTransformMap.mScaleFaktor = 0.5f;
    mTransformMap.mTranslate.Set(0.0f, 0.0f);
    mTransformMap.mType = 1;

    mTransformThumb.mScaleFaktor = 0.25f;
    mTransformThumb.mTranslate.Set(0.0f, 0.0f);
    mTransformThumb.mType = 0;
    mFavTextureHandle = 0;
}


static const char* StaticUniformLocation[] =
{
    "uShaderMode",
    "uColor",
    "uUseTexture"
    "uUseShadow",
    "uUseColor",
    "uProjectionMatrix",
    NULL
};


//---------------------------------------------------------------------------
//
// Klasse:  CGL_Context
// Methode: InitShaderModes
//
//
//---------------------------------------------------------------------------



void CGL_Context::InitShaderModes()
{
    gdstr << "CGL_Context::InitShaderModes START";
    gdlog();

    mShaderModes.clear();

    for (int i = 0; StaticUniformLocation[i] != NULL; i++)
    {
        int uniloc = glGetUniformLocation(mProgram, StaticUniformLocation[i]);


        gdstr << "  " << StaticUniformLocation[i] << " = " << uniloc;
        gdlog();

        mShaderModes.push_back(uniloc);
    }

    gdstr << "CGL_Context::InitShaderModes OK";
    gdlog();
}


//---------------------------------------------------------------------------
//
// Klasse:  CGL_Context
// Methode: GetUniformLocation
//
//
//---------------------------------------------------------------------------

GLint CGL_Context::GetUniformLocation(const char* UniName) const
{
    return glGetUniformLocation(mProgram, UniName);
}


//---------------------------------------------------------------------------
//
// Klasse:  CGL_Context
// Methode: SetUniformVec2
//
//
//---------------------------------------------------------------------------

bool CGL_Context::SetUniformVec2(const char* UniName, const float* v) const
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
// Klasse:  CGL_Context
// Methode: SetUniformVec3
//
//
//---------------------------------------------------------------------------

bool CGL_Context::SetUniformVec3(const char* UniName, const float* v) const
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
// Klasse:  CGL_Context
// Methode: SetUniformVec4
//
//
//---------------------------------------------------------------------------

bool CGL_Context::SetUniformVec4(const char* UniName, const float* v) const
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
// Klasse:  CGL_Context
// Methode: glConRotate
//
//
//---------------------------------------------------------------------------

void CGL_Context::glConRotate(float z)
{
    mMatrixStack.RotateZ(z);
}


//---------------------------------------------------------------------------
//
// Klasse:  CGL_Context
// Methode: glConTranslate
//
//
//---------------------------------------------------------------------------

void CGL_Context::glConTranslate(float x, float y, float z)
{
    mMatrixStack.MatTranslate(x, y, 0.0);
}


//---------------------------------------------------------------------------
//
// Klasse:  CGL_Context
// Methode: glConScale
//
//
//---------------------------------------------------------------------------

void CGL_Context::glConScale(float fx, float fy, float fz)
{
    mMatrixStack.MatScale(fx, fy, fz);
}


//---------------------------------------------------------------------------
//
// Klasse:  CGL_Context
// Methode: glConPushMatrix
//
//
//---------------------------------------------------------------------------

void CGL_Context::glConPushMatrix()
{
    mMatrixStack.MatPushMatrix();
}


//---------------------------------------------------------------------------
//
// Klasse:  CGL_Context
// Methode: glConTransform
//
//
//---------------------------------------------------------------------------

void CGL_Context::glConTransform(CVector2<float>* vec)
{
    mMatrixStack.Transform(vec);
}


//---------------------------------------------------------------------------
//
// Klasse:  CGL_Context
// Methode: glConLoadIdentity
//
//
//---------------------------------------------------------------------------

void CGL_Context::glConLoadIdentity()
{
    mMatrixStack.MatLoadIdentity();
}


//---------------------------------------------------------------------------
//
// Klasse:  CGL_Context
// Methode: iglPopMatrix
//
//
//---------------------------------------------------------------------------

void CGL_Context::glConPopMatrix()
{
    mMatrixStack.MatPopMatrix();
}


//---------------------------------------------------------------------------
//
// Klasse:  CGL_Context
// Methode: SetUniformInt
//
//
//---------------------------------------------------------------------------

bool CGL_Context::SetUniformInt(const char* UniName, int i) const
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
// Klasse:  CGL_Context
// Methode: SetUniformFloat
//
//
//---------------------------------------------------------------------------

bool CGL_Context::SetUniformFloat(const char* UniName, float f) const
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


void CGL_Context::SetUniformInt(int uloc, int i) const
{
    glUniform1i(mShaderModes[uloc], i);
}


void CGL_Context::SetUniformMatrix(int uloc, const float* mat) const
{
    glUniformMatrix4fv(mShaderModes[uloc], 1, GL_FALSE, mat);
}


void CGL_Context::SetUniformVec4(int uloc, const float* v) const
{
    glUniform4fv(mShaderModes[uloc], 1, v);
}


void CGL_Context::SetUniformVec3(int uloc, const float* v) const
{
    glUniform3fv(mShaderModes[uloc], 1, v);
}


void CGL_Context::SetUniformVec2(int uloc, const float* v) const
{
    glUniform2fv(mShaderModes[uloc], 1, v);
}


void CGL_Context::SetUniformFloat(int uloc, const float f) const
{
    glUniform1f(mShaderModes[uloc], f);
}


//---------------------------------------------------------------------------
//
// Klasse:  CGL_Context
// Methode: SetUniformMatrix
//
//
//---------------------------------------------------------------------------

bool CGL_Context::SetUniformMatrix(const char* UniName, const float* mat) const
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
// Klasse:		CGL_Context
// Methode:		glConApplyMatrix
//
//
//---------------------------------------------------------------------------

void CGL_Context::glConApplyMatrix() const
{
    SetUniformMatrix("uProjectionMatrix", mMatrixStack.Mat());
}


#if 0
//---------------------------------------------------------------------------
//
// Klasse:		CGL_Context
// Methode:		ReadTgaTexture
//
//
//---------------------------------------------------------------------------

bool CGL_Context::ReadTgaTexture(CGL_Texture* Texture, const char* TgaFile)
{
    bool r = false;
    CGraflibTga tga;

    if (tga.Read(TgaFile))
    {
        tga.SwapRedBlue();
        tga.Yflip();
        Texture->CreateTexture(&tga);
        tga.DeleteData();
        r = true;
    }
    return r;
}


#endif


//---------------------------------------------------------------------------
//
// Klasse:  CGL_Context
// Methode: UseProgram
//
//
//---------------------------------------------------------------------------

void CGL_Context::UseProgram() const
{
    glUseProgram(mProgram);
    checkGlError("glUseProgram");
}


//---------------------------------------------------------------------------
//
// Klasse:  CGL_Context
// Methode: DeleteProgram
//
//
//---------------------------------------------------------------------------

void CGL_Context::DeleteProgram()
{
    glDeleteProgram(mProgram);
    checkGlError("glDeleteProgram");
}


inline int Imax(int a, int b)
{
    return (a > b) ? a : b;
}


//---------------------------------------------------------------------------
//
// Klasse:  CGL_Context
// Methode: Init
//
//
//---------------------------------------------------------------------------

bool CGL_Context::Init(int w, int h, const std::vector<std::string>* cmdlineparams)
{
    gdstr << "CGL_Context::Init w=" << w << " h=" << h;
    gdlog();

    bool r = false;

    mWidth = w;
    mHeight = h;

    mAspect = float(w) / mHeight;

    mOrientation = (mAspect > 1.0) ? EORIENT_HORIZONTAL : EORIENT_VERTICAL;

    if (SetupGraphics())
    {
        glViewport(0, 0, w, h);
        checkGlError("glViewport");
        r = true;
    }
    else
    {
        exit(0);
    }

    //	InitShaderModes();


    return r;
}


//---------------------------------------------------------------------------
//
// Klasse:  CGL_Context
// Methode: DpiScale
//
//
//---------------------------------------------------------------------------

float CGL_Context::DpiScale() const
{
    // ca. 0.5 fuer Retina
    // ca. 1.0 fuer normale Aufloesung

    return 1000.0 / Tmax(mWidth, mHeight);
}


//---------------------------------------------------------------------------
//
// Klasse:  CGL_Context
// Methode: SetViewport
//
//
//---------------------------------------------------------------------------

void CGL_Context::SetViewport(int w, int h)
{
    mWidth = w;
    mHeight = h;

    mAspect = float(w) / mHeight;
    mOrientation = (mAspect > 1.0) ? EORIENT_HORIZONTAL : EORIENT_VERTICAL;
    glViewport(0, 0, w, h);
}


//---------------------------------------------------------------------------
//
// Klasse:  CGL_Context
// Methode: LoadShaderFile
//
//
//---------------------------------------------------------------------------

static const char* LoadShaderFile(const char* ShaderFilename)
{
    CFileIO fio;
    int FileSize;

    return (const char*)fio.ReadFile(ShaderFilename, &FileSize);
}


//

//---------------------------------------------------------------------------
//
// Klasse:  CGL_Context
// Methode: LoadShader
//
//
//---------------------------------------------------------------------------

GLuint CGL_Context::LoadShader(GLenum shaderType, const char* ShaderSource)
{
    gdstr << "CGL_Context::LoadShader START type=" << shaderType << " size=" << strlen(ShaderSource);
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
                char* buf = (char*)malloc(infoLen);
                if (buf)
                {
                    glGetShaderInfoLog(shader, infoLen, NULL, buf);
                    gdstr << "CGL_Context::LoadShader: " << ShaderSource;
                    gderr();

                    gdstr << "Could not compile shader: " << shaderType << " " << buf;
                    gderr();
                    free(buf);
                }
                glDeleteShader(shader);
                shader = 0;
            }
        }
    }
    checkGlError("LoadShader", ShaderSource);

    gdstr << "CGL_Context::LoadShader OK";
    gdlog();


    return shader;
}


//---------------------------------------------------------------------------
//
// Klasse:	CGL_Context
// Methode: CreateProgram
//
//
//---------------------------------------------------------------------------

GLuint CGL_Context::CreateProgram(const char* pVertexSource, const char* pFragmentSource)
{
    gdstr << "CGL_Context::CreateProgram START";
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
                char* buf = (char*)malloc(bufLength);
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

    gdstr << "CGL_Context::CreateProgram OK";
    gdlog();

    return program;
}


//---------------------------------------------------------------------------
//
// Klasse:  CGL_Context
// Methode: SetupGraphics
//
//
//---------------------------------------------------------------------------


bool CGL_Context::SetupGraphics()
{
    bool r = true;

    gdstr << "===== SetupGraphics START";
    gdlog();

    printGLString("Version", GL_VERSION);
    printGLString("Vendor", GL_VENDOR);
    printGLString("Renderer", GL_RENDERER);
    //printGLString("Extensions", GL_EXTENSIONS);

#if 0
//	const char* StaticVertexShader   = shader_vert;
//	const char* StaticFragmentShader = shader_frag;


//	gdstr << "setupGraphics(" << w << "," << h << ")";
    gdlog();
//
    if (StaticVertexShader == NULL)
    {
        gdstr << "StaticVertexShader = NULL";
        gderr();
        r = false;
    }



    if (StaticFragmentShader == NULL)
    {
        gdstr << "StaticFragmentShader = NULL";
        gderr();
        r = false;
    }

    if (r)
    {
        mProgram = CreateProgram(StaticVertexShader, StaticFragmentShader);
        if (!mProgram)
        {
            gdstr << "Could not create program";
            gderr();
            r = false;
        }

        if (r)
        {
            mVertAttrib = glGetAttribLocation(mProgram, "aVertAttrib");
            checkGlError("glGetAttribLocation VertAttrib");
            mTextAttrib = glGetAttribLocation(mProgram, "aTextAttrib");
            checkGlError("glGetAttribLocation TextAttrib");
            mColorAttrib = glGetAttribLocation(mProgram, "aColorAttrib");
            checkGlError("glGetAttribLocation ColorAttrib");

            //"glGetAttribLocation(\"aPosition\") = %d\n",	mVertAttrib);
        }
    }
#endif
    gdstr << "===== SetupGraphics END r=" << r;
    gdlog();


    return r;
}


//---------------------------------------------------------------------------
//
// Klasse:  CGL_Context
// Methode: TransformModelToScreen
//
//
//---------------------------------------------------------------------------


void CGL_Context::TransformModelToScreen(float mx, float my, float* sx, float* sy) const
{
    float xo = mTransformMap.mTranslate.x;
    float yo = mTransformMap.mTranslate.y;
    float s = mTransformMap.mScaleFaktor;


    *sx = (mx  + xo) * s;
    *sy = (my  + yo) * s;
}


//---------------------------------------------------------------------------
//
// Klasse:  CGL_Context
// Methode: TransformScreenToModel
//
//
//---------------------------------------------------------------------------


void CGL_Context::TransformScreenToModel(float sx, float sy, float* mx, float* my) const
{
    float xo = mTransformMap.mTranslate.x;
    float yo = mTransformMap.mTranslate.y;
    float s = mTransformMap.mScaleFaktor;

    *mx = (sx - xo) / s;
    *my = (sy - yo) / s;
}
