#ifndef __EXCEPTIONS_H_
#define __EXCEPTIONS_H_

#include <stdexcept>

namespace Hiage
{
    class FileNotFoundException : public std::runtime_error
    {
        public:
            FileNotFoundException(const std::string& message) : std::runtime_error(message)
            {
            }
    };

    class IndexOutOfRangeException : public std::runtime_error
    {
        public:
            IndexOutOfRangeException(const std::string& message) : std::runtime_error(message)
            {
            }
    };

    class KeyNotFoundException : public std::runtime_error
    {
        public:
            KeyNotFoundException(const std::string& message) : std::runtime_error(message)
            {
            }
    };
}
#endif
