#pragma once
#include <iostream>

namespace NovaEngine {
        /**
         * @brief A mass component that defines the mass of the entity.
         *
         * The mass component is used to determine the total weight of the entity.
        */

    typedef struct mass_t {
        double mass;
        mass_t(double mass = 0) : mass(mass) {}
        mass_t(const mass_t& other) : mass(other.mass) {}
        operator double() const {
            return mass;
        }
        mass_t operator+(const double other) const {
            return mass_t(mass + other);
        }
        mass_t operator-(const double& other) const {
            return mass_t(mass - other);
        }
        mass_t &operator+=(const double& other) {
            mass += other;
            return *this;
        }
        mass_t &operator-=(const double& other) {
            mass -= other;
            return *this;
        }
        mass_t &operator=(const mass_t& other) {
            mass = other.mass;
            return *this;
        }
    } mass_t;
}
