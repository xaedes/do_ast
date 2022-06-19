#pragma once

#include <string>
#include <array>
#include <cstdint>
#include <type_traits>

#include <do_ast/v2_value_union.h>
#include <do_ast/item_pool_tuple.h>

namespace do_ast {
namespace v2 {


    // struct ItemPoolIndex
    // {
    //     std::size_t index = 0;
    //     uint32_t smc = 0; // sequential modification counter
    // };

    template<class TIndex, uint32_t TCount, class TNumArgs = uint32_t>
    struct Relations_
    {
        using Index = TIndex;
        using Count = std::integral_constant<uint32_t, TCount>;
        using SizeOfPayload = std::integral_constant<uint32_t, TCount * sizeof(Index) + sizeof(TNumArgs)>;
        using CountPadding = std::integral_constant<uint32_t, SizeOfPayload::value % 16>;

        std::array<Index, TCount> args;
        TNumArgs num_args = 0;
        AutoPadding<SizeOfPayload::value, 16> _padding;

        Relations_() = default;

        template<class Arg, class... Args>
        Relations_(Arg arg, Args... more) 
        { 
            set(arg, more...); 
        }

        template<class... Args>
        void set(Args... args)
        {
            static_assert((sizeof...(Args) <= TCount), "Too many arguments. Supports at most TCount arguments.");
            set_<0>(args...);
        }

    protected:
        template<uint32_t Idx=0, class Arg, std::enable_if_t<(Idx < TCount), bool> = true>
        void set_(Arg arg)
        {
            args[Idx] = arg;
            num_args = Idx+1;
        }
        template<uint32_t Idx=0, class Arg, class Arg1, class... Args, std::enable_if_t<(Idx < TCount-1), bool> = true>
        void set_(Arg arg, Arg1 next, Args... more)
        {
            args[Idx] = arg;
            set_<Idx+1>(next, more...);
        }
    };


    template<class TTypeClass = uint32_t, class TRelations = Relations_<ItemPoolIndex, 4>, class TValue = ValueUnion<sizeof(double)>>
    struct Expressions
    {
        using TypeClass = TTypeClass;
        using Relations = TRelations;
        using Value = TValue;

        static_assert(std::is_copy_assignable<TypeClass>::value, "std::is_copy_assignable<TypeClass>");
        static_assert(std::is_copy_assignable<Value>::value, "std::is_copy_assignable<Value>");
        static_assert(std::is_copy_assignable<Relations>::value, "std::is_copy_assignable<Relations>");
        static_assert((sizeof(Relations) % 16 == 0),"sizeof(Relations) % 16 == 0");
        // static_assert(sizeof(Value) % 16 == 0, "sizeof(Value) % 16 == 0");
        //ItemPoolTuple<TypeClass, Value> pool;
        ItemPoolTuple<TypeClass, Relations, Value> pool;

        using Expression = ItemPoolIndex;

        Expression insert(TypeClass type, Relations rel=Relations(), Value val = Value::Void()) 
        { 
            return pool.insert(type, rel, val); 
        }

        template<class Callback>
        void traverse_pre_order(Expression expr, Callback cb)
        {
            const auto* types     = pool.slots<0>().data();
            const auto* relations = pool.slots<1>().data();
            const auto* values    = pool.slots<2>().data();

            struct StackItem
            {
                Expression expr;
                int depth;
            };

            std::vector<StackItem> stack;
            stack.push_back({expr,0});
            while (!stack.empty())
            {
                auto item = stack.back();
                stack.pop_back();

                auto idx = item.expr.index;
                const auto& rel = relations[item.expr.index];
                cb(item.depth, item.expr, types[idx], rel, values[idx]);
                for (uint32_t k = 0; k<rel.num_args; ++k)
                {
                    auto arg = rel.args[rel.num_args-1-k];
                    if (pool.contains(arg))
                    {
                        stack.push_back({arg, item.depth+1});
                    }
                }
            }

        }

