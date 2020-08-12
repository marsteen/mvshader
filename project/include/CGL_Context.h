#ifndef CGL_Context_H
#define CGL_Context_H

#include <string>
#include <GLinclude.h>
#include <CMatrixStack.h>
#include <CGL_Texture.h>
#include <CVector2T.h>
#include <EShaderMode.h>
#include <EUniformLocation.h>


struct STransform
{
    STransform()
    {
        mScaleFaktorSet = false;
    }


    CVector2<float> mTranslate;
    float           mScaleFaktor;
    bool            mScaleFaktorSet;    // Skalierungsfaktor wurde gesetzt nach Laden der Karte
    int             mType;              // 0 = Thumbnails, 1 = Maps
};

enum EDrawMode
{
    EDRAWMODE_THUMBNAILS,   // 0 = Thumbnail-Modus
    EDRAWMODE_MAPVIEW,      // 1 = Karten-Modus
    EDRAWMODE_SPLASH        // 2 = Splashscreen anzeigen
};



enum EPadOrientation
{
    EORIENT_HORIZONTAL,
    EORIENT_VERTICAL
};



class CGL_Context
{
    public:

        CGL_Context();
        virtual ~CGL_Context() { }

        virtual bool Init(int w, int h, const std::vector<std::string>* cmdlineparams=nullptr);

        virtual void Draw2D() { }
        virtual void MouseMove(int x, int y, bool lb, bool rb) { }
        virtual void MouseMoveAbs(int x, int y, bool lb, bool rb) { }
        virtual void MouseWheel(bool up) { }
        virtual void Finish() { }
        virtual void KeyPress(int key, bool down) { }
        virtual void Timer() { }

        static CGL_Context* CreateContext();


        GLuint VertAttrib() const { return mVertAttrib; }
        GLuint TextAttrib() const { return mTextAttrib; }
        GLuint ColorAttrib() const { return mColorAttrib; }

        static void SetVertexShaderSource(const char* VertexShader);
        static void SetFragmentShaderSource(const char* FragmentShader);



        void UseProgram() const;
        void DeleteProgram();
        GLint GetUniformLocation(const char* UniName) const;
        bool SetUniformInt(const char* Name, int i) const;
        bool SetUniformFloat(const char* UniName, float f) const;
        bool SetUniformMatrix(const char* UniName, const float* mat) const;
        bool SetUniformVec2(const char* UniName, const float* v) const;
        bool SetUniformVec3(const char* UniName, const float* v) const;
        bool SetUniformVec4(const char* UniName, const float* v) const;
        bool ReadTgaTexture(CGL_Texture* Texture, const char* TgaFile);
        void SetViewport(int w, int h);

        void SetUniformInt(int uloc, int i) const;
        void SetUniformMatrix(int uloc, const float* mat) const;
        void SetUniformFloat(int UniName, const float f) const;
        void SetUniformVec2(int UniName, const float* v) const;
        void SetUniformVec3(int UniName, const float* v) const;
        void SetUniformVec4(int UniName, const float* v) const;



        float ScaleFaktor() const { return mTransformAkt->mScaleFaktor; }// * mDpiScaleFaktor; }
        float TranslateX() const { return mTransformAkt->mTranslate.x; }
        float TranslateY() const { return mTransformAkt->mTranslate.y; }

        void glConLoadIdentity();
        void glConTranslate(float x, float y, float z);
        void glConScale(float fx, float fy, float fz);
        void glConPopMatrix();
        void glConPushMatrix();
        void glConApplyMatrix() const;
        void glConTransform(CVector2<float>* vec);
        void glConRotate(float z);


        int WindowWidth() const { return mWidth; }
        int WindowHeight() const { return mHeight; }
        float DpiScale() const;

        void TransformModelToScreen(float mx, float my, float* sx, float* sy) const;
        void TransformScreenToModel(float sx, float sy, float* mx, float* my) const;
        void InitShaderModes();

        //protected:
        //void glConTransformPos(float ax, float ay, float* tx, float* ty);

        int mWidth;
        int mHeight;
        float mAspect;
        GLuint mVertAttrib;
        GLuint mTextAttrib;
        GLuint mColorAttrib;
        bool mTouchHold;
        bool mNachlaufFlag;
        int mRightThumbBorder;
        int mFavTextureHandle;



        STransform* mTransformAkt;
        STransform mTransformMap;
        STransform mTransformThumb;
        EPadOrientation mOrientation;
        EDrawMode mDrawMode;                // Zeichenmodus(Thumbnails oder Maps)
        std::string mSelection;             // ausgewaehlte Sachgruppe
        std::string mHauptThema;            // Hauptthema bei ThumbMode = ETHUMBMODE_THEMA
        std::vector<int> mShaderModes;



        bool SetupGraphics();
        GLuint CreateProgram(const char* pVertexSource, const char* pFragmentSource);
        GLuint LoadShader(GLenum shaderType, const char* pSource);

        GLuint mProgram;
        CMatrixStack mMatrixStack;
};

#endif
