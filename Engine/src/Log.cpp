#include <iostream>
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

ConsoleLog::ConsoleLog()
{
}

ConsoleLog::~ConsoleLog()
{
}

void ConsoleLog::Write(LogSeverity severity, std::string module, std::string message)
{
    switch (severity)
    {
        case Debug:
            cout << "DEBUG ";
            break;
        case Info:
            cout << "INFO  ";
            break;
        case Warning:
            cout << "WARN  ";
            break;
        case Error:
            cout << "ERROR ";
            break;
    }
    cout << module << ": " << message;
}
