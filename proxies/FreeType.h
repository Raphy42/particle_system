//
// Created by Raphaël Dantzer on 16/10/16.
//

#ifndef PARTICLE_SYSTEM_FREETYPE_H
#define PARTICLE_SYSTEM_FREETYPE_H

#include <ft2build.h>
#include FT_FREETYPE_H
#include <vector>
#include <string>
#include "ProxyInterface.h"

namespace Proxy {

    class FreeType : public ProxyInterface {
    public:
        FreeType(std::string font, std::pair<FT_UInt, FT_UInt> size);

        virtual void PreInit() override;
        virtual void Init() override;
        virtual void PostInit() override;

    private:
        FT_Library                      _ft;
        std::string                     _font;
        FT_Face                         _face;
        std::pair<FT_UInt , FT_UInt>    _size;
        std::vector<FT_GlyphSlot>       _glyphs;
    };

}

#endif //PARTICLE_SYSTEM_FREETYPE_H
