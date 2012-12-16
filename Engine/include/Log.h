#ifndef LOG_H
#define LOG_H

#include <memory>
#include <string>
#include <sstream>

namespace Hiage
{
    enum LogSeverity
    {
        Trace = 1,
        Debug = 2,
        Info = 3,
        Warning = 4,
        Error = 5
    };

    class Log
    {
        public:
            Log();
            virtual ~Log();

            void Write(LogSeverity severity, std::string module, std::string message);
            void Write(std::string module, std::string message);

            Log& operator<<(std::string message);
            Log& operator<<(int n);
            Log& operator<<(long int n);
            Log& operator<<(unsigned int n);
            Log& operator<<(long unsigned int n);
            Log& operator<<(double n);
            Log& operator<<(std::ostream& (*m) (std::ostream&));
            Log& operator<<(LogSeverity severity);
            
            void setSeverityLimit(LogSeverity severity);
        protected:
            virtual void WriteToLog(std::string message) = 0;

        private:
            std::stringstream tokenStream;
            std::string currentModule;
            LogSeverity currentSeverity = Info;
            LogSeverity severityLimit = Trace;
            int ntokens = 0;
    };

    class ConsoleLog : public Log
    {
        public:
            ConsoleLog();
            virtual ~ConsoleLog();


        protected:
            virtual void WriteToLog(std::string message);
        private:
    };
}
#endif // LOG_H
