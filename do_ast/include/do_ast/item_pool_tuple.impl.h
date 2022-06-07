#pragma once

#include <do_ast/item_pool_tuple.h>

namespace do_ast {

    template<class... Args>
    ItemPoolIndex ItemPoolTuple<Args...>::index(std::size_t index) const
    {
        auto smc = index < m_slot_smcs.size() ? m_slot_smcs[index] : 0;
        return ItemPoolIndex{index, smc};
    }

    template<class... Args>
    template<std::size_t K>
    std::tuple_element_t<K, std::tuple<std::vector<Args>...>>& ItemPoolTuple<Args...>::slots()
    {
        return std::get<K>(m_slots);
    }
    
    template<class... Args>
    template<std::size_t K>
    const std::tuple_element_t<K, std::tuple<std::vector<Args>...>>& ItemPoolTuple<Args...>::slots() const
    {
        return std::get<K>(m_slots);
    }
    
    template<class... Args>
    template<std::size_t K>
    std::tuple_element_t<K, std::tuple<Args...>>& ItemPoolTuple<Args...>::get(ItemPoolIndex idx)
    {
        return std::get<K>(m_slots)[idx.index];
    }
    
    template<class... Args>
    template<std::size_t K>
    const std::tuple_element_t<K, std::tuple<Args...>>& ItemPoolTuple<Args...>::get(ItemPoolIndex idx) const
    {
        return std::get<K>(m_slots)[idx.index];
    }
    
    template<class... Args>
    template<std::size_t K>
    std::tuple_element_t<K, std::tuple<Args...>>& ItemPoolTuple<Args...>::at(ItemPoolIndex idx)
    {
        assert(contains(idx));
        return std::get<K>(m_slots).at(idx.index);
    }
    
    template<class... Args>
    template<std::size_t K>
    const std::tuple_element_t<K, std::tuple<Args...>>& ItemPoolTuple<Args...>::at(ItemPoolIndex idx) const
    {
        assert(contains(idx));
        return std::get<K>(m_slots).at(idx.index);
    }
    
    struct Clear
    {
        template<std::size_t Idx, class T>
        void visit(T& slots)
        {
            slots.clear();
        }
    };

    template<class... Args>
    void ItemPoolTuple<Args...>::clear()
    {
        visit_slots(Clear());
        
        m_occupied_slots.clear();
        m_free_slot_ids.clear();
        m_size = 0;
    }
    
    template<class... Args>
    typename ItemPoolTuple<Args...>::size_type ItemPoolTuple<Args...>::size() const
    {
        return m_size;
    }
    
    struct EmplaceBack
    {
        template<std::size_t Idx, class T>
        void visit(T& slots)
        {
            slots.emplace_back();
        }
    };

    template<class... Args>
    ItemPoolIndex ItemPoolTuple<Args...>::insert()
    {
        if (m_free_slot_ids.size() > 0)
        {
            ItemPoolIndex new_idx;
            new_idx.index = m_free_slot_ids.back().index;
            m_slot_smcs[new_idx.index]++;
            new_idx.smc = m_slot_smcs[new_idx.index];
            ++m_size;
            m_free_slot_ids.pop_back();
            m_occupied_slots[new_idx.index] = true;
            return new_idx;
        }
        else
        {
            ItemPoolIndex new_idx;
            new_idx.index = m_size;
            new_idx.smc = 1;
            ++m_size;

            visit_slots(EmplaceBack());

            m_slot_smcs.emplace_back(new_idx.smc);
            m_occupied_slots.push_back(true);
            return new_idx;
        }
    }
    
    template<class... Args>
    struct Assign
    {
        using tuple_type = std::tuple<Args...>;
        tuple_type values_tuple;
        ItemPoolIndex new_idx;

        template<std::size_t Idx, class T>
        void visit(T& slots)
        {
            slots[new_idx.index] = std::get<Idx>(values_tuple);
        }
    };

    template<class... Args>
    ItemPoolIndex ItemPoolTuple<Args...>::insert(Args... values)
    {
        // ItemPoolIndex new_idx = insert();


        // tuple_type values_tuple = std::make_tuple(values...);


        Assign<Args...> assign;
        assign.values_tuple = std::make_tuple(values...);
        assign.new_idx = insert();
        visit_slots(assign);

        // m_slots[new_idx.index] = value;
        return assign.new_idx;
    }
    
    template<class... Args>
    void ItemPoolTuple<Args...>::erase(ItemPoolIndex idx)
    {
        if (!m_occupied_slots[idx.index]) return;
        m_slot_smcs[idx.index]++;
        m_occupied_slots[idx.index] = false;
        m_free_slot_ids.push_back(idx);
        --m_size;
    }
    
    template<class... Args>
    bool ItemPoolTuple<Args...>::contains(ItemPoolIndex idx) const
    {
        // return m_occupied_slots[idx.index] && (m_slot_smcs[idx.index] == idx.smc);
        return (m_slot_smcs[idx.index] == idx.smc);
    }

    template<class tuple_type_>
    struct TupleVisitor
    {
        using tuple_type = tuple_type_;
        using num_types = std::tuple_size<tuple_type>;

        template<class Visitor, std::size_t Idx, std::enable_if_t<(Idx < num_types::value), bool> = true>
        void visit(Visitor& visitor, tuple_type& tuple)
        {
            visitor.visit<Idx>(std::get<Idx>(tuple));
            visit<Visitor,Idx+1>(visitor, tuple);
        };
        template<class Visitor, std::size_t Idx, std::enable_if_t<(Idx >= num_types::value), bool> = true>
        void visit(Visitor& visitor, tuple_type& tuple)
        {
        };
    };
    
    template<class... Args>
    using TupleVisitor_ = TupleVisitor<std::tuple<Args...>>;

    template<class... Args>
    template<class SlotsVisitor>
    void ItemPoolTuple<Args...>::visit_slots(SlotsVisitor& slots_visitor)
    {
        TupleVisitor<tuple_type> tuple_visitor;

        tuple_visitor.visit<SlotsVisitor,0>(slots_visitor, m_slots);
    }


} // namespace do_ast
