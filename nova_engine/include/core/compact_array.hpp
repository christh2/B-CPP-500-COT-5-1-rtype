#include <iostream>
#include <vector>
#include <memory>
#include <optional>
#pragma once

namespace NovaEngine {
    /**
     * @brief A compact array class to store sparse components efficiently.
     * 
     * This class is designed to store components that are not assigned to every entity
     * in an Entity-Component-System (ECS) architecture. It uses two vectors to
     * store components for only those entities that have them, optimizing memory usage
     * for sparse components.
     * 
     * @tparam T The type of component to be stored in the compact array.
     */
    template<typename Component>
    class CompactArray {
    public:
        using value_type = std::optional<Component>;
        using reference_type = value_type &;
        using const_reference_type = const value_type &;
        using container_t = std::vector<value_type>;
        using entity_id = std::size_t;
        using id_container = std::vector<entity_id>;
        using size_type = typename container_t::size_type;
        using allocator = typename std::allocator<Component>;
        using allocator_traits = typename std::allocator_traits<allocator>;

        CompactArray() = default;
        CompactArray(CompactArray const &vlue) {
            _data = vlue._data;
            _ids = vlue._ids;
        }
        CompactArray(CompactArray const &&vlue) noexcept {
            _data = vlue._data;
            _ids = vlue._ids;
        }

        ~CompactArray() = default;

        CompactArray &operator=(CompactArray const &vlue) {
            _data = vlue._data;
            _ids = vlue._ids;
            return *this;
        }
        CompactArray &operator=(CompactArray &&vlue) noexcept {
            _data = vlue._data;
            _ids = vlue._ids;
            return *this;
        }
        reference_type operator[](entity_id id) {
            int i = 0;
            for (auto v1 : _ids) {
                if (v1 == id) {
                    return _data[i];
                }
                i++;
            }
            return _data[0];
        }
        const_reference_type operator[](entity_id id) const {
            int i = 0;
            for (auto v1 : _ids) {
                if (v1 == id) {
                    return _data[i];
                }
                i++;
            }
            return _data[0];
        }

        value_type at(std::size_t index) {
            int i = 0;
            for (auto v1 : _ids) {
                if (v1 == index) {
                    return _data.at(i);
                }
                i++;
            }
            return _data.at(0);
        }

        size_type size() const {
            return _data.size();
        }

        reference_type insert_at(entity_id id, Component const &component) {
           _data.push_back(component);
           _ids.push_back(id);
           return _data.back();
        }
        reference_type insert_at(entity_id id, Component &&component) {
            _data.push_back(std::move(component));
           _ids.push_back(id);
            return _data.back();
        }

        // template<class ...Params>
        // reference_type emplace_at(entity_id id, Params &&...params) {
        //     auto it = std::find(_ids.begin(), _ids.end(), id);
        //     if (it != _ids.end()) {
        //         allocator allocate;
        //         Component *ptr = nullptr;
        //         allocator_traits::destroy(allocate, ptr);
        //         ptr = std::addressof(it);
        //         allocator_traits::construct(allocate, ptr, std::forward<Params>(params)...);
        //         return *it;
        //     }
        //     _ids.push_back(id);
        //     _data.push_back(std::forward<Params>(params)...);
        //     return _data.back();
        // }
        void erase(entity_id id) {
            auto it = std::find(_ids.begin(), _ids.end(), id);
            if (it != _ids.end()) {
                _ids.erase(it);
            }
        }

    protected:
        container_t _data;
        id_container _ids;
    };
}

template<typename Component>
std::ostream& operator<<(std::ostream &os, NovaEngine::CompactArray<Component> &array) {
    for (int i = 0; i < array.size(); i++) {
        if (array[i].has_value())
            std::cout << "[index " << i << "] value: " << array[i].value() << std::endl;
        else
            std::cout << "[index " << i << "] value: Empty" << std::endl;
    }
    return os;
}
