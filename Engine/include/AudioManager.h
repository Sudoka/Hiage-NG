#ifndef AUDIOMANAGER_H_INCLUDED
#define AUDIOMANAGER_H_INCLUDED

#include <string>

namespace Hiage
{
    class AudioManager
    {
        public:
            void PlaySound(std::string name);
            void PlayMusic(std::string name);
    };
}
#endif // AUDIOMANAGER_H_INCLUDED
