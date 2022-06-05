#pragma once

#include <do_ast/ast.h>

namespace do_ast {

    template <class V>
    void Ast::visit(ItemPoolIndex expr_idx, V& visitor)
    {
        auto* expr = get(expr_idx);
        if (expr == nullptr)
        {
            visitor.nil(*this, expr_idx);
        }
        else
        switch(expr->arg_type)
        {
            case ArgTypes::NoArgs::value: 
            {
                visitor.no_args(*this, expr_idx, expr->expr_type); 
                break;
            }
            case ArgTypes::WithArgs<1>::value: 
            {
                auto args = m_arg_expr_list_1_pool.get(expr->arg_idx); 
                visitor.with_args(*this, expr_idx, expr->expr_type, args[0]); 
                break;
            }
            case ArgTypes::WithArgs<2>::value: 
            {
                auto args = m_arg_expr_list_2_pool.get(expr->arg_idx); 
                visitor.with_args(*this, expr_idx, expr->expr_type, args[0], args[1]); 
                break;
            }
            case ArgTypes::WithArgs<3>::value: 
            {
                auto args = m_arg_expr_list_3_pool.get(expr->arg_idx); 
                visitor.with_args(*this, expr_idx, expr->expr_type, args[0], args[1], args[2]); 
                break;
            }
            case ArgTypes::WithArgs<4>::value: 
            {
                auto args = m_arg_expr_list_4_pool.get(expr->arg_idx); 
                visitor.with_args(*this, expr_idx, expr->expr_type, args[0], args[1], args[2], args[3]); 
                break;
            }
            case ArgTypes::WithValue<void>::value:
            {
                visitor.with_value(*this, expr_idx, expr->expr_type); 
                break;
            }
            case ArgTypes::WithValue<void*>::value:
            {
                visitor.with_value(*this, expr_idx, expr->expr_type, m_arg_value_voidptr_pool.get(expr->arg_idx).value); 
                break;
            }
            case ArgTypes::WithValue<bool>::value:
            {
                visitor.with_value(*this, expr_idx, expr->expr_type, m_arg_value_bool_pool.get(expr->arg_idx).value); 
                break;
            }
            case ArgTypes::WithValue<int8_t>::value:
            {
                visitor.with_value(*this, expr_idx, expr->expr_type, m_arg_value_int8_pool.get(expr->arg_idx).value); 
                break;
            }
            case ArgTypes::WithValue<uint8_t>::value:
            {
                visitor.with_value(*this, expr_idx, expr->expr_type, m_arg_value_uint8_pool.get(expr->arg_idx).value); 
                break;
            }
            case ArgTypes::WithValue<int16_t>::value:
            {
                visitor.with_value(*this, expr_idx, expr->expr_type, m_arg_value_int16_pool.get(expr->arg_idx).value); 
                break;
            }
            case ArgTypes::WithValue<uint16_t>::value:
            {
                visitor.with_value(*this, expr_idx, expr->expr_type, m_arg_value_uint16_pool.get(expr->arg_idx).value); 
                break;
            }
            case ArgTypes::WithValue<int32_t>::value:
            {
                visitor.with_value(*this, expr_idx, expr->expr_type, m_arg_value_int32_pool.get(expr->arg_idx).value); 
                break;
            }
            case ArgTypes::WithValue<uint32_t>::value:
            {
                visitor.with_value(*this, expr_idx, expr->expr_type, m_arg_value_uint32_pool.get(expr->arg_idx).value); 
                break;
            }
            case ArgTypes::WithValue<int64_t>::value:
            {
                visitor.with_value(*this, expr_idx, expr->expr_type, m_arg_value_int64_pool.get(expr->arg_idx).value); 
                break;
            }
            case ArgTypes::WithValue<uint64_t>::value:
            {
                visitor.with_value(*this, expr_idx, expr->expr_type, m_arg_value_uint64_pool.get(expr->arg_idx).value); 
                break;
            }
            case ArgTypes::WithValue<float>::value:
            {
                visitor.with_value(*this, expr_idx, expr->expr_type, m_arg_value_float_pool.get(expr->arg_idx).value); 
                break;
            }
            case ArgTypes::WithValue<double>::value:
            {
                visitor.with_value(*this, expr_idx, expr->expr_type, m_arg_value_double_pool.get(expr->arg_idx).value); 
                break;
            }
            case ArgTypes::WithValue<std::string>::value:
            {
                visitor.with_value(*this, expr_idx, expr->expr_type, m_arg_value_string_pool.get(expr->arg_idx).value); 
                break;
            }
        }
    }


} // namespace do_ast
