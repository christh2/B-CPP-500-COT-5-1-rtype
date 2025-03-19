#include <iostream>
#include <SFML/System.hpp>

namespace NovaEngine {
    typedef struct sound_t {
      public:
        std::string sound_file;
        int volume;
        bool loop;
        bool can_play;
        sound_t(std::string sound_file, int volume, bool can_play = false, bool loop = false) : sound_file(sound_file), volume(volume), loop(loop), can_play(can_play) {};
        ~sound_t() {};
      private:
    } sound_t;
}