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

            auto* next = this->next();

            const Index invalid = InvalidIndex();
            const Index finished = InvalidIndex();

            struct StackItem
            {
                Index node;
                Index child;

                StackItem(Index node, Index child) : node(node), child(child) {}
            };
            static Container<Index> s_stack_node;
            static Container<Index> s_stack_child;
            // expected maximum depth of stack is: log(max_num_items=2^(num_bits=sizeof(Size)*8))=sizeof(Size)*8
            // allocate +1 to avoid need for allocation on last level of expected maximum depth (which is sizeof(Size)*8)
            Size stack_capacity = sizeof(Size)*8 + 1; 
            s_stack_node.clear();
            s_stack_child.clear();
            s_stack_node.resize(stack_capacity);
            s_stack_child.resize(stack_capacity);
            auto* stack_node = s_stack_node.data();
            auto* stack_child = s_stack_child.data();
            Index stack_size = 0;
            stack_node[stack_size] = root;
            stack_child[stack_size] = go_down[root];
            ++stack_size;

            Index front = invalid;
            Index back = invalid;

            while (stack_size)
            {
                if (stack_size == stack_capacity)
                {
                    // grow stack
                    stack_capacity += (stack_capacity >> 1);
                    s_stack_node.resize(stack_capacity);
                    s_stack_child.resize(stack_capacity);
                    stack_node = s_stack_node.data();
                    stack_child = s_stack_child.data();                    
                }

                auto& current_node = stack_node[stack_size-1];
                auto& current_child = stack_child[stack_size-1];

                if (current_child != finished)
                {
                    // recurse down the tree
                    stack_node[stack_size] = current_child;
                    stack_child[stack_size] = go_down[current_child];
                    // advance current child
                    current_child = go_next[current_child]; 
                    // recurse down the tree
                    ++stack_size;
                    continue;
                }
                else
                {
                    // no more children to traverse
                    if (back == invalid)
                    {
                        front = current_node;
                    }
                    else
                    {
                        next[back] = current_node;
                    }
                    back = current_node;
                    --stack_size;
                }
            }
            m_front = front;
            m_back = back;
            next[back] = invalid;

            s_stack_node.clear();
            s_stack_child.clear();

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
