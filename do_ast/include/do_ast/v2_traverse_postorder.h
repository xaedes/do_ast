#pragma once

#include <vector>
#include <limits>
#include <type_traits>

namespace do_ast {

    template <
        class TNodeForest,
        template<class...> class TContainer = TNodeForest::Container
    >
    struct TraversePostorder
    {
        using NodeForest = TNodeForest;
        using Nodes      = typename NodeForest::Nodes;
        using Index      = typename NodeForest::Index;
        using Size       = typename NodeForest::Size;
        template<class...Args> using Container  = TContainer<Args...>;

        static Index InvalidIndex() { return std::numeric_limits<Index>::max(); }
        static Size InvalidSize() { return std::numeric_limits<Size>::max(); }

    public:

        void build(const TNodeForest& forest, Index root)
        {
            // traverse(node) {
            //   for(child: children) {
            //     traverse(node)
            //   } 
            //   set_next_node(node)
            // }

            resize(forest.size());
            const auto* go_up   = forest.up();
            const auto* go_down = forest.down();
            const auto* go_prev = forest.prev();
            const auto* go_next = forest.next();

            // const auto* preorder_next = preorder.next();
            // const auto* preorder_skip = preorder.skip();

            auto* next = this->next();

            const Index invalid = InvalidIndex();
            const Index finished = InvalidIndex();

            struct StackItem
            {
                Index node;
                Index child;

                StackItem(Index node, Index child) : node(node), child(child) {}
            };
            static Container<StackItem> s_stack;
            s_stack.clear();

            s_stack.emplace_back(root, go_down[root]);

            Index front = invalid;
            Index back = invalid;

            while (s_stack.size() > 0)
            {
                auto& current = s_stack.back();

                if (current.child != finished)
                {
                    // recurse down the tree
                    Index node = current.child;
                    Index child = go_down[current.child];
                    // advance current child
                    current.child = go_next[current.child]; 
                    // recurse down the tree
                    s_stack.emplace_back(node, child);
                    continue;
                }
                else
                {
                    // no more children to traverse
                    if (back == invalid)
                    {
                        front = current.node;
                    }
                    else
                    {
                        next[back] = current.node;
                    }
                    back = current.node;
                    s_stack.pop_back();
                }
            }
            m_front = front;
            m_back = back;
            next[back] = invalid;
        }

    public:
        Size size() const { return m_size; }
        Index front() const { return m_front; }
        Index back() const { return m_back; }

        void resize(Size size)
        {
            m_size = size;
            m_next.resize(size);
            // m_skip.resize(size);
            m_next_data = m_next.data();
            // m_skip_data = m_skip.data();
        }

        void clear()
        {
            resize(0);
        }

        Index* next()  { return m_next_data; }
        // Index* skip()  { return m_skip_data; }
        const Index* next() const { return m_next_data; }
        // const Index* skip() const { return m_skip_data; }

    protected:
        Size m_size = 0;
        Index m_front = 0;
        Index m_back = 0;
        Container<Index> m_next;
        // Container<Index> m_skip;
        Index* m_next_data;
        // Index* m_skip_data;
    };

} // namespace do_ast
