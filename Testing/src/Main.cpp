#include <iostream>
#include <memory>

#include <Game.h>
#include <ResourceManager.h>
#include <Log.h>

using namespace std;
using namespace Hiage;

int main()
{
    ConsoleLog log;
    ResourceManager resources(log);
    resources.LoadResources("data");

    const Texture& t = resources.GetTexture("background-blue-domes");
    return 0;
}
