#ifndef _SPRITELOADER_H_
#define _SPRITELOADER_H_

#include <memory>
#include <string>
#include "ResourceLoader.h"
#include "SpriteDescriptor.h"

namespace Hiage
{
    /// Class for loading textures into the resource manager
    class SpriteLoader : public ResourceLoader<SpriteDescriptor>
    {
        public:
            virtual SpriteDescriptor LoadResource(std::string filename, std::string name) const;
    };
}
#endif
