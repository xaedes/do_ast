
#include <do_ast/ast.h>

namespace do_ast {

    ItemPoolIndex Ast::create(
        uint32_t expr_type
    )
    {
        return m_expr_pool.emplace(expr_type, ArgTypes::NoArgs::value, ItemPoolIndex());
    }


    ItemPoolIndex Ast::create_with_args(
        uint32_t expr_type, ItemPoolIndex arg1
    )
    {
        return m_expr_pool.emplace(expr_type, ArgTypes::WithArgs<1>::value, m_arg_expr_list_1_pool.insert({arg1}));
    }

    ItemPoolIndex Ast::create_with_args(
        uint32_t expr_type, ItemPoolIndex arg1, ItemPoolIndex arg2
    )
    {
        return m_expr_pool.emplace(expr_type, ArgTypes::WithArgs<2>::value, m_arg_expr_list_2_pool.insert({arg1, arg2}));
    }

    ItemPoolIndex Ast::create_with_args(
        uint32_t expr_type, ItemPoolIndex arg1, ItemPoolIndex arg2, ItemPoolIndex arg3
    )
    {
        return m_expr_pool.emplace(expr_type, ArgTypes::WithArgs<3>::value, m_arg_expr_list_3_pool.insert({arg1, arg2, arg3}));
    }

    ItemPoolIndex Ast::create_with_args(
        uint32_t expr_type, ItemPoolIndex arg1, ItemPoolIndex arg2, ItemPoolIndex arg3, ItemPoolIndex arg4
    )
    {
        return m_expr_pool.emplace(expr_type, ArgTypes::WithArgs<4>::value, m_arg_expr_list_4_pool.insert({arg1, arg2, arg3, arg4}));
    }


    ItemPoolIndex Ast::create_with_value(
        uint32_t expr_type
    )
    {
        return m_expr_pool.emplace(expr_type, ArgTypes::WithValue<void>::value, ItemPoolIndex());
    }

    ItemPoolIndex Ast::create_with_value(
        uint32_t expr_type, void* value
    )
    {
        return m_expr_pool.emplace(expr_type, ArgTypes::WithValue<void*>::value, m_arg_value_voidptr_pool.emplace(value));
    }

    ItemPoolIndex Ast::create_with_value(
        uint32_t expr_type, int8_t value
    )
    {
        return m_expr_pool.emplace(expr_type, ArgTypes::WithValue<int8_t>::value, m_arg_value_int8_pool.emplace(value));
    }

    ItemPoolIndex Ast::create_with_value(
        uint32_t expr_type, bool value
    )
    {
        return m_expr_pool.emplace(expr_type, ArgTypes::WithValue<bool>::value, m_arg_value_bool_pool.emplace(value));
    }

    ItemPoolIndex Ast::create_with_value(
        uint32_t expr_type, uint8_t value
    )
    {
        return m_expr_pool.emplace(expr_type, ArgTypes::WithValue<uint8_t>::value, m_arg_value_uint8_pool.emplace(value));
    }

    ItemPoolIndex Ast::create_with_value(
        uint32_t expr_type, int16_t value
    )
    {
        return m_expr_pool.emplace(expr_type, ArgTypes::WithValue<int16_t>::value, m_arg_value_int16_pool.emplace(value));
    }

    ItemPoolIndex Ast::create_with_value(
        uint32_t expr_type, uint16_t value
    )
    {
        return m_expr_pool.emplace(expr_type, ArgTypes::WithValue<uint16_t>::value, m_arg_value_uint16_pool.emplace(value));
    }

    ItemPoolIndex Ast::create_with_value(
        uint32_t expr_type, int32_t value
    )
    {
        return m_expr_pool.emplace(expr_type, ArgTypes::WithValue<int32_t>::value, m_arg_value_int32_pool.emplace(value));
    }

    ItemPoolIndex Ast::create_with_value(
        uint32_t expr_type, uint32_t value
    )
    {
        return m_expr_pool.emplace(expr_type, ArgTypes::WithValue<uint32_t>::value, m_arg_value_uint32_pool.emplace(value));
    }

    ItemPoolIndex Ast::create_with_value(
        uint32_t expr_type, int64_t value
    )
    {
        return m_expr_pool.emplace(expr_type, ArgTypes::WithValue<int64_t>::value, m_arg_value_int64_pool.emplace(value));
    }

    ItemPoolIndex Ast::create_with_value(
        uint32_t expr_type, uint64_t value
    )
    {
        return m_expr_pool.emplace(expr_type, ArgTypes::WithValue<uint64_t>::value, m_arg_value_uint64_pool.emplace(value));
    }

    ItemPoolIndex Ast::create_with_value(
        uint32_t expr_type, float value
    )
    {
        return m_expr_pool.emplace(expr_type, ArgTypes::WithValue<float>::value, m_arg_value_float_pool.emplace(value));
    }

