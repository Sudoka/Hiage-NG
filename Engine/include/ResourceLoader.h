#ifndef _RESOURCE_LOADER_H_
#define _RESOURCE_LOADER_H_

#include <memory.h>

namespace Hiage
{
    template<class T> class ResourceLoader
    {
        public:
        virtual T LoadResource(std::string filename, std::string name) const = 0;
    };
}

#endif

