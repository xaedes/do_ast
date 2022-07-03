#pragma once

namespace do_ast {
template<class TForest>
struct NodeTypeValueInserter
{
    using Forest    = TForest;
    using Nodes     = typename Forest::Nodes;
    using Index     = typename Forest::Index;
    using Size      = typename Forest::Size;
    using NodeType  = typename Forest::Nodes::NodeType;
    using NodeValue = typename Forest::Nodes::NodeValue;

    static Index InvalidIndex() { return std::numeric_limits<Index>::max(); }
    static Size InvalidSize() { return std::numeric_limits<Size>::max(); }

    NodeTypeValueInserter() = default;
    NodeTypeValueInserter(Forest* forest)
    :type(forest->nodes().type())
    ,value(forest->nodes().value())
    ,up(forest->up())
    ,prev(forest->prev())
    ,next(forest->next())
    ,down(forest->down())
    {}

    template<class T, class V, class... Children>
    Index operator()(
        T ntype,
        V nvalue,
        Children... children
    )
    {
        Index nid  = static_cast<Index>(m_size++);
        type[nid]  = static_cast<NodeType>(ntype);
        value[nid] = static_cast<NodeValue>(nvalue);
        up[nid] = InvalidIndex();
        prev[nid] = InvalidIndex();
        next[nid] = InvalidIndex();
        set_children(nid, children...);
        return nid;
    }

    Size size() const { return m_size; }

protected:
    Size m_size = 0;
    NodeType* type = nullptr;
    NodeValue* value = nullptr;
    Index* up = nullptr;
    Index* prev = nullptr;
    Index* next = nullptr;
    Index* down = nullptr;

    template<class... Children, std::enable_if_t<(sizeof...(Children) > 4), bool> = true>
    void set_children(Index parent, Children... children)
    {
        do_ast::set_children(up, down, prev, next, parent, children...);
    }

    void set_children(Index parent)
    {
        down[parent] = InvalidIndex();
    }
    void set_children(Index parent, Index child0)
    {
        up[child0] = parent;
        prev[child0] = InvalidIndex();
        next[child0] = InvalidIndex();
        down[parent] = child0;
    }
    void set_children(Index parent, Index child0, Index child1)
    {
        up[child0] = parent;
        up[child1] = parent;
        prev[child0] = InvalidIndex();
        prev[child1] = child0;
        next[child0] = child1;
        next[child1] = InvalidIndex();
        down[parent] = child0;
    }
    void set_children(Index parent, Index child0, Index child1, Index child2)
    {
        up[child0] = parent;
        up[child1] = parent;
        up[child2] = parent;
        prev[child0] = InvalidIndex();
        prev[child1] = child0;
        prev[child2] = child1;
        next[child0] = child1;
        next[child1] = child2;
        next[child2] = InvalidIndex();
        down[parent] = child0;
    }
    void set_children(Index parent, Index child0, Index child1, Index child2, Index child3)
    {
        up[child0] = parent;
        up[child1] = parent;
        up[child2] = parent;
        up[child3] = parent;
        prev[child0] = InvalidIndex();
        prev[child1] = child0;
        prev[child2] = child1;
        prev[child3] = child2;
        next[child0] = child1;
        next[child1] = child2;
        next[child2] = child3;
        next[child3] = InvalidIndex();
        down[parent] = child0;
    }    
};


} // namespace do_ast

