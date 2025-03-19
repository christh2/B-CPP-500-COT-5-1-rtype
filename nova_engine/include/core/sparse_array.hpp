#pragma once
#include <iostream>
#include <vector>
#include <memory>
#include <optional>

namespace NovaEngine {
    /**
     * @brief A Sparse array class to store sparse components efficiently.
     * 
     * This class is designed to store components that are assigned to a large set of entities
     * in an Entity-Component-System (ECS) architecture. It uses one vectors to
     * store components for every entities that have them, optimizing time usage
     * for sparse components.
     * 
     * @tparam T The type of component to be stored in the Sparse array.
     */
    template<typename Component>
    class SparseArray {
    public:
        using value_type = std::optional<Component>;
        using reference_type = value_type &;
        using const_reference_type = const value_type &;
        using container_t = std::vector<value_type>;
        using size_type = typename container_t::size_type;
        using iterator = typename container_t::iterator;
        using const_iterator = typename container_t::const_iterator;
        using allocator = typename std::allocator<Component>;
        using allocator_traits = typename std::allocator_traits<allocator>;

        SparseArray() = default;
        SparseArray(SparseArray const &vlue) {
            _data = vlue._data;
        }
        SparseArray(SparseArray const &&vlue) noexcept {
            _data = vlue._data;
        }

        ~SparseArray() = default;

        SparseArray &operator=(SparseArray const &vlue) {
            _data = vlue._data;
            return *this;
        }
        SparseArray &operator=(SparseArray &&vlue) noexcept {
            _data = vlue._data;
            return *this;
        }
        reference_type operator[](std::size_t index) {
            if (index >= _data.size()) {
                _data.resize(index + 1);
            }
            return _data[index];
        }
        value_type at(std::size_t index) {
            if (index >= _data.size()) {
                _data.resize(index + 1);
            }
            return _data.at(index);
        }
        const_reference_type operator[](std::size_t index) const {
            if (index >= _data.size()) {
                _data.resize(index + 1);
            }
            return _data[index];
        }

        iterator begin() {
            return _data.begin();
        }
        const_iterator begin() const {
            return _data.begin();
        }
        const_iterator cbegin() const {
            return _data.begin();
        }

        iterator end() {
            return _data.end();
        }
        const_iterator end() const {
            return _data.end();
        }
        const_iterator cend() const {
            return _data.end();
        }

        size_type size() const {
            return _data.size();
        }

        reference_type insert_at(size_type pos, Component const &component) {
           if (pos >= _data.size()) {
               _data.resize(pos+1);
           }
           _data[pos] = component;
           return _data[pos];
        }
        reference_type insert_at(size_type pos, Component &&component) {
            if (pos >= _data.size()) {
                _data.resize(pos+1);
            }
            _data[pos] = std::move(component);
            return _data[pos];
        }

        void erase(size_type pos) {
            if (pos > _data.size())
                return;
            if (_data[pos].has_value()) {
                _data[pos].reset();
            }
        }
        size_type get_index(value_type const &vlue) const {
            auto it = std::find(_data.begin(), _data.end(), &vlue);
            if (it != _data.end()) {
                return (it - _data.begin());
            }
            return 0;
        }

    protected:
        container_t _data;
    };
}

template<typename Component>
std::ostream& operator<<(std::ostream &os, NovaEngine::SparseArray<Component> &array) {
    for (int i = 0; i < array.size(); i++) {
        if (array[i].has_value())
            std::cout << "[index " << i << "] value: " << array[i].value() << std::endl;
        else
            std::cout << "[index " << i << "] value: Empty" << std::endl;
    }
    return os;
}
