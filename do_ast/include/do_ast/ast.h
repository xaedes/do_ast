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

    template <class index_t>
    struct Expression
    {
        using index_type = index_t;

        index_type arg_id = 0;
        uint32_t expr_type = 0;
        uint32_t arg_type = 0;

        Expression() 
        {}

        Expression(uint32_t expr_type, uint32_t arg_type=0, index_type arg_id=0) 
        :arg_id(arg_id)
        ,expr_type(expr_type)
        ,arg_type(arg_type)
        {}
    };

    template<class index_t, uint32_t N>
    using ArgExpressionList = std::array<index_t, N>;

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
        using index_type = typename Pool<int>::index_type;


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
            void no_args(Ast& ast, index_type expr_idx, uint32_t expr_type) {} 
            void with_args(Ast& ast, index_type expr_idx, uint32_t expr_type, index_type arg1) {} 
            void with_args(Ast& ast, index_type expr_idx, uint32_t expr_type, index_type arg1, index_type arg2) {} 
            void with_args(Ast& ast, index_type expr_idx, uint32_t expr_type, index_type arg1, index_type arg2, index_type arg3) {} 
            void with_args(Ast& ast, index_type expr_idx, uint32_t expr_type, index_type arg1, index_type arg2, index_type arg3, index_type arg4) {} 
            void with_value(Ast& ast, index_type expr_idx, uint32_t expr_type) {} 
            void with_value(Ast& ast, index_type expr_idx, uint32_t expr_type, void* value) {} 
            void with_value(Ast& ast, index_type expr_idx, uint32_t expr_type, bool value) {} 
            void with_value(Ast& ast, index_type expr_idx, uint32_t expr_type, int8_t value) {} 
            void with_value(Ast& ast, index_type expr_idx, uint32_t expr_type, uint8_t value) {} 
            void with_value(Ast& ast, index_type expr_idx, uint32_t expr_type, int16_t value) {} 
            void with_value(Ast& ast, index_type expr_idx, uint32_t expr_type, uint16_t value) {} 
            void with_value(Ast& ast, index_type expr_idx, uint32_t expr_type, int32_t value) {} 
            void with_value(Ast& ast, index_type expr_idx, uint32_t expr_type, uint32_t value) {} 
            void with_value(Ast& ast, index_type expr_idx, uint32_t expr_type, int64_t value) {} 
            void with_value(Ast& ast, index_type expr_idx, uint32_t expr_type, uint64_t value) {} 
            void with_value(Ast& ast, index_type expr_idx, uint32_t expr_type, float value) {} 
            void with_value(Ast& ast, index_type expr_idx, uint32_t expr_type, double value) {} 
            void with_value(Ast& ast, index_type expr_idx, uint32_t expr_type, const std::string& value) {} 
        };

        template <class V>
        void visit(index_type expr_idx, V& visitor)
        {
            auto& expr = get(expr_idx);
            switch(expr.arg_type)
            {
                case ArgTypes::NoArgs::value: 
                {
                    visitor.no_args(*this, expr_idx, expr.expr_type); 
                    break;
                }
                case ArgTypes::WithArgs<1>::value: 
                {
                    auto args = m_arg_expr_list_1_pool[expr.arg_id]; 
                    visitor.with_args(*this, expr_idx, expr.expr_type, args[0]); 
                    break;
                }
                case ArgTypes::WithArgs<2>::value: 
                {
                    auto args = m_arg_expr_list_2_pool[expr.arg_id]; 
                    visitor.with_args(*this, expr_idx, expr.expr_type, args[0], args[1]); 
                    break;
                }
                case ArgTypes::WithArgs<3>::value: 
                {
                    auto args = m_arg_expr_list_3_pool[expr.arg_id]; 
                    visitor.with_args(*this, expr_idx, expr.expr_type, args[0], args[1], args[2]); 
                    break;
                }
                case ArgTypes::WithArgs<4>::value: 
                {
                    auto args = m_arg_expr_list_4_pool[expr.arg_id]; 
                    visitor.with_args(*this, expr_idx, expr.expr_type, args[0], args[1], args[2], args[3]); 
                    break;
                }
                case ArgTypes::WithValue<void>::value:
                {
                    visitor.with_value(*this, expr_idx, expr.expr_type); 
                    break;
                }
                case ArgTypes::WithValue<void*>::value:
                {
                    visitor.with_value(*this, expr_idx, expr.expr_type, m_arg_value_voidptr_pool[expr.arg_id].value); 
                    break;
                }
                case ArgTypes::WithValue<bool>::value:
                {
                    visitor.with_value(*this, expr_idx, expr.expr_type, m_arg_value_bool_pool[expr.arg_id].value); 
                    break;
                }
                case ArgTypes::WithValue<int8_t>::value:
                {
                    visitor.with_value(*this, expr_idx, expr.expr_type, m_arg_value_int8_pool[expr.arg_id].value); 
                    break;
                }
                case ArgTypes::WithValue<uint8_t>::value:
                {
                    visitor.with_value(*this, expr_idx, expr.expr_type, m_arg_value_uint8_pool[expr.arg_id].value); 
                    break;
                }
                case ArgTypes::WithValue<int16_t>::value:
                {
                    visitor.with_value(*this, expr_idx, expr.expr_type, m_arg_value_int16_pool[expr.arg_id].value); 
                    break;
                }
                case ArgTypes::WithValue<uint16_t>::value:
                {
                    visitor.with_value(*this, expr_idx, expr.expr_type, m_arg_value_uint16_pool[expr.arg_id].value); 
                    break;
                }
                case ArgTypes::WithValue<int32_t>::value:
                {
                    visitor.with_value(*this, expr_idx, expr.expr_type, m_arg_value_int32_pool[expr.arg_id].value); 
                    break;
                }
                case ArgTypes::WithValue<uint32_t>::value:
                {
                    visitor.with_value(*this, expr_idx, expr.expr_type, m_arg_value_uint32_pool[expr.arg_id].value); 
                    break;
                }
                case ArgTypes::WithValue<int64_t>::value:
                {
                    visitor.with_value(*this, expr_idx, expr.expr_type, m_arg_value_int64_pool[expr.arg_id].value); 
                    break;
                }
                case ArgTypes::WithValue<uint64_t>::value:
                {
                    visitor.with_value(*this, expr_idx, expr.expr_type, m_arg_value_uint64_pool[expr.arg_id].value); 
                    break;
                }
                case ArgTypes::WithValue<float>::value:
                {
                    visitor.with_value(*this, expr_idx, expr.expr_type, m_arg_value_float_pool[expr.arg_id].value); 
                    break;
                }
                case ArgTypes::WithValue<double>::value:
                {
                    visitor.with_value(*this, expr_idx, expr.expr_type, m_arg_value_double_pool[expr.arg_id].value); 
                    break;
                }
                case ArgTypes::WithValue<std::string>::value:
                {
                    visitor.with_value(*this, expr_idx, expr.expr_type, m_arg_value_string_pool[expr.arg_id].value); 
                    break;
                }
            }
        }

        Expression<index_type>& get(index_type expr_idx)
        {
            return m_expr_pool[expr_idx];
        }
        
        const Expression<index_type>& get(index_type expr_idx) const
        {
            return m_expr_pool[expr_idx];
        }


        index_type create(uint32_t expr_type);

        index_type create_with_args(uint32_t expr_type, index_type arg1);
        index_type create_with_args(uint32_t expr_type, index_type arg1, index_type arg2);
        index_type create_with_args(uint32_t expr_type, index_type arg1, index_type arg2, index_type arg3);
        index_type create_with_args(uint32_t expr_type, index_type arg1, index_type arg2, index_type arg3, index_type arg4);

        index_type create_with_value(uint32_t expr_type);
        index_type create_with_value(uint32_t expr_type, void* value);
        index_type create_with_value(uint32_t expr_type, bool value);
        index_type create_with_value(uint32_t expr_type, int8_t value);
        index_type create_with_value(uint32_t expr_type, uint8_t value);
        index_type create_with_value(uint32_t expr_type, int16_t value);
        index_type create_with_value(uint32_t expr_type, uint16_t value);
        index_type create_with_value(uint32_t expr_type, int32_t value);
        index_type create_with_value(uint32_t expr_type, uint32_t value);
        index_type create_with_value(uint32_t expr_type, int64_t value);
        index_type create_with_value(uint32_t expr_type, uint64_t value);
        index_type create_with_value(uint32_t expr_type, float value);
        index_type create_with_value(uint32_t expr_type, double value);
        index_type create_with_value(uint32_t expr_type, std::string value);

        void clear();
    protected:
        Pool<Expression<index_type>> m_expr_pool;

        Pool<ArgExpressionList<index_type, 1>> m_arg_expr_list_1_pool;
        Pool<ArgExpressionList<index_type, 2>> m_arg_expr_list_2_pool;
        Pool<ArgExpressionList<index_type, 3>> m_arg_expr_list_3_pool;
        Pool<ArgExpressionList<index_type, 4>> m_arg_expr_list_4_pool;

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
