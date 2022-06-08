#pragma once

#include <string>
#include <array>
#include <cstdint>
#include <type_traits>

#include <do_ast/auto_padding.h>
#include <do_ast/item_pool_tuple.h>

namespace do_ast {

    struct ValueUnion
    {
        enum class Type
        {
            Void = 0,
            String,
            VoidPtr,
            Bool,
            Int8,
            Uint8,
            Int16,
            Uint16,
            Int32,
            Uint32,
            Int64,
            Uint64,
            Float,
            Double
        };

        template<class SmallType, class BigType = std::string>
        using CountSmallInBig = std::integral_constant<
            uint32_t, 
            sizeof(BigType) / sizeof(SmallType)
        >;

        template<class SmallType, class BigType = std::string>
        using ArrayOfSmallType = std::array<SmallType, CountSmallInBig<SmallType, BigType>::value>;
        union
        {
            ArrayOfSmallType<void*   > as_void_ptr;
            ArrayOfSmallType<bool    > as_bool;
            ArrayOfSmallType<int8_t  > as_int8;
            ArrayOfSmallType<uint8_t > as_uint8;
            ArrayOfSmallType<int16_t > as_int16;
            ArrayOfSmallType<uint16_t> as_uint16;
            ArrayOfSmallType<int32_t > as_int32;
            ArrayOfSmallType<uint32_t> as_uint32;
            ArrayOfSmallType<int64_t > as_int64;
            ArrayOfSmallType<uint64_t> as_uint64;
            ArrayOfSmallType<float   > as_float;
            ArrayOfSmallType<double  > as_double;
            std::string as_string;
        };

        Type type = Type::Void;

        // uint8_t padding[12];

        ValueUnion()
        {
            // as_uint8.fill(0);
        }
        ValueUnion(Type type) : type(type) {}
        ValueUnion(const ValueUnion& other)
        {
            as_uint8 = other.as_uint8;
            type = other.type;
        }
        ValueUnion& operator=(const ValueUnion& other)
        {
            as_uint8 = other.as_uint8;
            type = other.type;
            return *this;
        }
        static ValueUnion Void    () { return ValueUnion(); }
        static ValueUnion VoidPtr (void*    value) { ValueUnion result(Type::VoidPtr); result.as_void_ptr[0] = value; return result;}
        static ValueUnion Bool    (bool     value) { ValueUnion result(Type::Bool);    result.as_bool[0] = value;     return result;}
        static ValueUnion Int8    (int8_t   value) { ValueUnion result(Type::Int8);    result.as_int8[0] = value;     return result;}
        static ValueUnion Uint8   (uint8_t  value) { ValueUnion result(Type::Uint8);   result.as_uint8[0] = value;    return result;}
        static ValueUnion Int16   (int16_t  value) { ValueUnion result(Type::Int16);   result.as_int16[0] = value;    return result;}
        static ValueUnion Uint16  (uint16_t value) { ValueUnion result(Type::Uint16);  result.as_uint16[0] = value;   return result;}
        static ValueUnion Int32   (int32_t  value) { ValueUnion result(Type::Int32);   result.as_int32[0] = value;    return result;}
        static ValueUnion Uint32  (uint32_t value) { ValueUnion result(Type::Uint32);  result.as_uint32[0] = value;   return result;}
        static ValueUnion Int64   (int64_t  value) { ValueUnion result(Type::Int64);   result.as_int64[0] = value;    return result;}
        static ValueUnion Uint64  (uint64_t value) { ValueUnion result(Type::Uint64);  result.as_uint64[0] = value;   return result;}
        static ValueUnion Float   (float    value) { ValueUnion result(Type::Float);   result.as_float[0] = value;    return result;}
        static ValueUnion Double  (double   value) { ValueUnion result(Type::Double);  result.as_double[0] = value;   return result;}
        static ValueUnion String  (const std::string& value) { ValueUnion result(Type::String); result.as_string = value; return result;}
        
        ~ValueUnion()
        {
            if (type == Type::String)
            {
                as_string.~basic_string();
            }
        }

    };

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


    template<class TTypeClass = uint32_t, class TRelations = Relations_<ItemPoolIndex, 4>, class TValue = ValueUnion>
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

} // namespace do_ast
