#include <iostream>
#include <vector>
#include <string>
#include <SFML/System.hpp>

typedef struct playlist_t {
  public:
    playlist_t() {};
    ~playlist_t() {};
    std::vector<std::string> sound_files;
    int current_music;
    int volume;
    bool loop;
  private:
}playlist_t;