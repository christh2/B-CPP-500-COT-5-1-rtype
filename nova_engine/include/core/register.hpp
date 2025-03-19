#pragma once
#include <iostream>
#include <unordered_map>
#include <any>
#include <typeindex>
#include <functional>
#include <variant>
#include <SFML/Graphics.hpp>
#include "entity.hpp"
#include "hybrid_array.hpp"
#include "asset_handler.hpp"
#include "message.hpp"

namespace NovaEngine {
    /**
     * @brief A class that manages entities and components in a hybrid array system.
     * 
     * The Registry provides methods for creating and destroying entities, registering and 
     * managing components, and executing systems. It supports various systems like standard, 
     * event-based, and rendering systems, which can operate on entities and their components.
     */
    class Registry {
    public:
        using entity_id = std::size_t;

        Registry() = default;
        ~Registry() = default;
        template<class Component>
        void init_sparse_array(std::type_index type) {
            hybrid_array<Component> vlue = SparseArray<Component>();
            _components_array[type] = vlue;
            _eraser_array[type] = [this](Registry& reg, entity_id id) {
                hybrid_array<Component> &array = reg.get_registry<Component>();
                std::optional<Component> &comp = array[id];
                if (comp.has_value()) {
                    comp.reset();
                }
            };
        }

        template<class Component>
        void init_compact_array(std::type_index type) {
            hybrid_array<Component> storage = CompactArray<Component>();
            _components_array[type] = storage;
            _eraser_array[type] = [this](Registry& reg, entity_id id) {
                hybrid_array<Component> &array = reg.get_registry<Component>();
                array.erase(id);
            };
        }

        template<class Component>
        hybrid_array<Component> &register_component(bool is_common = true) {
            std::type_index type = std::type_index(typeid(Component));
            if (is_common) {
                init_sparse_array<Component>(type);
            } else {
                init_compact_array<Component>(type);
            }
            return std::any_cast<hybrid_array<Component>&>(_components_array[type]);
        }

        template<class Component>
        std::optional<Component> &get_component(entity_id index) {
            return std::any_cast<hybrid_array<Component>&>(_components_array[std::type_index(typeid(Component))])[index];
        }

        template<class Component>
        std::optional<Component> const get_component(entity_id index) const {
            return get_registry<Component>()[index];
        }

        template<class Component>
        hybrid_array<Component> &get_registry() {
            return std::any_cast<hybrid_array<Component>&>(_components_array[std::type_index(typeid(Component))]);
        }

        template<class Component>
        hybrid_array<Component> const &get_registry() const {
            return std::any_cast<hybrid_array<Component>&>(_components_array.at(std::type_index(typeid(Component))));
        }

        Entity spawn_entity() {
            entity_id _id;
            if (!_dead_entities.empty()) {
                _id = _dead_entities.back();
                _dead_entities.pop_back();
            } else {
                _id = _entities.size();
            }
            Entity new_entity = Entity(_id);
            _entities.push_back(new_entity);
            return _entities.back();
        }

        void add_entity(entity_id id) {
            _entities.push_back(Entity(id));
        }

        Entity entity_from_index(entity_id index) {
            return _entities[index];
        }

        void kill_entity(entity_id size) {
            if (_entities.empty()) {
                return;
            }
            for (auto& it : _eraser_array) {
               it.second(*this, size);
            }
            auto it = std::find(_entities.begin(), _entities.end(), size);
            _dead_entities.push_back(*it);
            if (it != _entities.end())
                _entities.erase(it);
        }

        template<typename Component>
        void add_component(
                entity_id entity, Component component)
        {
            hybrid_array<Component> &an_array = std::any_cast<hybrid_array<Component>&>(
                _components_array[std::type_index(typeid(Component))]);
            an_array.insert_at(entity, component);
        }

        template<typename Component>
        void remove_component(entity_id id)
        {
            try {
                hybrid_array<Component> &an_array = std::any_cast<hybrid_array<Component>>(
                        _components_array[std::type_index(typeid(Component))]);
                an_array.erase_at(id);
            } catch (std::bad_any_cast& e) {
                std::cout << e.what() << std::endl;
            }
        }

        template<typename Component>
        std::size_t array_size() {
            return std::any_cast<hybrid_array<Component>&>(_components_array[std::type_index(typeid(Component))]).size();
        }

