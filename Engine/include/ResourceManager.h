#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include <map>
#include <string>
#include <libxml/tree.h>
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

    //aliases
    template <class T>
    using ResourceDictionary = std::map<std::string, Resource<T>>;

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
            void LoadResourceFiles(std::vector<boost::filesystem::path> files);
            void LoadResourceFile(boost::filesystem::path path);
            void LoadResourcesXml(xmlDocPtr resourceXML, const boost::filesystem::path& path);
            void LoadObjectXml(xmlDocPtr resourceXML, const boost::filesystem::path& path);
        private:
            Log& log;

            ResourceDictionary<Texture>	         textures;
            ResourceDictionary<SpriteDescriptor> sprites;
            //std::map<std::string, Resource<ISE.FTFont> >  		fonts;
            ResourceDictionary<Tileset>          tilesets;
            ResourceDictionary<Sound>            sounds;
            ResourceDictionary<Music>            music;
            ResourceDictionary<MapDescriptor>    tilemaps;
            ResourceDictionary<ObjectDescriptor> objects;
    };
}
#endif // RESOURCEMANAGER_H
