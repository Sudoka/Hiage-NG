
#ifndef _OBJECTLOADER_H_
#define _OBJECTLOADER_H_

#include <memory>
#include <string>
#include "ResourceLoader.h"
#include "ObjectDescriptor.h"

namespace Hiage
{
    /// Class for loading textures into the resource manager
    class ObjectLoader : public ResourceLoader<ObjectDescriptor>
    {
        public:
            ObjectDescriptor LoadResource(std::string filename, std::string name) const;
    };
}
#endif
