#pragma once

#include <vector>
#include <limits>
#include <type_traits>

#include <do_ast/arg_template_helpers.h>

namespace do_ast {

    template <
        class TNodeType = int,
        class TNodeValue = int,
        class TSize = std::size_t,
        template<class...> class TContainer = std::vector
    >
    struct NodeItemsV2_
    {
        using NodeType   = TNodeType;
        using NodeValue  = TNodeValue;
        using Size       = TSize;
        template<class...Args> using Container  = TContainer<Args...>;

        Size size() const { return m_size; }

        void resize(Size size)
        {
            m_size = size;
            m_type_container.resize(size);
            m_value_container.resize(size);
            m_type_data = m_type_container.data();
            m_value_data = m_value_container.data();
        }

        void clear()
        {
            resize(0);
        }

        NodeType*  type()   { return m_type_data; }
        NodeValue* value()  { return m_value_data; }
        const NodeType*  type()  const { return m_type_data; }
        const NodeValue* value() const { return m_value_data; }

    protected:
        Size m_size = 0;
        Container<NodeType> m_type_container;
        Container<NodeValue> m_value_container;
        NodeType* m_type_data;
        NodeValue* m_value_data;
    };

    template<class Index, class... Children>
    void set_children(
        Index* up,
        Index* down,
        Index* prev,
        Index* next,
        Index parent, 
        Children... children
    )
    {
        do_ast::visit_n_args<1>([down, parent](auto item){
            down[parent] = item;
        }, children...);
        do_ast::visit_args([up, parent](auto item){
            up[item] = parent;
        }, children...);
        do_ast::foldl_args([prev,next](auto lhs, auto rhs){
            prev[rhs] = lhs;
            next[lhs] = rhs;
        }, children...);
    }

    template <
        class TNodes,
        class TIndex = std::size_t,
        class TSize = std::size_t,
        template<class...> class TContainer = std::vector
    >
    struct NodesV2_
    {
        using Nodes      = TNodes;
        using Index      = TIndex;
        using Size       = TSize;
        template<class...Args> using Container  = TContainer<Args...>;

        static Index InvalidIndex() { return std::numeric_limits<Index>::max(); }
        static Size InvalidSize() { return std::numeric_limits<Size>::max(); }

        Size size() const { return m_size; }

        template<class... Children>
        void set_children(Index parent, Children... children)
        {
            auto* up = this->up();
            auto* down = this->down();
            auto* prev = this->prev();
            auto* next = this->next();
            do_ast::set_children(up(), down(), prev(), next(), parent, children...);
        }

        void set_children_fast(Index parent)
        {
            m_down_data[parent] = InvalidIndex();
        }
        void set_children_fast(Index parent, Index child0)
        {
            m_up_data[child0] = parent;
            m_prev_data[child0] = InvalidIndex();
            m_next_data[child0] = InvalidIndex();
            m_down_data[parent] = child0;
        }
        void set_children_fast(Index parent, Index child0, Index child1)
        {
            m_up_data[child0] = parent;
            m_up_data[child1] = parent;
            m_prev_data[child0] = InvalidIndex();
            m_prev_data[child1] = child0;
            m_next_data[child0] = child1;
            m_next_data[child1] = InvalidIndex();
            m_down_data[parent] = child0;
        }
        void set_children_fast(Index parent, Index child0, Index child1, Index child2)
        {
            m_up_data[child0] = parent;
            m_up_data[child1] = parent;
            m_up_data[child2] = parent;
            m_prev_data[child0] = InvalidIndex();
            m_prev_data[child1] = child0;
            m_prev_data[child2] = child1;
            m_next_data[child0] = child1;
            m_next_data[child1] = child2;
            m_next_data[child2] = InvalidIndex();
            m_down_data[parent] = child0;
        }
        void set_children_fast(Index parent, Index child0, Index child1, Index child2, Index child3)
        {
            m_up_data[child0] = parent;
            m_up_data[child1] = parent;
            m_up_data[child2] = parent;
            m_up_data[child3] = parent;
            m_prev_data[child0] = InvalidIndex();
            m_prev_data[child1] = child0;
            m_prev_data[child2] = child1;
            m_prev_data[child3] = child2;
            m_next_data[child0] = child1;
            m_next_data[child1] = child2;
            m_next_data[child2] = child3;
            m_next_data[child3] = InvalidIndex();
            m_down_data[parent] = child0;
        }

        void resize(Size size)
        {
            m_size = size;
            m_nodes.resize(size);
            m_up.resize(size);
            m_down.resize(size);
            m_prev.resize(size);
            m_next.resize(size);
            m_up_data = m_up.data();
            m_down_data = m_down.data();
            m_prev_data = m_prev.data();
            m_next_data = m_next.data();
        }

        void clear()
        {
            m_size = 0;
        }

        void shrink()
        {
            resize(size());
        }

        Nodes& nodes() { return m_nodes; }
        Index* up()    { return m_up_data; }
        Index* down()  { return m_down_data; }
        Index* prev()  { return m_prev_data; }
        Index* next()  { return m_next_data; }

        const Nodes& nodes() const { return m_nodes; }
        const Index* up()    const { return m_up_data; }
        const Index* down()  const { return m_down_data; }
        const Index* prev()  const { return m_prev_data; }
        const Index* next()  const { return m_next_data; }

    protected:
        Size m_size = 0;
        Nodes m_nodes;
        Container<Index> m_up;
        Container<Index> m_down;
        Container<Index> m_prev;
        Container<Index> m_next;
        Index* m_up_data;
        Index* m_down_data;
        Index* m_prev_data;
        Index* m_next_data;
    };

} // namespace do_ast
