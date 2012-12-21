#include <string>
#include "MapLoader.h"

using namespace std;
using namespace Hiage;

MapDescriptor MapLoader::LoadResource(string filename, string name) const
{
    return MapDescriptor(name, 128, 128, 8, 32, 0, 0, "grassland");
}
