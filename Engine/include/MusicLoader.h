#ifndef _MUSICLOADER_H_
#define _MUSICLOADER_H_

#include <memory>
#include <string>
#include "ResourceLoader.h"
#include "Music.h"

namespace Hiage
{
    /// Class for loading textures into the resource manager
    class MusicLoader : public ResourceLoader<Music>
    {
        public:
            virtual Music LoadResource(std::string filename, std::string name) const;
    };
}
#endif

