#pragma once

#include <vector>

namespace do_ast {

    template<class T>
    struct ItemPool
    {
        using size_type = typename std::vector<T>::size_type;
        using index_type = size_type;
        
        T& operator[](index_type idx);
        const T& operator[](index_type idx) const;
        T& at(index_type idx);
        const T& at(index_type idx) const;
        void clear();
        size_type size() const;
        index_type insert();
        index_type insert(const T& value);
        template <class... Args> index_type emplace(Args... args);
        void erase(index_type idx);
        bool contains(index_type idx);

    protected:
        std::vector<T> m_slots;
        std::vector<bool> m_occupied_slots;
        std::vector<index_type> m_free_slot_ids;
    };

} // namespace do_ast

#include <do_ast/item_pool.impl.h>