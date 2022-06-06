#pragma once

#include <do_ast/item_pool.h>

namespace do_ast {

    template<class T>
    ItemPoolIndex ItemPool<T>::index(std::size_t index) const
    {
        auto smc = index < m_slot_smcs.size() ? m_slot_smcs[index] : 0;
        return ItemPoolIndex{index, smc};
    }
    
    template<class T>
    T& ItemPool<T>::get(ItemPoolIndex idx)
    {
        return m_slots[idx.index];
    }
    
    template<class T>
    const T& ItemPool<T>::get(ItemPoolIndex idx) const
    {
        return m_slots[idx.index];
    }
    
    template<class T>
    T& ItemPool<T>::at(ItemPoolIndex idx)
    {
        assert(contains(idx));
        return m_slots.at(idx.index);
    }
    
    template<class T>
    const T& ItemPool<T>::at(ItemPoolIndex idx) const
    {
        assert(contains(idx));
        return m_slots.at(idx.index);
    }
    
    template<class T>
    void ItemPool<T>::clear()
    {
        m_slots.clear();
        m_occupied_slots.clear();
        m_free_slot_ids.clear();
    }
    
    template<class T>
    typename ItemPool<T>::size_type ItemPool<T>::size() const
    {
        return m_slots.size() - m_free_slot_ids.size();
    }
    
    template<class T>
    ItemPoolIndex ItemPool<T>::insert()
    {
        if (m_free_slot_ids.size() > 0)
        {
            ItemPoolIndex new_idx;
            new_idx.index = m_free_slot_ids.back().index;
            m_slot_smcs[new_idx.index]++;
            new_idx.smc = m_slot_smcs[new_idx.index];
            m_free_slot_ids.pop_back();
            m_occupied_slots[new_idx.index] = true;
            return new_idx;
        }
        else
        {
            ItemPoolIndex new_idx;
            new_idx.index = m_slots.size();
            new_idx.smc = 1;
            m_slots.emplace_back();
            m_slot_smcs.emplace_back(new_idx.smc);
            m_occupied_slots.push_back(true);
            return new_idx;
        }
    }
    
    template<class T>
    ItemPoolIndex ItemPool<T>::insert(const T& value)
    {
        ItemPoolIndex new_idx = insert();
        m_slots[new_idx.index] = value;
        return new_idx;
    }
    
    template<class T>
    template<class... Args>
    ItemPoolIndex ItemPool<T>::emplace(Args... args)
    {
        ItemPoolIndex new_idx = insert();
        m_slots[new_idx.index] = T(args...);
        return new_idx;
    }
    
    template<class T>
    void ItemPool<T>::erase(ItemPoolIndex idx)
    {
        if (!m_occupied_slots[idx.index]) return;
        m_slot_smcs[idx.index]++;
        m_occupied_slots[idx.index] = false;
        m_free_slot_ids.push_back(idx);
    }
    
    template<class T>
    bool ItemPool<T>::contains(ItemPoolIndex idx) const
    {
        return m_occupied_slots[idx.index] && (m_slot_smcs[idx.index] == idx.smc);
    }


} // namespace do_ast
