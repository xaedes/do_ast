#pragma once

#include <iostream>
#include <cstdint>
#include <string>
#include <array>
#include <ios>
#include <iomanip>
#include <type_traits>
#include <do_ast/item_pool.h>

namespace do_ast {

    struct Expression
    {

        ItemPoolIndex arg_idx{};
        uint32_t expr_type = 0;
        uint32_t arg_type = 0;

        Expression() 
        {}

        Expression(uint32_t expr_type, uint32_t arg_type=0, ItemPoolIndex arg_idx = ItemPoolIndex()) 
        :arg_idx(arg_idx)
        ,expr_type(expr_type)
        ,arg_type(arg_type)
        {}
    };

    template<uint32_t N>
    using ArgExpressionList = std::array<ItemPoolIndex, N>;

    template<class T>
    struct ArgValue
    {
        T value;

        ArgValue() = default;
        ArgValue(T value) : value(value) {}
    };

    struct Ast
    {
        template<class T> using Pool = ItemPool<T>;


        struct ArgTypes
        {
            using NoArgs = std::integral_constant<uint32_t, 0>;
            template<uint32_t N> using WithArgs = std::integral_constant<uint32_t, N>;
            template<class T> struct WithValue : public std::integral_constant<uint32_t, 0x80000000> {};
            template<> struct WithValue<void*>       : public std::integral_constant<uint32_t, 0x80000000 + 1> {};
            template<> struct WithValue<bool>        : public std::integral_constant<uint32_t, 0x80000000 + 2> {};
            template<> struct WithValue<int8_t>      : public std::integral_constant<uint32_t, 0x80000000 + 3> {};
            template<> struct WithValue<uint8_t>     : public std::integral_constant<uint32_t, 0x80000000 + 4> {};
            template<> struct WithValue<int16_t>     : public std::integral_constant<uint32_t, 0x80000000 + 5> {};
            template<> struct WithValue<uint16_t>    : public std::integral_constant<uint32_t, 0x80000000 + 6> {};
            template<> struct WithValue<int32_t>     : public std::integral_constant<uint32_t, 0x80000000 + 7> {};
            template<> struct WithValue<uint32_t>    : public std::integral_constant<uint32_t, 0x80000000 + 8> {};
            template<> struct WithValue<int64_t>     : public std::integral_constant<uint32_t, 0x80000000 + 9> {};
            template<> struct WithValue<uint64_t>    : public std::integral_constant<uint32_t, 0x80000000 + 10> {};
            template<> struct WithValue<float>       : public std::integral_constant<uint32_t, 0x80000000 + 11> {};
            template<> struct WithValue<double>      : public std::integral_constant<uint32_t, 0x80000000 + 12> {};
            template<> struct WithValue<std::string> : public std::integral_constant<uint32_t, 0x80000000 + 13> {};
        };

        struct Visitor
        {
            void nil(Ast& ast, ItemPoolIndex expr_idx) {} 
            void no_args(Ast& ast, ItemPoolIndex expr_idx, uint32_t expr_type) {} 
            void with_args(Ast& ast, ItemPoolIndex expr_idx, uint32_t expr_type, ItemPoolIndex arg1) {} 
            void with_args(Ast& ast, ItemPoolIndex expr_idx, uint32_t expr_type, ItemPoolIndex arg1, ItemPoolIndex arg2) {} 
            void with_args(Ast& ast, ItemPoolIndex expr_idx, uint32_t expr_type, ItemPoolIndex arg1, ItemPoolIndex arg2, ItemPoolIndex arg3) {} 
            void with_args(Ast& ast, ItemPoolIndex expr_idx, uint32_t expr_type, ItemPoolIndex arg1, ItemPoolIndex arg2, ItemPoolIndex arg3, ItemPoolIndex arg4) {} 
            void with_value(Ast& ast, ItemPoolIndex expr_idx, uint32_t expr_type) {} 
            void with_value(Ast& ast, ItemPoolIndex expr_idx, uint32_t expr_type, void* value) {} 
            void with_value(Ast& ast, ItemPoolIndex expr_idx, uint32_t expr_type, bool value) {} 
            void with_value(Ast& ast, ItemPoolIndex expr_idx, uint32_t expr_type, int8_t value) {} 
            void with_value(Ast& ast, ItemPoolIndex expr_idx, uint32_t expr_type, uint8_t value) {} 
            void with_value(Ast& ast, ItemPoolIndex expr_idx, uint32_t expr_type, int16_t value) {} 
            void with_value(Ast& ast, ItemPoolIndex expr_idx, uint32_t expr_type, uint16_t value) {} 
            void with_value(Ast& ast, ItemPoolIndex expr_idx, uint32_t expr_type, int32_t value) {} 
            void with_value(Ast& ast, ItemPoolIndex expr_idx, uint32_t expr_type, uint32_t value) {} 
            void with_value(Ast& ast, ItemPoolIndex expr_idx, uint32_t expr_type, int64_t value) {} 
            void with_value(Ast& ast, ItemPoolIndex expr_idx, uint32_t expr_type, uint64_t value) {} 
            void with_value(Ast& ast, ItemPoolIndex expr_idx, uint32_t expr_type, float value) {} 
            void with_value(Ast& ast, ItemPoolIndex expr_idx, uint32_t expr_type, double value) {} 
            void with_value(Ast& ast, ItemPoolIndex expr_idx, uint32_t expr_type, const std::string& value) {} 
        };

