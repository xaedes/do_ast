#pragma once

#include <tuple>
#include <type_traits>

#include <do_ast/item_pool.h>

namespace do_ast {

    template<class... Args>
    struct ItemPoolTuple
    {
        using tuple_type = std::tuple<std::vector<Args>...>;
        using num_types = std::tuple_size<tuple_type>;
        using size_type = typename std::vector<int>::size_type;
        template<std::size_t K> using get_type = std::tuple_element_t<K, std::tuple<Args...>>;
        template<std::size_t K> using get_slots_type = std::tuple_element_t<K, tuple_type>;
        
        template<std::size_t K>       get_slots_type<K>& slots();
        template<std::size_t K> const get_slots_type<K>& slots() const;
        template<std::size_t K>       get_type<K>& get(ItemPoolIndex idx);
        template<std::size_t K> const get_type<K>& get(ItemPoolIndex idx) const;
        template<std::size_t K>       get_type<K>& at(ItemPoolIndex idx);
        template<std::size_t K> const get_type<K>& at(ItemPoolIndex idx) const;

        void clear();

        bool contains(ItemPoolIndex idx) const;
        size_type size() const;

        ItemPoolIndex insert();
        ItemPoolIndex insert(Args... value);

        void erase(ItemPoolIndex idx);

        template<class SlotsVisitor>
        void visit_slots(SlotsVisitor& slots_visitor);
        
        ItemPoolIndex index(std::size_t index) const;

    protected:

        std::tuple<std::vector<Args>...> m_slots;
        std::vector<uint32_t> m_slot_smcs; // sequential modification counters
        std::vector<bool> m_occupied_slots;
        std::vector<ItemPoolIndex> m_free_slot_ids;
        std::size_t m_size = 0;
    };

} // namespace do_ast

#include <do_ast/item_pool_tuple.impl.h>