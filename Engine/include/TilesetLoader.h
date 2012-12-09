#ifndef _TILESETLOADER_H_
#define _TILESETLOADER_H_

#include <memory>
#include <string>
#include "ResourceLoader.h"
#include "Tileset.h"

namespace Hiage
{
    /// Class for loading textures into the resource manager
    class TilesetLoader : public ResourceLoader<Tileset>
    {
        public:
            Tileset LoadResource(std::string filename, std::string name) const;
    };
}
#endif
