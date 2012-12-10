#ifndef LOG_H
#define LOG_H

#include <string>
#include <map>

namespace Hiage
{
    enum LogSeverity
    {
        Debug,
        Info,
        Warning,
        Error
    };

    class Log
    {
        public:
            Log();
            virtual ~Log();

            virtual void Write(LogSeverity severity, std::string module, std::string message) = 0;
            void Write(std::string module, std::string message);
        protected:

        private:
            static std::map<std::string, Log> logs;
    };

    class ConsoleLog : public Log
    {
        public:
            ConsoleLog();
            virtual ~ConsoleLog();

            virtual void Write(LogSeverity severity, std::string module, std::string message);

        protected:
        private:
    };
}
#endif // LOG_H
