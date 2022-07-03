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
    struct NodesV2_
    {
        using NodeType   = TNodeType;
        using NodeValue  = TNodeValue;
        using Size       = TSize;
        template<class...Args> using Container  = TContainer<Args...>;

        static Size InvalidSize() { return std::numeric_limits<Size>::max(); }

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

} // namespace do_ast
