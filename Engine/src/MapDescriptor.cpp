#include <string>
#include "MapDescriptor.h"

using namespace std;
using namespace Hiage;

MapDescriptor::MapDescriptor(string mapID, int width, int height, int layers, double tileSize, double offsetX, double offsetY, string tileset)
    :  width(width), height(height), layers(layers), offsetX(offsetX), offsetY(offsetY), tileSize(tileSize), mapID(mapID), tileset(tileset)
{
}

void MapDescriptor::setTile(int x, int y, int z, int tileID)
{
    tiles[x][y][z] = tileID;
}

int MapDescriptor::getTile(int x, int y, int z)
{
    return tiles[x][y][z];
}

void MapDescriptor::LoadFromArray(std::vector<int> tileData, int width, int height, int layers)
{
    //Convert the byte array to a 3D array of ints
    for (int x = 0; x < width; x++)
    {
        for (int y = 0; y < height; y++)
        {
            for (int z = 0; z < layers; z++)
            {
                for (size_t i = 0; i < sizeof(int); i++)
                {
                    int index = i + z*sizeof(int) + y*sizeof(int)*layers + x*sizeof(int)*layers*height;
                    tiles[x][y][z] |= (tileData[index] << (i*8));
                }
            }
        }
    }
}
