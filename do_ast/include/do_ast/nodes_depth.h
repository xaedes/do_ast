#pragma once

#include <vector>
#include <cstdint>
#include <type_traits>

namespace do_ast {

    template <
        class TNodeId = int64_t, 
        class TDepth = uint32_t,
        class TSize = int64_t
    >
    struct NodesDepth
    {
        static_assert(std::is_integral<TNodeId>::value, "std::is_integral<TNodeId>::value");
        static_assert(std::is_integral<TDepth>::value, "std::is_integral<TDepth>::value");
        static_assert(std::is_integral<TSize>::value, "std::is_integral<TSize>::value");

        using NodeId = TNodeId;
        using Depth = TDepth;
        using Size = TSize;

        std::vector<Depth> depth;

        void clear()
        {
            depth.clear();
        }

        Depth& operator[](NodeId id) { return depth[id]; }
        Depth operator[](NodeId id) const { return depth[id]; }

        Size size() const { return depth.size(); }
        void resize(Size new_size) { depth.resize(new_size); }

        void build_depth()
        {
            build_depth(root_id());
        }
        void build_depth(NodeId root)
        {
            
            
            depth.resize(size());
            // depth[root] = 0;
            NodeId current = root;
            for(Size ctr = 0; ctr < size(); ++ctr)
            {
                NodeId next = next_preorder[current];
                if (next == current)
                {
                    break;
                }
                current = next;
                depth[current] = depth[up[current]]+1;
            }
            m_depth_valid = true;
        }

    };

} // namespace do_ast
