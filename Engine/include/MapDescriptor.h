#ifndef _MAPDESCRIPTOR_H_
#define _MAPDESCRIPTOR_H_

#include <string>
#include <vector>
#include <unordered_map>

namespace Hiage
{
    /// Contains information on objects placed around the map
    class MapObject
    {
        public:
            MapObject(double x, double y, std::string name) : x(x), y(y), name(name)
        {
        }

        private:
            double x, y;
            std::string name;
            std::unordered_map<std::string, std::string> properties; 
    };

    class MapDescriptor
    {
        public:
            //Create an empty map descriptor
            MapDescriptor();
            MapDescriptor(std::string mapID, int width, int height, int layers, double tileSize, double offsetX, double offsetY, std::string tileset);

            void setTile(int x, int y, int z, int tileID);
            int getTile(int x, int y, int z);

            void LoadFromArray(std::vector<int> tileData, int width, int height, int layers);

        private:
            int width, height, layers;
            double offsetX, offsetY, tileSize;
            std::string mapID, background, tileset;
            std::vector<std::vector<std::vector<int>>> tiles;
            std::unordered_map<std::string, std::string> properties;
            std::vector<MapObject> objects;
    };
};

#endif
