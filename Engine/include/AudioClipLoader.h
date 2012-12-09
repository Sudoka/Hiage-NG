#ifndef _AUDIOCLIPLOADER_H_
#define _AUDIOCLIPLOADER_H_

#include <memory>
#include <string>
#include "ResourceLoader.h"
#include "Sound.h"

namespace Hiage
{
    /// Class for loading textures into the resource manager
    class AudioClipLoader : public ResourceLoader<Sound>
    {
        public:
            Sound LoadResource(std::string filename, std::string name) const;
    };
}
#endif
