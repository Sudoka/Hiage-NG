#include <iostream>
#include <string>
#include "Log.h"

using namespace Hiage;
using namespace std;

Log::Log()
{
}

Log::~Log()
{
}

void Log::Write(LogSeverity severity, std::string module, std::string message)
{
    if (severity < severityLimit)
        return;

    stringstream ss;
    switch (severity)
    {
        case Trace:
            ss << "TRACE ";
            break;
        case Debug:
            ss << "DEBUG ";
            break;
        case Info:
            ss << "INFO  ";
            break;
        case Warning:
            ss << "WARN  ";
            break;
        case Error:
            ss << "ERROR ";
            break;
    }

    ss << module << ": " <<  message << endl;

    WriteToLog(ss.str());
}

void Log::Write(std::string module, std::string message)
{
    Write(Info, module, message);
}

Log& Log::operator<<(std::string message)
{
    ntokens++;
    if (ntokens == 1)
        currentModule = message;
    else 
        tokenStream << message;

    return *this;
}

Log& Log::operator<<(int n)
{
    return operator<<((long int)n);
}

Log& Log::operator<<(long int n)
{
    ntokens++;
    if (ntokens == 1)
        currentModule = "";

    tokenStream << n;

    return *this;
}

Log& Log::operator<<(unsigned int n)
{
    return operator<<((long unsigned int)n);
}

Log& Log::operator<<(long unsigned int n)
{
    ntokens++;
    if (ntokens == 1)
        currentModule = "";

    tokenStream << n;

    return *this;
}


Log& Log::operator<<(double n)
{
    ntokens++;
    if (ntokens == 1)
        currentModule = "";

    tokenStream << n;

    return *this;
}

Log& Log::operator<<(LogSeverity severity)
{
    currentSeverity = severity;

    return *this;
}

Log& Log::operator<<(ostream& (*m)(ostream&))
{
    Write(currentSeverity, currentModule, tokenStream.str());
    tokenStream.str("");
    ntokens = 0;
    currentModule = "";
    currentSeverity = Info;

    return *this;
}

void Log::setSeverityLimit(LogSeverity severity)
{
    severityLimit = severity;
}

ConsoleLog::ConsoleLog()
{
}

ConsoleLog::~ConsoleLog()
{
}

void ConsoleLog::WriteToLog(std::string message)
{
    cout << message << flush;
}
