#pragma once

#include <do_ast/item_pool.h>

namespace do_ast {

    
    template<class T>
    T& ItemPool<T>::operator[](index_type idx)
    {
        return m_slots[idx];
    }
    
    template<class T>
    const T& ItemPool<T>::operator[](index_type idx) const
    {
        return m_slots[idx];
    }
    
    template<class T>
    T& ItemPool<T>::at(index_type idx)
    {
        assert(contains(idx));
        return m_slots.at(idx);
    }
    
    template<class T>
    const T& ItemPool<T>::at(index_type idx) const
    {
        assert(contains(idx));
        return m_slots.at(idx);
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
    typename ItemPool<T>::index_type ItemPool<T>::insert()
    {
        if (m_free_slot_ids.size() > 0)
        {
            index_type new_idx = m_free_slot_ids.back();
            m_free_slot_ids.pop_back();
            m_occupied_slots[new_idx] = true;
            return new_idx;
        }
        else
        {
            index_type new_idx = m_slots.size();
            m_slots.emplace_back();
            m_occupied_slots.push_back(true);
            return new_idx;
        }
    }
    
    template<class T>
    typename ItemPool<T>::index_type ItemPool<T>::insert(const T& value)
    {
        index_type new_idx = insert();
        m_slots[new_idx] = value;
        return new_idx;
    }
    
    template<class T>
    template<class... Args>
    typename ItemPool<T>::index_type ItemPool<T>::emplace(Args... args)
    {
        index_type new_idx = insert();
        m_slots[new_idx] = T(args...);
        return new_idx;
    }
    
    template<class T>
    void ItemPool<T>::erase(index_type idx)
    {
        if (!m_occupied_slots[idx]) return;
        m_occupied_slots[idx] = false;
        m_free_slot_ids.push_back(idx);
    }
    
    template<class T>
    bool ItemPool<T>::contains(index_type idx)
    {
        return m_occupied_slots[idx];
    }


} // namespace do_ast