        template <class V = Visitor> void visit(V& visitor, ItemPoolIndex expr_idx);

        Expression* get(ItemPoolIndex expr_idx)
        {
            return m_expr_pool.contains(expr_idx) ? &m_expr_pool.get(expr_idx) : nullptr;
        }
        
        const Expression* get(ItemPoolIndex expr_idx) const
        {
            return m_expr_pool.contains(expr_idx) ? &m_expr_pool.get(expr_idx) : nullptr;
        }

        ItemPoolIndex create(uint32_t expr_type);

        ItemPoolIndex create_with_args(uint32_t expr_type, ItemPoolIndex arg1);
        ItemPoolIndex create_with_args(uint32_t expr_type, ItemPoolIndex arg1, ItemPoolIndex arg2);
        ItemPoolIndex create_with_args(uint32_t expr_type, ItemPoolIndex arg1, ItemPoolIndex arg2, ItemPoolIndex arg3);
        ItemPoolIndex create_with_args(uint32_t expr_type, ItemPoolIndex arg1, ItemPoolIndex arg2, ItemPoolIndex arg3, ItemPoolIndex arg4);

        ItemPoolIndex create_with_value(uint32_t expr_type);
        ItemPoolIndex create_with_value(uint32_t expr_type, void* value);
        ItemPoolIndex create_with_value(uint32_t expr_type, bool value);
        ItemPoolIndex create_with_value(uint32_t expr_type, int8_t value);
        ItemPoolIndex create_with_value(uint32_t expr_type, uint8_t value);
        ItemPoolIndex create_with_value(uint32_t expr_type, int16_t value);
        ItemPoolIndex create_with_value(uint32_t expr_type, uint16_t value);
        ItemPoolIndex create_with_value(uint32_t expr_type, int32_t value);
        ItemPoolIndex create_with_value(uint32_t expr_type, uint32_t value);
        ItemPoolIndex create_with_value(uint32_t expr_type, int64_t value);
        ItemPoolIndex create_with_value(uint32_t expr_type, uint64_t value);
        ItemPoolIndex create_with_value(uint32_t expr_type, float value);
        ItemPoolIndex create_with_value(uint32_t expr_type, double value);
        ItemPoolIndex create_with_value(uint32_t expr_type, std::string value);

        void erase_expr(ItemPoolIndex expr_idx);
        void erase_expr_recursive(ItemPoolIndex expr_idx);

        void clear();
    protected:
        void erase_arg(uint32_t arg_type, ItemPoolIndex arg_idx);

        Pool<Expression> m_expr_pool;

        Pool<ArgExpressionList<1>> m_arg_expr_list_1_pool;
        Pool<ArgExpressionList<2>> m_arg_expr_list_2_pool;
        Pool<ArgExpressionList<3>> m_arg_expr_list_3_pool;
        Pool<ArgExpressionList<4>> m_arg_expr_list_4_pool;

        Pool<ArgValue<void*>> m_arg_value_voidptr_pool;
        Pool<ArgValue<bool>> m_arg_value_bool_pool;
        Pool<ArgValue<int8_t>> m_arg_value_int8_pool;
        Pool<ArgValue<uint8_t>> m_arg_value_uint8_pool;
        Pool<ArgValue<int16_t>> m_arg_value_int16_pool;
        Pool<ArgValue<uint16_t>> m_arg_value_uint16_pool;
        Pool<ArgValue<int32_t>> m_arg_value_int32_pool;
        Pool<ArgValue<uint32_t>> m_arg_value_uint32_pool;
        Pool<ArgValue<int64_t>> m_arg_value_int64_pool;
        Pool<ArgValue<uint64_t>> m_arg_value_uint64_pool;
        Pool<ArgValue<float>> m_arg_value_float_pool;
        Pool<ArgValue<double>> m_arg_value_double_pool;
        Pool<ArgValue<std::string>> m_arg_value_string_pool;
    };


} // namespace do_ast

#include <do_ast/ast.impl.h>
