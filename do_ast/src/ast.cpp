
#include <do_ast/ast.h>

namespace do_ast {

    void Ast::clear()
    {
        m_expr_pool.clear();
        m_arg_expr_list_1_pool.clear();
        m_arg_expr_list_2_pool.clear();
        m_arg_expr_list_3_pool.clear();
        m_arg_expr_list_4_pool.clear();
        m_arg_value_voidptr_pool.clear();
        m_arg_value_bool_pool.clear();
        m_arg_value_int8_pool.clear();
        m_arg_value_uint8_pool.clear();
        m_arg_value_int16_pool.clear();
        m_arg_value_uint16_pool.clear();
        m_arg_value_int32_pool.clear();
        m_arg_value_uint32_pool.clear();
        m_arg_value_int64_pool.clear();
        m_arg_value_uint64_pool.clear();
        m_arg_value_float_pool.clear();
        m_arg_value_double_pool.clear();
        m_arg_value_string_pool.clear();
    }

    typename Ast::index_type Ast::create(
        uint32_t expr_type
    )
    {
        return m_expr_pool.emplace(expr_type, ArgTypes::NoArgs::value, 0);
    }


    typename Ast::index_type Ast::create_with_args(
        uint32_t expr_type, index_type arg1
    )
    {
        return m_expr_pool.emplace(expr_type, ArgTypes::WithArgs<1>::value, m_arg_expr_list_1_pool.insert({arg1}));
    }

    typename Ast::index_type Ast::create_with_args(
        uint32_t expr_type, index_type arg1, index_type arg2
    )
    {
        return m_expr_pool.emplace(expr_type, ArgTypes::WithArgs<2>::value, m_arg_expr_list_2_pool.insert({arg1, arg2}));
    }

    typename Ast::index_type Ast::create_with_args(
        uint32_t expr_type, index_type arg1, index_type arg2, index_type arg3
    )
    {
        return m_expr_pool.emplace(expr_type, ArgTypes::WithArgs<3>::value, m_arg_expr_list_3_pool.insert({arg1, arg2, arg3}));
    }

    typename Ast::index_type Ast::create_with_args(
        uint32_t expr_type, index_type arg1, index_type arg2, index_type arg3, index_type arg4
    )
    {
        return m_expr_pool.emplace(expr_type, ArgTypes::WithArgs<4>::value, m_arg_expr_list_4_pool.insert({arg1, arg2, arg3, arg4}));
    }


    typename Ast::index_type Ast::create_with_value(
        uint32_t expr_type
    )
    {
        return m_expr_pool.emplace(expr_type, ArgTypes::WithValue<void>::value, 0);
    }

    typename Ast::index_type Ast::create_with_value(
        uint32_t expr_type, void* value
    )
    {
        return m_expr_pool.emplace(expr_type, ArgTypes::WithValue<void*>::value, m_arg_value_voidptr_pool.emplace(value));
    }

    typename Ast::index_type Ast::create_with_value(
        uint32_t expr_type, int8_t value
    )
    {
        return m_expr_pool.emplace(expr_type, ArgTypes::WithValue<int8_t>::value, m_arg_value_int8_pool.emplace(value));
    }

    typename Ast::index_type Ast::create_with_value(
        uint32_t expr_type, bool value
    )
    {
        return m_expr_pool.emplace(expr_type, ArgTypes::WithValue<bool>::value, m_arg_value_bool_pool.emplace(value));
    }

    typename Ast::index_type Ast::create_with_value(
        uint32_t expr_type, uint8_t value
    )
    {
        return m_expr_pool.emplace(expr_type, ArgTypes::WithValue<uint8_t>::value, m_arg_value_uint8_pool.emplace(value));
    }

    typename Ast::index_type Ast::create_with_value(
        uint32_t expr_type, int16_t value
    )
    {
        return m_expr_pool.emplace(expr_type, ArgTypes::WithValue<int16_t>::value, m_arg_value_int16_pool.emplace(value));
    }

    typename Ast::index_type Ast::create_with_value(
        uint32_t expr_type, uint16_t value
    )
    {
        return m_expr_pool.emplace(expr_type, ArgTypes::WithValue<uint16_t>::value, m_arg_value_uint16_pool.emplace(value));
    }

    typename Ast::index_type Ast::create_with_value(
        uint32_t expr_type, int32_t value
    )
    {
        return m_expr_pool.emplace(expr_type, ArgTypes::WithValue<int32_t>::value, m_arg_value_int32_pool.emplace(value));
    }

    typename Ast::index_type Ast::create_with_value(
        uint32_t expr_type, uint32_t value
    )
    {
        return m_expr_pool.emplace(expr_type, ArgTypes::WithValue<uint32_t>::value, m_arg_value_uint32_pool.emplace(value));
    }

    typename Ast::index_type Ast::create_with_value(
        uint32_t expr_type, int64_t value
    )
    {
        return m_expr_pool.emplace(expr_type, ArgTypes::WithValue<int64_t>::value, m_arg_value_int64_pool.emplace(value));
    }

    typename Ast::index_type Ast::create_with_value(
        uint32_t expr_type, uint64_t value
    )
    {
        return m_expr_pool.emplace(expr_type, ArgTypes::WithValue<uint64_t>::value, m_arg_value_uint64_pool.emplace(value));
    }

    typename Ast::index_type Ast::create_with_value(
        uint32_t expr_type, float value
    )
    {
        return m_expr_pool.emplace(expr_type, ArgTypes::WithValue<float>::value, m_arg_value_float_pool.emplace(value));
    }

    typename Ast::index_type Ast::create_with_value(
        uint32_t expr_type, double value
    )
    {
        return m_expr_pool.emplace(expr_type, ArgTypes::WithValue<double>::value, m_arg_value_double_pool.emplace(value));
    }

    typename Ast::index_type Ast::create_with_value(
        uint32_t expr_type, std::string value
    )
    {
        return m_expr_pool.emplace(expr_type, ArgTypes::WithValue<std::string>::value, m_arg_value_string_pool.emplace(value));
    }


} // namespace do_ast
