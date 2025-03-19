#include <SFML/System/Clock.hpp>

namespace NovaEngine {
    typedef struct snackbar_t {
        bool dead;
        bool start = false;
        snackbar_t(float duration_seconds) : duration(duration_seconds) {
            dead = false;
        }
        bool is_expired() const {
            return timer.getElapsedTime().asMilliseconds() >= duration;
        }
        bool soon_expire() const {
            return timer.getElapsedTime().asMilliseconds() >= (duration - 5);
        }
        void restart() {
            timer.restart();
        }
        float duration;
        sf::Clock timer;
    }snackbar_t;
}