    ItemPoolIndex Ast::create_with_value(
        uint32_t expr_type, double value
    )
    {
        return m_expr_pool.emplace(expr_type, ArgTypes::WithValue<double>::value, m_arg_value_double_pool.emplace(value));
    }

    ItemPoolIndex Ast::create_with_value(
        uint32_t expr_type, std::string value
    )
    {
        return m_expr_pool.emplace(expr_type, ArgTypes::WithValue<std::string>::value, m_arg_value_string_pool.emplace(value));
    }

    void Ast::erase_expr(ItemPoolIndex expr_idx)
    {
        const auto* expr = get(expr_idx);
        if (!expr) return;
        erase_arg(expr->arg_type, expr->arg_idx);
        m_expr_pool.erase(expr_idx);
    }

    void Ast::erase_expr_recursive(ItemPoolIndex expr_idx)
    {
        const auto* expr = get(expr_idx);
        if (!expr) return;
        switch(expr->arg_type)
        {
            case ArgTypes::WithArgs<1>::value: 
            {
                for (auto item : m_arg_expr_list_1_pool.get(expr->arg_idx))
                {
                    erase_expr_recursive(item);
                }
                break;
            }
            case ArgTypes::WithArgs<2>::value: 
            {
                for (auto item : m_arg_expr_list_2_pool.get(expr->arg_idx))
                {
                    erase_expr_recursive(item);
                }
                break;
            }
            case ArgTypes::WithArgs<3>::value: 
            {
                for (auto item : m_arg_expr_list_3_pool.get(expr->arg_idx))
                {
                    erase_expr_recursive(item);
                }
                break;
            }
            case ArgTypes::WithArgs<4>::value: 
            {
                for (auto item : m_arg_expr_list_4_pool.get(expr->arg_idx))
                {
                    erase_expr_recursive(item);
                }
                break;
            }
            default:
                break;
        }
        erase_arg(expr->arg_type, expr->arg_idx);
        m_expr_pool.erase(expr_idx);
    }

    void Ast::erase_arg(uint32_t arg_type, ItemPoolIndex arg_idx)
    {
        switch(arg_type)
        {
            case ArgTypes::NoArgs::value: 
            {
                break;
            }
            case ArgTypes::WithArgs<1>::value: 
            {
                m_arg_expr_list_1_pool.erase(arg_idx);
                break;
            }
            case ArgTypes::WithArgs<2>::value: 
            {
                m_arg_expr_list_2_pool.erase(arg_idx);
                break;
            }
            case ArgTypes::WithArgs<3>::value: 
            {
                m_arg_expr_list_3_pool.erase(arg_idx);
                break;
            }
            case ArgTypes::WithArgs<4>::value: 
            {
                m_arg_expr_list_4_pool.erase(arg_idx);
                break;
            }
            case ArgTypes::WithValue<void>::value:
            {
                break;
            }
            case ArgTypes::WithValue<void*>::value:
            {
                m_arg_value_voidptr_pool.erase(arg_idx);
                break;
            }
            case ArgTypes::WithValue<bool>::value:
            {
                m_arg_value_bool_pool.erase(arg_idx);
                break;
            }
            case ArgTypes::WithValue<int8_t>::value:
            {
                m_arg_value_int8_pool.erase(arg_idx);
                break;
            }
            case ArgTypes::WithValue<uint8_t>::value:
            {
                m_arg_value_uint8_pool.erase(arg_idx);
                break;
            }
            case ArgTypes::WithValue<int16_t>::value:
            {
                m_arg_value_int16_pool.erase(arg_idx);
                break;
            }
            case ArgTypes::WithValue<uint16_t>::value:
            {
                m_arg_value_uint16_pool.erase(arg_idx);
                break;
            }
            case ArgTypes::WithValue<int32_t>::value:
            {
                m_arg_value_int32_pool.erase(arg_idx);
                break;
            }
            case ArgTypes::WithValue<uint32_t>::value:
            {
                m_arg_value_uint32_pool.erase(arg_idx);
                break;
            }
            case ArgTypes::WithValue<int64_t>::value:
            {
                m_arg_value_int64_pool.erase(arg_idx);
                break;
            }
            case ArgTypes::WithValue<uint64_t>::value:
            {
                m_arg_value_uint64_pool.erase(arg_idx);
                break;
            }
            case ArgTypes::WithValue<float>::value:
            {
                m_arg_value_float_pool.erase(arg_idx);
                break;
            }
            case ArgTypes::WithValue<double>::value:
            {
                m_arg_value_double_pool.erase(arg_idx);
                break;
            }
            case ArgTypes::WithValue<std::string>::value:
            {
                m_arg_value_string_pool.erase(arg_idx);
                break;
            }
        }
    }

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


} // namespace do_ast
