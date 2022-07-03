#pragma once

#include <vector>
#include <limits>
#include <type_traits>

namespace do_ast {

    template <
        class TNodeForest,
        class TDepth = TNodeForest::Size,
        template<class...> class TContainer = TNodeForest::Container
    >
    struct NodeDepth
    {
        using NodeForest = TNodeForest;
        using Depth      = TDepth;
        using Nodes      = typename NodeForest::Nodes;
        using Index      = typename NodeForest::Index;
        using Size       = typename NodeForest::Size;
        template<class...Args> using Container  = TContainer<Args...>;

        static Index InvalidIndex() { return std::numeric_limits<Index>::max(); }
        static Size InvalidSize() { return std::numeric_limits<Size>::max(); }

    public:

        template<template<class...> class TC = Container>
        void build(const TNodeForest& forest, const TraversePreorder<NodeForest, TC>& preorder)
        {
            resize(forest.size());
            const auto* go_up   = forest.up();

            const auto* preorder_next = preorder.next();

            const Index invalid = InvalidIndex();
            Index current = preorder.front();
            m_depth_data[current] = 0;
            m_max_depth = 0;
            current = preorder_next[current];
            while (current != invalid)
            {
                auto depth = m_depth_data[go_up[current]] + 1;
                m_depth_data[current] = depth;
                m_max_depth = (depth > m_max_depth) ? depth : m_max_depth;
                current = preorder_next[current];
            }
        }

    public:
        Size size() const { return m_size; }
        Depth max_depth() const { return m_max_depth; }

        void set_max_depth(Depth val)
        {
            m_max_depth = val;
        }
        
        void resize(Size size)
        {
            m_size = size;
            m_depth.resize(size);
            m_depth_data = m_depth.data();
        }

        void clear()
        {
            resize(0);
        }

        Depth* depth()  { return m_depth_data; }
        const Depth* depth() const { return m_depth_data; }

    protected:
        Size m_size = 0;
        Container<Depth> m_depth;
        Depth m_max_depth = 0;
        Index* m_depth_data = nullptr;
    };

} // namespace do_ast
