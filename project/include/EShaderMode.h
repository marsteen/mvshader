//***************************************************************************
//
//
// @PROJECT  :	Basic SDL
// @VERSION  :	1.0
// @FILENAME :	EShaderMode.h
// @DATE     :	8.1.2015
//
// @AUTHOR   :	Martin Steen
// @EMAIL    :	martin@martin-steen.de
//
//
//***************************************************************************


//
//
//


#ifndef ESHADER_MODE_H
#define ESHADER_MODE_H

enum {
    /* 00 */
    ESHADER_MODE_MAPS,                  // Nur Textur
    /* 01 */ ESHADER_MODE_SHADOW,       // Schatten
    /* 02 */ ESHADER_MODE_TEXT,         // Freetype-Text
    /* 03 */ ESHADER_MODE_ANTS,         // gestrichelte Linie ("Ameisenkolonne")
    /* 04 */ ESHADER_MODE_COLOR,        // Farbe
    /* 05 */ ESHADER_MODE_COLOR_OPA,    // Farbe opaque
    /* 06 */ ESHADER_MODE_FB_TEXT,      // Framebuffer Freetype Text
    /* 07 */ ESHADER_MODE_DARK_TEXTURE  // Abgedunkelte Textur
};

#endif