        template<class Callback>
        void traverse_post_order(Expression expr, Callback cb)
        {
            const auto* types     = pool.slots<0>().data();
            const auto* relations = pool.slots<1>().data();
            const auto* values    = pool.slots<2>().data();

            struct StackItem
            {
                Expression expr;
                int depth;
                bool done = false;
                uint64_t _padding;

                StackItem() = default;
                StackItem(Expression expr, int depth) 
                : expr(expr), depth(depth) {}
            };
            // std::cout << "sizeof(StackItem) " << sizeof(StackItem) << "\n";

            static std::vector<StackItem> stack;
            stack.clear();
            stack.reserve(1024);
            stack.push_back({expr,0});
            while (!stack.empty())
            {
                auto idx_item = stack.size()-1;
                auto& item = stack.back();

                auto idx = item.expr.index;
                const auto& rel = relations[item.expr.index];

                if (item.done || (rel.num_args == 0))
                {
                    cb(item.depth, item.expr, types[idx], rel, values[idx]);
                    stack.pop_back();
                }
                else
                {
                    auto new_depth = item.depth + 1;
                    for (uint32_t k = 0; k<rel.num_args; ++k)
                    {
                        auto arg = rel.args[rel.num_args-1-k];
                        if (pool.contains(arg))
                        {
                            // stack.push_back({arg, new_depth});
                            stack.emplace_back(arg, new_depth);
                        }
                    }
                    // auto&item is invalidated after stack.push_back
                    stack[idx_item].done = true;
                }
            }
        }

        template<class CallbackPre, class CallbackIn, class CallbackPost>
        void traverse_in_order(
            Expression expr, 
            CallbackPre cbPreOrder, 
            CallbackIn cbInOrder, 
            CallbackPost cbPostOrder,
            bool enable_pre_order = true,
            bool enable_in_order = true,
            bool enable_post_order = true
        )
        {
            const auto* types     = pool.slots<0>().data();
            const auto* relations = pool.slots<1>().data();
            const auto* values    = pool.slots<2>().data();

            decltype(types) null_types = nullptr;
            decltype(relations) null_relations = nullptr;
            decltype(values) null_values = nullptr;

            struct StackFrame
            {
                Expression expr;
                int depth = 0;
                int step = 0;
                

                // bool started = false;
                int processed_args = 0;
                int num_args = 0;
            };

            std::vector<StackFrame> stack;
            stack.push_back({expr,0});
            while (!stack.empty())
            {
                auto idx_item = stack.size()-1;
                auto& item = stack.back();
                
                if (!pool.contains(item.expr))
                {
                    if (enable_pre_order) cbPreOrder(item.depth, item.expr, null_types, null_relations, null_values);
                    if (enable_in_order) cbInOrder(item.depth, item.expr, null_types, null_relations, null_values);
                    if (enable_post_order) cbPostOrder(item.depth, item.expr, null_types, null_relations, null_values);
                    stack.pop_back();
                    continue;
                }

                auto new_depth = item.depth + 1;

                auto idx = item.expr.index;
                const auto& rel = relations[item.expr.index];

                if (item.step == 0)
                {
                    item.step = 1;

                    if (enable_pre_order) cbPreOrder(item.depth, item.expr, &types[idx], &rel, &values[idx]);

                    if (rel.num_args == 0)
                    {
                        if (enable_in_order) cbInOrder(item.depth, item.expr, &types[idx], &rel, &values[idx]);
                        if (enable_post_order) cbPostOrder(item.depth, item.expr, &types[idx], &rel, &values[idx]);
                        stack.pop_back();
                        continue;
                    }

                    item.processed_args = 1;
                    item.num_args = rel.num_args;

                    auto arg = rel.args[0];
                    stack.push_back({arg, new_depth});
                    continue;
                }
                if (item.step == 1)
                {
                    if ((item.processed_args < item.num_args) && (enable_in_order))
                    {
                        cbInOrder(item.depth, item.expr, &types[idx], &rel, &values[idx]);
                    }
                    if (item.processed_args >= item.num_args)
                    {
                        if (enable_post_order) cbPostOrder(item.depth, item.expr, &types[idx], &rel, &values[idx]);
                        stack.pop_back();
                        continue;
                    }

                    auto arg = rel.args[item.processed_args];
                    ++item.processed_args;
                    stack.push_back({arg, new_depth});
                    continue;
                }
            }

        }

    };

} // namespace v2
} // namespace do_ast
