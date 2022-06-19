#pragma once

#include <vector>
#include <cstdint>
#include <cassert>
#include <type_traits>

#include <do_ast/type_value.h>

namespace do_ast {


    template<
        class TNode = TypeValue<>, 
        class TNodeId = int64_t, 
        class TDepth = uint32_t, 
        class TSize = int64_t
    >
    struct NodesPostorder
    {
        // store tree in post order (i.e. first arg1, then arg2, then node for node with two arguments arg1,arg2)

        // inspired by https://stackoverflow.com/a/28643465/798588 which suggests storing ordered by preorder for preorder recursion.
        // during construction by code the items are in a natural postorder.
        // by keeping track of up,down,prev,next and next_or_up pointer indices next_preorder indices can be computed for easy preorder traversal.
        // during evaluation postorder storage is optimal: evaluation requires postorder traversal, so that evaluated arguments exist for evaluation of the 'current' node.
        // preorder traversal is necessary for printing or more generally tree rewriting
        

        static_assert(std::is_integral<TNodeId>::value, "std::is_integral<TNodeId>::value");
        static_assert(std::is_integral<TDepth>::value, "std::is_integral<TDepth>::value");
        static_assert(std::is_integral<TSize>::value, "std::is_integral<TSize>::value");
        
        using Node = TNode;
        using NodeId = TNodeId;
        using Depth = TDepth;
        using Size = TSize;

        // minimum information necessary
        std::vector<Node> postorder;
        std::vector<Size> num_args;

        // pointers allowing traversal in any direction
        std::vector<NodeId> up;
        std::vector<NodeId> down;
        std::vector<NodeId> prev;
        std::vector<NodeId> next;

        // preorder related
        std::vector<Depth> depth;
        std::vector<NodeId> preorder;
        std::vector<NodeId> next_preorder;
        std::vector<NodeId> skip_preorder; 

        // std::vector<NodeId> next_or_up;
        
        void clear()
        {
            postorder.clear();
            num_args.clear();
            depth.clear();
            preorder.clear();
            up.clear();
            down.clear();
            prev.clear();
            next.clear();
            // next_or_up.clear();
            next_preorder.clear();
            skip_preorder.clear();
            m_next_preorder_valid = true;
            m_preorder_valid = true;
            m_depth_valid = true;

        }

        Node& operator[](NodeId id) { return postorder[id]; }
        const Node& operator[](NodeId id) const { return postorder[id]; }
        
        NodeId root_id() const { return size() - 1; }
        Node& root() { return postorder.back(); }

        NodeId insert()
        {
            NodeId i = postorder.size();
            postorder.emplace_back();
            num_args.emplace_back();
            return i;
        };
        
        NodeId insert(const Node& node, Size num_args = 0)
        {
            auto i = insert();
            this->postorder[i] = node;
            this->num_args[i] = num_args;
            return i;
        };
        
        Size size() const { return postorder.size(); }
        template<class... Args>
        NodeId add_node(const Node& node, Args... args)
        {
            NodeId i = insert(node, sizeof...(Args));
            up.push_back(i);
            down.push_back(i);
            prev.push_back(i);
            next.push_back(i);
            // next_or_up.push_back(i);
            depth.push_back(0);
            next_preorder.push_back(i);
            skip_preorder.push_back(i);
            
            set_up(i, args...);
            set_down(i, args...);
            set_prev(i, args...);
            set_next(i, args...);
            assert_postorder(i, args...);
            
            m_next_preorder_valid = false;
            m_preorder_valid = false;
            m_depth_valid = false;
            return i;
        }
        
        bool is_next_preorder_valid() const { return m_next_preorder_valid; }
        bool is_preorder_valid() const { return m_preorder_valid; }
        bool is_depth_valid() const { return m_depth_valid; }
        
        void build()
        {
            build_next_preorder();
            build_preorder();
            build_depth();
        }
        
        void build_next_preorder()
        {
            build_next_preorder(root_id());
        }
        void build_next_preorder(NodeId root)
        {
            if (is_preorder_valid()) return;
            next_preorder.resize(size());
            NodeId last = root;
            NodeId here = root;
            for(Size ctr = 0; ctr < size(); ++ctr)
            {
                bool has_next = (next[here] != here);
                if (has_next)
                {
                    skip_preorder[here] = next[here];
                }
                else
                {
                    skip_preorder[here] = skip_preorder[up[here]];
                }
                if (num_args[here] > 0)
                {
                    next_preorder[here] = down[here];
                    here = down[here];
                }
                else
                {
                    if(skip_preorder[here] == skip_preorder[root])
                    {
                        // reached root
                        // i.e. no further iteration
                        skip_preorder[here] = here; 
                        next_preorder[here] = here;
                        break;
                    }
                    else
                    {
                        next_preorder[here] = skip_preorder[here];
                        here = skip_preorder[here];
                    }
                }
            }
            m_next_preorder_valid = true;
        }
        void build_preorder()
        {
            build_preorder(root_id());
        }
        
        void build_preorder(NodeId root)
        {
            if (is_preorder_valid()) return;
            if (!is_next_preorder_valid()) build_next_preorder();
            
            preorder.clear();
            NodeId current = root;
            for(Size ctr = 0; ctr < size(); ++ctr)
            {
                preorder.push_back(current);
                NodeId next = next_preorder[current];
                if (next == current)
                {
                    break;
                }
                current = next;
            }
            assert(preorder.size() == size());
            m_preorder_valid = true;
        }
        
        void build_depth()
        {
            build_depth(root_id());
        }
        void build_depth(NodeId root)
        {
            if (is_depth_valid()) return;
            if (!is_next_preorder_valid()) build_next_preorder();
            
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

    protected:
        
        bool m_next_preorder_valid = true;
        bool m_preorder_valid = true;
        bool m_depth_valid = true;
        
        void set_up(NodeId parent)
        {}
        template<class Arg, class... Args>
        void set_up(NodeId parent, Arg arg, Args... more)
        {
            up[arg] = parent;
            set_up(parent, more...);
        }
        
        void set_down(NodeId parent)
        {}
        template<class Arg, class... Args>
        void set_down(NodeId parent, Arg arg, Args... more)
        {
            down[parent] = arg;
        }
        
        void set_prev(NodeId parent)
        {}
        template<class Arg>
        void set_prev(NodeId previous, Arg arg)
        {
            prev[arg] = previous;
        }
        template<class Arg, class... Args>
        void set_prev(NodeId previous, Arg arg,  Args... more)
        {
            prev[arg] = previous;
            set_prev(arg, more...);
        }
        
        void set_next(NodeId parent)
        {}
        template<class Arg>
        void set_next(NodeId parent, Arg arg)
        {
        }
        template<class Arg1, class Arg2, class... Args>
        void set_next(NodeId parent, Arg1 arg1,  Arg2 arg2, Args... more)
        {
            next[arg1] = arg2;
            set_next(parent, arg2, more...);
        }

        void assert_postorder(NodeId parent)
        {}
        template<class Arg>
        void assert_postorder(NodeId parent, Arg arg)
        {
            const bool args_postordered = (parent > arg);
            assert(args_postordered);
        }
        template<class Arg1, class Arg2, class... Args>
        void assert_postorder(NodeId parent, Arg1 arg1,  Arg2 arg2, Args... more)
        {
            const bool args_postordered = (arg1 < arg2);
            assert(args_postordered);
            assert_postorder(parent, arg2, more...);
        }
    };

} // namespace do_ast

