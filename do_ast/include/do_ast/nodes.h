#pragma once

#include <type_traits>
#include <vector>

#include <do_ast/visit_struct/visit_struct.h>
#include <do_ast/arg_template_helpers.h>

namespace do_ast {

} // namespace do_ast


#define DO_AST_FORWARD_FIRST_RETURN(return_type, fname, the_function)  \
    return_type fname()                                         \
    {                                                           \
        return visit_struct::get<0>(*this).##the_function();    \
    }

#define DO_AST_FORWARD_CALL(fname, the_function)      \
    void fname()                               \
    {                                          \
        visit_struct::for_each(                \
            *this,                             \
            [](const char* name, auto& value)  \
            {                                  \
                value.##the_function();        \
            }                                  \
        );                                     \
    }

#define DO_AST_FORWARD_CALL_PRE(fname, the_function, pre_f)  \
    void fname()                                      \
    {                                                 \
        pre_f                                         \
        visit_struct::for_each(                       \
            *this,                                    \
            [](const char* name, auto& value)         \
            {                                         \
                value.##the_function();               \
            }                                         \
        );                                            \
    }

#define DO_AST_FORWARD_CALL_TEMPLATED_ARGS(fname, the_function)                    \
    template<class... ForwardedArgs>                                        \
    void fname(ForwardedArgs&&... args)                                     \
    {                                                                       \
        visit_struct::for_each(                                             \
            *this,                                                          \
            [&args...](const char* name, auto& value)                       \
            {                                                               \
                value.##the_function(std::forward<ForwardedArgs>(args)...); \
            }                                                               \
        );                                                                  \
    }

#define DO_AST_FORWARD_CALL_TEMPLATED_ARGS_PRE(fname, the_function, pre_f)         \
    template<class... ForwardedArgs>                                        \
    void fname(ForwardedArgs&&... args)                                     \
    {                                                                       \
        pre_f                                                               \
        visit_struct::for_each(                                             \
            *this,                                                          \
            [&args...](const char* name, auto& value)                       \
            {                                                               \
                value.##the_function(std::forward<ForwardedArgs>(args)...); \
            }                                                               \
        );                                                                  \
    }

#define DO_AST_RETURN_IF_NOT_ENABLED() if (!enabled) return;
#define DO_AST_RETURN_INVALID_INDEX_IF_NOT_ENABLED() if (!enabled) return InvalidIndex();



#define DO_AST_CONTAINER_FORWARDS()                                                \
    DO_AST_FORWARD_FIRST_RETURN(Size, size, size);                                 \
    DO_AST_FORWARD_FIRST_RETURN(Size, capacity, capacity);                         \
    DO_AST_FORWARD_CALL_PRE(clear, clear, DO_AST_RETURN_IF_NOT_ENABLED());                  \
    DO_AST_FORWARD_CALL_PRE(emplace_back, emplace_back, DO_AST_RETURN_IF_NOT_ENABLED());    \
    DO_AST_FORWARD_CALL_TEMPLATED_ARGS_PRE(resize, resize, DO_AST_RETURN_IF_NOT_ENABLED()); 

#define DO_AST_CONTAINER(default_enabled)  \
    DO_AST_CONTAINER_FORWARDS()            \
    bool enabled = default_enabled; \
    Version shape_version = 0;      \
    Version content_version = 0;

#define DO_AST_CONTAINER_DEFAULT() DO_AST_CONTAINER(true)
#define DO_AST_CONTAINER_OPTIONAL() DO_AST_CONTAINER(false)


template<
    class SetUp,
    class SetDown,
    class SetPrev,
    class SetNext,
    class Index,
    class... Args
>
void set_children_from_args(
    SetUp up, SetDown down, SetPrev prev, SetNext next, 
    Index invalid_index,
    Index parent,
    Args... children
)
{
    if (sizeof...(children) == 0)
    {
        down(parent, invalid_index);
    }
    do_ast::visit_n_args<1>([&down, parent](auto item){
        down(parent, item);
    }, children...);
    do_ast::visit_args([&up, parent](auto item){
        up(item, parent);
    }, children...);
    do_ast::foldl_args([&prev,next](auto lhs, auto rhs){
        prev(rhs, lhs);
        next(lhs, rhs);
    }, children...);
    up(parent, invalid_index);
    next(parent, invalid_index);
    prev(parent, invalid_index);
}

template <
    class TNode,
    class TIndex = std::size_t,
    class TSize = std::size_t,
    class TVersion = std::size_t,
    template<class...> class TContainer = std::vector
>
struct Nodes_
{
    using Node       = TNode;
    using Index      = TIndex;
    using Size       = TSize;
    using Version    = TVersion;
    template<class...Args> using Container  = TContainer<Args...>;

    static Index InvalidIndex() { return std::numeric_limits<Index>::max(); }
    
