//
// Created by Raphaël Dantzer on 16/10/16.
//

#include "FreeType.h"
#include "../utils/FileLogger.h"

Proxy::FreeType::FreeType(std::string font, std::pair<FT_UInt , FT_UInt> size) :
_font(font), _size(size){
    PreInit();
    Init();
    PostInit();
}

void Proxy::FreeType::PreInit() {
    if (FT_Init_FreeType(&_ft))
        FLOG_CRITICAL("Unable to init FreeType context");

}

void Proxy::FreeType::Init() {
    if (FT_New_Face(_ft, _font.c_str(), 0, &_face))
        FLOG_CRITICAL("Unable to open font file");
}

void Proxy::FreeType::PostInit() {
    FT_Set_Pixel_Sizes(_face, _size.first, _size.second);
    _glyphs.reserve(128);
    for (int i = 0; i < 128; i++) {
        if (FT_Load_Char(_face, (FT_ULong)i, FT_LOAD_RENDER))
            FLOG_CRITICAL("Unable to load character");
        _glyphs.push_back(_face->glyph);
    }
}


