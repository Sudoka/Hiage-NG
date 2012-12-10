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
    return 0;
}
