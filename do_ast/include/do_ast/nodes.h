#pragma once

#include <type_traits>
#include <cstdint>

#include <do_ast/nodes_neighbors.h>
#include <do_ast/nodes_depth.h>
#include <do_ast/nodes_postorder.h>
#include <do_ast/type_value.h>
// #include <do_ast/nodes_preorder.h>

namespace do_ast {

    enum class NodesOrder
    {
        Unordered = 0,
        Preorder,
        Postorder
    };


    template<
        class TNode = TypeValue<>, 
        class TNodeId = int64_t, 
        class TDepth = uint32_t, 
        class TSize = int64_t,
        NodesOrder TNodesOrder
    >
    struct Nodes
    {
        using nodes_order = std::integral_constant<NodesOrder, TNodesOrder>;
        using Node = TNode;
        using NodeId = TNodeId;
        using Depth = TDepth;
        using Size = TSize;
        using NodesContainer = NodesPostorder<Node,NodeId,Depth,Size>;
        // using NodesContainer = std::conditional_t<
            // nodes_order::value == NodesOrder::Postorder,
            // NodesPostorder<Node,NodeId,Depth,Size>,
            // NodesPostorder<Node,NodeId,Depth,Size>,
        // >;

        void clear()
        {
            nodes.clear();
            neighbors.clear();
        }
        NodeId insert() { return nodes.insert(); }

        Node& operator[](NodeId id) { return nodes[id]; }
        const Node& operator[](NodeId id) const { return nodes[id]; }
        
        NodeId root_id() const { return nodes.root_id(); }
        Node& root() { return nodes.root(); }

        NodeId neighbor_up(NodeId n) const { return neighbors.up[n]; }
        NodeId neighbor_down(NodeId n) const { return neighbors.down[n]; }
        NodeId neighbor_prev(NodeId n) const { return neighbors.prev[n]; }
        NodeId neighbor_next(NodeId n) const { return neighbors.next[n]; }

    protected:
        NodesContainer nodes;
        NodesNeighbors neighbors;
    };

} // namespace do_ast