    struct Items
    {
        DO_AST_CONTAINER_DEFAULT()
        Container<Node> items;
    };

    struct Neighbors
    {
        DO_AST_CONTAINER_DEFAULT()
        Container<Index> up;
        Container<Index> down;
        Container<Index> prev;
        Container<Index> next;

        template<class... Args>
        void set_children(Index parent, Args... children)
        {
            DO_AST_RETURN_IF_NOT_ENABLED()
            ::set_children_from_args(
                [this](auto idx, auto val){ up[idx] = val; },
                [this](auto idx, auto val){ down[idx] = val; },
                [this](auto idx, auto val){ prev[idx] = val; },
                [this](auto idx, auto val){ next[idx] = val; },
                InvalidIndex(),
                parent,
                children...
            );
        }

    };

    struct NeighborsPacked
    {
        DO_AST_CONTAINER_OPTIONAL()
        struct Neighbors
        {
            Index up;
            Index down;
            Index prev;
            Index next;
        };
        Container<Neighbors> neighbors;

        template<class... Args>
        void set_children(Index parent, Args... children)
        {
            DO_AST_RETURN_IF_NOT_ENABLED()
            ::set_children_from_args(
                [this](auto idx, auto val){ neighbors[idx].up = val; },
                [this](auto idx, auto val){ neighbors[idx].down = val; },
                [this](auto idx, auto val){ neighbors[idx].prev = val; },
                [this](auto idx, auto val){ neighbors[idx].next = val; },
                InvalidIndex(),
                parent,
                children...
            );
        }

    };

    struct TraversePreorder
    {
        DO_AST_CONTAINER_OPTIONAL()
        Container<Index> next;
        Container<Index> skip;
    };

    struct TraversePostorder
    {
        DO_AST_CONTAINER_OPTIONAL()
        Container<Index> next;
    };

    struct SortedPreorder
    {
        DO_AST_CONTAINER_OPTIONAL()
        Container<Index> indices;
    };

    struct SortedPostorder
    {
        DO_AST_CONTAINER_OPTIONAL()
        Container<Index> indices;
    };

    struct Depth
    {
        DO_AST_CONTAINER_OPTIONAL()
        Container<Size> depth;
    };

    struct NumChildren
    {
        DO_AST_CONTAINER_OPTIONAL()
        Container<Size> num_children;
        template<class... Args>
        void set_children(Index parent, Args... children)
        {
            DO_AST_RETURN_IF_NOT_ENABLED()
            num_children[parent] = sizeof...(Args);
        }        
    };

    DO_AST_CONTAINER_DEFAULT()

    Items             items;
    Neighbors         neighbors;
    NeighborsPacked   neighbors_packed;
    TraversePreorder  traverse_preorder;
    TraversePostorder traverse_postorder;
    SortedPreorder    sorted_preorder;
    SortedPostorder   sorted_postorder;
    Depth             depth;
    NumChildren       num_children;

    Index insert()
    {
        DO_AST_RETURN_INVALID_INDEX_IF_NOT_ENABLED()
        Index new_id = size();
        emplace_back();
        return new_id;
    }

    Index insert(const Node& node)
    {
        DO_AST_RETURN_INVALID_INDEX_IF_NOT_ENABLED()
        Index new_id = insert();
        if (items.enabled)
        {
            items.items[new_id] = node;
        }
        return new_id;
    }

    template<class... Args>
    void set_children(Index parent, Args... children)
    {
        DO_AST_RETURN_IF_NOT_ENABLED()
        neighbors.set_children(parent, children...);
        neighbors_packed.set_children(parent, children...);
        num_children.set_children(parent, children...);
    }
};

#define DO_AST_REGISTER_NODES(TheNodesType)                                                                                                                               \
VISITABLE_STRUCT(TheNodesType, items, neighbors, neighbors_packed, traverse_preorder, traverse_postorder, sorted_preorder, sorted_postorder, depth, num_children); \
VISITABLE_STRUCT(TheNodesType::Items,             items);                                                                                                          \
VISITABLE_STRUCT(TheNodesType::Neighbors,         up, down, prev, next);                                                                                           \
VISITABLE_STRUCT(TheNodesType::NeighborsPacked,   neighbors);                                                                                                      \
VISITABLE_STRUCT(TheNodesType::TraversePreorder,  next, skip);                                                                                                     \
VISITABLE_STRUCT(TheNodesType::TraversePostorder, next);                                                                                                           \
VISITABLE_STRUCT(TheNodesType::SortedPreorder,    indices);                                                                                                        \
VISITABLE_STRUCT(TheNodesType::SortedPostorder,   indices);                                                                                                        \
VISITABLE_STRUCT(TheNodesType::Depth,             depth);                                                                                                          \
VISITABLE_STRUCT(TheNodesType::NumChildren,       num_children);                                                                                                   