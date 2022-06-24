#pragma once

#include <vector>
#include <cstdint>
#include <type_traits>

namespace do_ast {

    template <
        class TNodeId = int64_t, 
        class TSize = int64_t
    >
    struct NodesNeighbors
    {
        static_assert(std::is_integral<TNodeId>::value, "std::is_integral<TNodeId>::value");
        static_assert(std::is_integral<TSize>::value, "std::is_integral<TSize>::value");

        using NodeId = TNodeId;
        using Size = TSize;

        // pointers allowing traversal in any direction
        std::vector<NodeId> up;   // parent
        std::vector<NodeId> down; // first argument 
        std::vector<NodeId> prev; // previous argument or parent
        std::vector<NodeId> next; // next argument

        void clear()
        {
            up.clear();
            down.clear();
            prev.clear();
            next.clear();
        }

        Size size() const { return up.size(); }
        void resize(Size new_size) 
        { 
            up.resize(new_size); 
            down.resize(new_size); 
            prev.resize(new_size); 
            next.resize(new_size); 
        }

        template<class... Args>
        void set_arguments(NodeId node, Args... args)
        {
            set_up(node, args...);
            set_down(node, args...);
            set_prev(node, args...);
            set_next(node, args...);
        }

        // template<class... Args>
        // void set_arguments(NodeId node, const NodeId* args, Size num_args)
        // {
        //     set_up(node, args...);
        //     set_down(node, args...);
        //     set_prev(node, args...);
        //     set_next(node, args...);
        // }

    protected:
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

    };

} // namespace do_ast
