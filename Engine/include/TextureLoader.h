#ifndef _TEXTURELOADER_H_
#define _TEXTURELOADER_H_

#include <memory>
#include <string>
#include "ResourceLoader.h"
#include "Texture.h"

namespace Hiage
{
    /// Class for loading textures into the resource manager
    class TextureLoader : public ResourceLoader<Texture>
    {
        public:
            Texture LoadResource(std::string filename, std::string name) const;
    };
}
#endif
