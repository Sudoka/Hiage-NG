#ifndef _MAPLOADER_H_
#define _MAPLOADER_H_

#include <memory>
#include <string>
#include "ResourceLoader.h"
#include "MapDescriptor.h"

namespace Hiage
{
    /// Class for loading textures into the resource manager
    class MapLoader : public ResourceLoader<MapDescriptor>
    {
        public:
            MapDescriptor LoadResource(std::string filename, std::string name) const;
    };
}
#endif
