#pragma once

#include <vector>
#include <limits>

#include <do_ast/arg_template_helpers.h>

namespace do_ast {

    template <
        class TNode,
        class TIndex = std::size_t,
        class TSize = std::size_t,
        template<class...> class TContainer = std::vector,
        bool TEnableDebug = false
    >
    struct Nodes_
    {
        using Node       = TNode;
        using Index      = TIndex;
        using Size       = TSize;
        template<class...Args> using Container  = TContainer<Args...>;

        static Index InvalidIndex() { return std::numeric_limits<Index>::max(); }
        static Size InvalidSize() { return std::numeric_limits<Size>::max(); }

        Index insert()
        {
            if (size() == capacity())
            {
                grow();
            } 
            Index result = static_cast<Index>(size());
            m_size++;
            return result;
        }

        Index insert(const Node& value)
        {
            Index result = insert();
            node(result) = value;
            return result;
        }

        template<class... ChildrenIndices>
        Index insert(const Node& value, ChildrenIndices... children)
        {
            Index result = insert();
            node(result) = value;
            set_children(result, children...);
            return result;
        }
        Index insert_fast()
        {
            return static_cast<Index>(m_size++);
        }

        Index insert_fast(const Node& value)
        {
            Index result = insert_fast();
            node(result, value);
            return result;
        }

        template<class... ChildrenIndices>
        Index insert_fast(const Node& value, ChildrenIndices... children)
        {
            Index result = insert_fast();
            node(result) = value;
            set_children(result, children...);
            return result;
        }

        template<class... Children>
        void set_children(Index parent, Children... children)
        {
            do_ast::visit_n_args<1>([this, parent](auto item){
                down(parent, item);
            }, children...);
            do_ast::visit_args([this, parent](auto item){
                up(item, parent);
            }, children...);
            do_ast::foldl_args([this](auto lhs, auto rhs){
                prev(rhs, lhs);
                next(lhs, rhs);
            }, children...);
        }

        void node(Index idx, const Node& value)   { m_nodes_ptr[idx] = value; }
        void up  (Index idx, const Index& value)  { m_up_ptr[idx]    = value; }
        void down(Index idx, const Index& value)  { m_down_ptr[idx]  = value; }
        void prev(Index idx, const Index& value)  { m_prev_ptr[idx]  = value; }
        void next(Index idx, const Index& value)  { m_next_ptr[idx]  = value; }

        Node& node(Index idx)  { return m_nodes_ptr[idx]; }
        Index& up(Index idx)   { return m_up_ptr[idx]; }
        Index& down(Index idx) { return m_down_ptr[idx]; }
        Index& prev(Index idx) { return m_prev_ptr[idx]; }
        Index& next(Index idx) { return m_next_ptr[idx]; }

        const Node& node(Index idx)  const { return m_nodes_ptr[idx]; }
        const Index& up(Index idx)   const { return m_up_ptr[idx]; }
        const Index& down(Index idx) const { return m_down_ptr[idx]; }
        const Index& prev(Index idx) const { return m_prev_ptr[idx]; }
        const Index& next(Index idx) const { return m_next_ptr[idx]; }

        void grow()
        {
            reserve(size() + 8 + (size()/2));
        }

        void resize(Size size)
        {
            m_size = size;
            resize_containers(size);
        }

        void reserve(Size size)
        {
            if (TEnableDebug)
            {
                if (size <= m_capacity) return;
            }
            resize_containers(size);
        }

        void clear()
        {
            m_size = 0;
        }

        void shrink()
        {
            resize_containers(m_size);
        }

        void clear_shrink()
        {
            clear();
            shrink();
        }

        Size size() const { return m_size; }
        Size capacity() const { return m_capacity; }

    protected:

        void resize_containers(Size size)
        {
            m_capacity = size;
            m_nodes.resize(size);
            m_up.resize(size, InvalidIndex());
            m_down.resize(size, InvalidIndex());
            m_prev.resize(size, InvalidIndex());
            m_next.resize(size, InvalidIndex());
            m_nodes_ptr = m_nodes.data();
            m_up_ptr = m_up.data();
            m_down_ptr = m_down.data();
            m_prev_ptr = m_prev.data();
            m_next_ptr = m_next.data();
        }

        Size m_size = 0;
        Size m_capacity = 0;
        Container<Node> m_nodes;
        Container<Index> m_up;
        Container<Index> m_down;
        Container<Index> m_prev;
        Container<Index> m_next;

        Node* m_nodes_ptr;
        Index* m_up_ptr;
        Index* m_down_ptr;
        Index* m_prev_ptr;
        Index* m_next_ptr;
    };

} // namespace do_ast
