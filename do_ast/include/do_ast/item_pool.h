#pragma once

#include <vector>

namespace do_ast {

    struct ItemPoolIndex
    {
        std::size_t index = 0;
        uint32_t smc = 0; // sequential modification counter
    };
    

    template<class T>
    struct ItemPool
    {
        using size_type = typename std::vector<T>::size_type;
        //using index_type = size_type;

        T& get(ItemPoolIndex idx);
        const T& get(ItemPoolIndex idx) const;
        T& at(ItemPoolIndex idx);
        const T& at(ItemPoolIndex idx) const;
        void clear();
        size_type size() const;
        ItemPoolIndex insert();
        ItemPoolIndex insert(const T& value);
        template <class... Args> ItemPoolIndex emplace(Args... args);
        void erase(ItemPoolIndex idx);
        bool contains(ItemPoolIndex idx) const;

        ItemPoolIndex index(std::size_t index) const;

    protected:
        std::vector<T> m_slots;
        std::vector<uint32_t> m_slot_smcs; // sequential modification counters
        std::vector<bool> m_occupied_slots;
        std::vector<ItemPoolIndex> m_free_slot_ids;
    };

} // namespace do_ast

#include <do_ast/item_pool.impl.h>