#pragma once
#include <iostream>

/**
 * @brief Represents a game entity in the Entity-Component-System (ECS) architecture.
 * 
 * An Entity serves as a unique identifier and container for various components.
 * Entities themselves contain no logic or data beyond their ID; they rely on
 * associated components to define behavior and properties.
 */
namespace NovaEngine {
    class Entity {
    public:
        explicit Entity(std::size_t id) : _id(id) {}
        ~Entity() = default;
        operator std::size_t() {
            return _id;
        }
        bool operator==(const Entity& other) const {
            return _id == other._id;
        }
        bool operator==(const std::size_t id) const {
            return _id == id;
        }

    private:
        std::size_t _id;
    };
}
