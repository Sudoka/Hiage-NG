#include <string>
#include "Log.h"

using namespace Hiage;
using namespace std;

Log::Log()
{
    //ctor
}

Log::~Log()
{
    //dtor
}

void Log::Write(std::string module, std::string message)
{
    Write(Info, module, message);
}
