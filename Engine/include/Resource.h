#ifndef RESOURCE_H
#define RESOURCE_H

#include <string>
#include "ResourceLoader.h"

namespace Hiage
{

    template<class T> class Resource
    {
        public:
            Resource(std::string filename, std::string name)
            {
                this->filename = filename;
                this->name = name;
            }

            virtual ~Resource()
            {
            }

            bool IsLoaded()
            {
                return isLoaded;
            }

            void Load(const ResourceLoader<T>& resourceLoader)
            {
                resource = resourceLoader.LoadResource(filename, name);
                isLoaded = true;
            }

            const T& getResource()
            {
                return resource;
            }
        protected:
        private:
            bool isLoaded;
            std::string filename;
            T resource;
            std::string name;
    };

}
#endif // RESOURCE_H
