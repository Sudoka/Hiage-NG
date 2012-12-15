#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include <map>
#include <string>
#include <unordered_map>
#include "Resource.h"

#include "Texture.h"
#include "SpriteDescriptor.h"
#include "Tileset.h"
#include "Sound.h"
#include "Music.h"
#include "MapDescriptor.h"
#include "ObjectDescriptor.h"

namespace Hiage
{
    class Log;
    struct ResourceManager_impl;

    //aliases
    template <class T>
    using ResourceDictionary = std::unordered_map<std::string, Resource<T>>;

    class ResourceManager
    {
        public:
            ResourceManager(Log& log);
            virtual ~ResourceManager();

            void LoadResources(std::string fromDirectory);

            const Texture& GetTexture(std::string name);
            const SpriteDescriptor& GetSpriteDescriptor(std::string name);
//            const ISE.FTFont& GetFont(std::string name);
            const Tileset& GetTileset(std::string name);
            const MapDescriptor& GetMapDescriptor(std::string name);
            const ObjectDescriptor& GetObjectDescriptor(std::string name);
            const Sound& GetAudioClip(std::string name);
            const Music& GetMusic(std::string name);

        protected:
        private:
            std::unique_ptr<ResourceManager_impl> impl;
            Log& log;

    };
}
#endif // RESOURCEMANAGER_H
