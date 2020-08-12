#ifndef CGL_TEXTURE_H
#define CGL_TEXTURE_H

#include <CDataRect.h>

class CGL_Texture : public CDataRect
{
    public:

        CGL_Texture()
        {
            mTexHandle = 0;
        }


        bool LoadTextureTga(const char* TextureFilename);
        bool CreateTexture(const CDataRect* glib);
        void DeleteTexture();


        unsigned int mTexHandle;
};


#endif
