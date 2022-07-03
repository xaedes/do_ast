#pragma once

#include <vector>
#include <limits>
#include <type_traits>

namespace do_ast {

    template <
        class TNodeForest,
        template<class...> class TContainer = TNodeForest::Container
    >
    struct TraversePreorder
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
            m_count = 0;
            resize(forest.size());

            const Index invalid = InvalidIndex();

            m_front = root;
            m_back = root;

            if (root == invalid) return;

            const auto* go_up   = forest.up();
            const auto* go_down = forest.down();
            const auto* go_prev = forest.prev();
            const auto* go_next = forest.next();

            auto* next = m_next_data;
            auto* skip = m_skip_data;

            Index current = root;
            Index back = root;
            while (current != invalid)
            {
                ++m_count;

                back = current;
                Index cur_up = go_up[current];
                Index cur_down = go_down[current];
                Index cur_next = go_next[current];

                const bool has_next = (cur_next != invalid);
                const bool has_up   = (cur_up   != invalid);
                const bool has_down = (cur_down != invalid);

                Index skip_cur = (
                    has_next ? cur_next     :
                    has_up   ? skip[cur_up] :
                    invalid
                );
                Index next_cur = has_down ? cur_down : skip_cur;
                skip[current] = skip_cur;
                next[current] = next_cur;
                current = next_cur;
            }
            m_back = back;
        }

    public:
        Size size() const { return m_size; }
        Size count() const { return m_count; }
        Index front() const { return m_front; }
        Index back() const { return m_back; }

        void set_count(Size count) 
        {
            m_count = count;
        }
        void set_front(Index front) 
        {
            m_front = front;
        }
        void set_back(Index back) 
        {
            m_back = back;
        }
        void resize(Size size)
        {
            m_size = size;
            m_next.resize(size);
            m_skip.resize(size);
            m_next_data = m_next.data();
            m_skip_data = m_skip.data();
        }

        void clear()
        {
            m_count = 0;
            resize(0);
        }

        Index* next()  { return m_next_data; } 
        Index* skip()  { return m_skip_data; }
        const Index* next() const { return m_next_data; }
        const Index* skip() const { return m_skip_data; }

    protected:
        Size m_size = 0;
        Size m_count = 0;
        Container<Index> m_next;
        Container<Index> m_skip;
        Index* m_next_data = nullptr;
        Index* m_skip_data = nullptr;
        Index m_front = InvalidIndex();
        Index m_back = InvalidIndex();
    };

} // namespace do_ast
