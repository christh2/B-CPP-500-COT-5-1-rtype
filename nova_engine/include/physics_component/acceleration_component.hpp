#pragma once
#include <iostream>
#include <chrono>

namespace NovaEngine {
        /**
         * @brief An acceleration component that defines the speed rate at which the entity will be going.
         *
         * In the game when an entity is speeding fast or slower this component is the one that regulates its
         * behavior.
        */
    typedef struct acceleration_t {
        double acc_x;
        double acc_y;
        std::chrono::time_point<std::chrono::steady_clock> time;
        acceleration_t(double x, double y) : acc_x(x), acc_y(y) {
            time = std::chrono::steady_clock::now();
        }
        acceleration_t(const acceleration_t& other) : acc_x(other.acc_x), acc_y(other.acc_y) {
            time = std::chrono::steady_clock::now();            
        }
        acceleration_t operator+(const acceleration_t& other) const {
            return {acc_x + other.acc_x, acc_y + other.acc_y};
        }
        acceleration_t operator-(const acceleration_t& other) const {
            return {acc_x - other.acc_x, acc_y - other.acc_y};
        }
        acceleration_t operator*(double scalar) const {
            return {acc_x * scalar, acc_y * scalar};
        }
        acceleration_t operator=(const acceleration_t& other) {
            acc_x = other.acc_x;
            acc_y = other.acc_y;
            time = std::chrono::steady_clock::now();
            return *this;
        }
        void clock_restart() {
            time = std::chrono::steady_clock::now();
        }
        float dt() {
            return std::chrono::duration<float>(std::chrono::steady_clock::now() - time).count();
        }
    } acceleration_t;
}
