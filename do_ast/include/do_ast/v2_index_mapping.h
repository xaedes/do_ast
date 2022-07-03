#pragma once

#include <vector>
#include <limits>
#include <type_traits>

namespace do_ast {

    template<
        class TIndex = std::size_t,
        class TSize = std::size_t,
        template<class...> class TContainer = std::vector
    >
    struct IndexMapping 
    {
        using Index      = TIndex;
        using Size       = TSize;
        template<class...Args> using Container  = TContainer<Args...>;

        static Index InvalidIndex() { return std::numeric_limits<Index>::max(); }
        static Size InvalidSize() { return std::numeric_limits<Size>::max(); }

        Size size() const { return m_size; }

        void resize(Size size)
        {
            m_size = size;
            m_map.resize(size);
            m_map_data = m_map.data();
        }

        void clear()
        {
            resize(0);
        }

        Index* map() { return m_map_data; }
        const Index* map() const { return m_map_data; }

    protected:
        Size m_size = 0;
        Container<Index> m_map;
        Index* m_map_data = nullptr;
    };

} // namespace do_ast