        std::vector<Entity> &get_entities() {
            return _entities;
        }

        template <class... component, typename Function>
        void add_standard_system(Function &&function) {
            _standard_system.push_back([&function](Registry &registry, message_t &message) {
                function(registry.get_entities(), message, &registry.get_registry<component>()...);
            });
        }

        template <class... component, typename Function>
        void add_standard_system(Function const &function) {
            _standard_system.push_back([&function](Registry &registry, message_t &message) {
               function(registry.get_entities(), message, registry.get_registry<component>()...);
            });
        }

        template <class... component, typename Function>
        void add_event_system(Function &&function) {
            _event_system.push_back([&function](Registry &registry, message_t &message, sf::RenderWindow& win, sf::Event& event) {
                function(registry.get_entities(), message, win, event, &registry.get_registry<component>()...);
            });
        }

        template <class... component, typename Function>
        void add_event_system(Function const &function) {
            _event_system.push_back([&function](Registry &registry, message_t &message, sf::RenderWindow& win, sf::Event& event) {
               function(registry.get_entities(), message, win, event, registry.get_registry<component>()...);
            });
        }

        template <class... component, typename Function>
        void add_rendering_system(Function &&function) {
            _rendering_system.push_back([&function](Registry &registry, message_t &message, sf::RenderWindow &window, Asset_handler &handler) {
                function(registry.get_entities(), message, window, handler, registry.get_registry<component>()...);
            });
        }

        template <class... component, typename Function>
        void add_rendering_system(Function const &function) {
            _rendering_system.push_back([&function](Registry &registry, message_t &message, sf::RenderWindow &window, Asset_handler &handler) {
               function(registry.get_entities(), message, window, handler, registry.get_registry<component>()...);
            });
        }

        template <class... component, typename Function>
        void add_ui_system(Function const &function) {
            ui_system = ([&function](Registry &registry, sf::RenderWindow &window, sf::Event &event) -> std::string {
               return function(registry.get_entities(), window, event, registry.get_registry<component>()...);
            });
        }

        void run_event_system(sf::Event &event, sf::RenderWindow &window, Asset_handler &handler) {
            for (const auto &system : _event_system) {
                system(*this, message_queue, window, event);
            }
        }

        void run_standard_system() {

            for (const auto &system : _standard_system) {
                try {
                    system(*this, message_queue);
                } catch (const std::exception &e) {
                    // Gérer les exceptions standard
                    std::cerr << "Exception caught in standard system: " << e.what() << std::endl;
                } catch (...) {
                    // Gérer les exceptions inconnues
                    std::cerr << "Unknown exception caught in standard system." << std::endl;
                }
            }
        }

        std::string run_ui_system(sf::RenderWindow &window, sf::Event &event) {
            if (ui_system.has_value()) {
                return ui_system.value()(*this, window, event);
            }
            return "";
        }

        void run_rendering_system(sf::RenderWindow &window, Asset_handler &handler) {
            for (const auto &system : _rendering_system) {
                system(*this, message_queue, window, handler);
            }
        }

        std::string run_system(sf::Event &event, sf::RenderWindow &window, Asset_handler &handler) {
            std::cout << "----------------" << std::endl;
            std::cout << _entities.size() << std::endl;
            for (const auto &system : _standard_system) {
                system(*this, message_queue);
            }
            for (const auto &system : _rendering_system) {
                system(*this, message_queue, window, handler);
            }
            std::cout << "----------------" << std::endl;
            if (ui_system.has_value()) {
                return ui_system.value()(*this, window, event);
            }
            return "";
        }

        message_t& get_message_queue() {
            return message_queue;
        }

    protected:
        std::unordered_map<std::type_index, std::any> _components_array;
        std::unordered_map<std::type_index, std::function<void(Registry&,entity_id)>> _eraser_array;
        std::vector<std::function<void(Registry&, message_t&)>> _standard_system;
        std::vector<std::function<void(Registry&, message_t&, sf::RenderWindow&, sf::Event&)>> _event_system;
        std::vector<std::function<void(Registry&, message_t&, sf::RenderWindow&, Asset_handler&)>> _rendering_system;
        std::optional<std::function<std::string(Registry&, sf::RenderWindow&, sf::Event&)>> ui_system;
        std::vector<Entity> _entities;
        std::vector<entity_id> _dead_entities;
        message_t message_queue;
    };
}