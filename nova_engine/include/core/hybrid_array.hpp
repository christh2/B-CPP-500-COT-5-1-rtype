#pragma once
#include <iostream>
#include <vector>
#include <memory>
#include <optional>
#include <variant>
#include "sparse_array.hpp"
#include "compact_array.hpp"

namespace NovaEngine {
        /**
         * @brief A hybrid array that stores components in either a sparse or compact representation.
         * 
         * This array allows efficient storage and retrieval of components, choosing
         * between sparse and compact storage strategies based on usage patterns.
         * It provides methods for insertion, deletion, and access of components.
         */
    template<typename Component>
    class hybrid_array {
    public:
        using hybrid_storage = std::variant<SparseArray<Component>, CompactArray<Component>>;
        using reference_type = std::optional<Component> &;
        using const_reference_type = const reference_type &;
        using entity_id = std::size_t;

        hybrid_array() {
            _storage = SparseArray<Component>();
        }
        hybrid_array(hybrid_array const &vlue) {
            _storage = vlue._storage;
        }
        hybrid_array(hybrid_array const &&vlue) noexcept {
            _storage = vlue._storage;
        }
        hybrid_array(SparseArray<Component> const &vlue) {
            _storage = vlue;
        }
        hybrid_array(SparseArray<Component> const &&vlue) noexcept {
            _storage = vlue;
        }
        hybrid_array(CompactArray<Component> const &vlue) {
            _storage = vlue;
        }
        hybrid_array(CompactArray<Component> const &&vlue) noexcept {
            _storage = vlue;
        }

        ~hybrid_array() = default;

        hybrid_array &operator=(hybrid_array const &vlue) {
            _storage = vlue._storage;
            return *this;
        }
        hybrid_array &operator=(hybrid_array &&vlue) noexcept {
            _storage = vlue._storage;
            return *this;
        }
        hybrid_array &operator=(SparseArray<Component> const &vlue) noexcept {
            this->_storage = vlue;
            return *this;
        }
        hybrid_array &operator=(SparseArray<Component> &&vlue) noexcept {
            this->_storage = vlue;
            return *this;
        }
        hybrid_array &operator=(CompactArray<Component> const &vlue) noexcept {
            _storage = vlue;
            return *this;
        }
        hybrid_array &operator=(CompactArray<Component> &&vlue) noexcept {
            _storage = vlue;
            return *this;
        }
        reference_type operator[](entity_id id) {
            if (std::holds_alternative<SparseArray<Component>>(_storage)) {
                return std::get<SparseArray<Component>>(_storage)[id];
            }
            return std::get<CompactArray<Component>>(_storage)[id];
        }
        const_reference_type operator[](entity_id id) const {
            if (std::holds_alternative<SparseArray<Component>>(_storage)) {
                return std::get<SparseArray<Component>>(_storage)[id];
            }
            return std::get<CompactArray<Component>>(_storage)[id];
        }

        std::size_t size() const {
            if (std::holds_alternative<SparseArray<Component>>(_storage)) {
                return std::get<SparseArray<Component>>(_storage).size();
            }
            return std::get<CompactArray<Component>>(_storage).size();
        }

        reference_type insert_at(entity_id id, Component const &component) {
            if (std::holds_alternative<SparseArray<Component>>(_storage)) {
                return std::get<SparseArray<Component>>(_storage).insert_at(id, component);
            }
            return std::get<CompactArray<Component>>(_storage).insert_at(id, component);
        }
        reference_type insert_at(entity_id id, Component &&component) {
            if (std::holds_alternative<SparseArray<Component>>(_storage)) {
                return std::get<SparseArray<Component>>(_storage).insert_at(id, component);
            }
            return std::get<CompactArray<Component>>(_storage).insert_at(id, component);
        }

        void erase(entity_id id) {
            if (std::holds_alternative<SparseArray<Component>>(_storage)) {
                std::get<SparseArray<Component>>(_storage).erase(id);
            }
            std::get<CompactArray<Component>>(_storage).erase(id);
        }

        hybrid_storage _storage;
    };
}
