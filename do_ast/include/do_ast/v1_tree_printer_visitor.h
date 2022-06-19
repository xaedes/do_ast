#pragma once

#include <iostream>
#include <do_ast/item_pool.h>
#include <do_ast/v1_ast.h>

namespace do_ast {
namespace v1 {

    struct TreePrinterVisitor
    {
        std::string indent = "    ";
        unsigned int current_indent = 0;

        void print_indent()
        {
            for (unsigned i = 0; i < current_indent; ++i)
            {
                std::cout << indent;
            }
        }

        void print_expr_idx(ItemPoolIndex expr_idx)
        {
            std::cout << "idx[";
            std::cout << std::setbase(16) << std::setfill('0') << std::setw(sizeof(ItemPoolIndex)/16) << expr_idx.index;
            std::cout << ", ";
            print_value(expr_idx.smc);
            std::cout << "]";
        }
        
        void print_expr_type(uint32_t expr_type)
        {
            print_value(expr_type);
        }
    
        void print_value(void* value)
        {
            std::cout << std::setbase(16) << std::setfill('0') << std::setw(sizeof(void*)/16) << value;
        }
        void print_value(bool value)
        {
            std::cout << (value ? "true" : "false");
        }
        void print_value(int8_t value)
        {
            std::cout << std::setbase(10) << std::setw(0) << value;
        }
        void print_value(uint8_t value)
        {
            std::cout << std::setbase(10) << std::setw(0) << value;
        }
        void print_value(int16_t value)
        {
            std::cout << std::setbase(10) << std::setw(0) << value;
        }
        void print_value(uint16_t value)
        {
            std::cout << std::setbase(10) << std::setw(0) << value;
        }
        void print_value(int32_t value)
        {
            std::cout << std::setbase(10) << std::setw(0) << value;
        }
        void print_value(uint32_t value)
        {
            std::cout << std::setbase(10) << std::setw(0) << value;
        }
        void print_value(int64_t value)
        {
            std::cout << std::setbase(10) << std::setw(0) << value;
        }
        void print_value(uint64_t value)
        {
            std::cout << std::setbase(10) << std::setw(0) << value;
        }
        void print_value(float value)
        {
            std::cout << std::defaultfloat << std::setw(0) << value;
        }
        void print_value(double value)
        {
            std::cout << std::defaultfloat << std::setw(0) << value;
        }
        void print_value(const std::string& value)
        {
            std::cout << "'" << value << "'";
        }

        void nil(Ast& ast, ItemPoolIndex expr_idx) 
        {
            print_indent();
            std::cout << "nil ";
            print_expr_idx(expr_idx);
            std::cout << "\n";
        }

        void no_args(Ast& ast, ItemPoolIndex expr_idx, uint32_t expr_type) 
        {
            print_indent();
            std::cout << "no_args ";
            print_expr_idx(expr_idx);
            std::cout << " ";
            print_expr_type(expr_type);
            std::cout << "\n";
        } 

        void with_args(Ast& ast, ItemPoolIndex expr_idx, uint32_t expr_type, ItemPoolIndex arg1) 
        {
            print_indent();
            std::cout << "with_args ";
            print_expr_idx(expr_idx);
            std::cout << " ";
            print_expr_type(expr_type);
            std::cout << "\n";
            ++current_indent;
            ast.visit(*this, arg1);
            --current_indent;

        } 

        void with_args(Ast& ast, ItemPoolIndex expr_idx, uint32_t expr_type, ItemPoolIndex arg1, ItemPoolIndex arg2) 
        {
            print_indent();
            std::cout << "with_args ";
            print_expr_idx(expr_idx);
            std::cout << " ";
            print_expr_type(expr_type);
            std::cout << "\n";
            ++current_indent;
            ast.visit(*this, arg1);
            ast.visit(*this, arg2);
            --current_indent;
        } 

        void with_args(Ast& ast, ItemPoolIndex expr_idx, uint32_t expr_type, ItemPoolIndex arg1, ItemPoolIndex arg2, ItemPoolIndex arg3) 
        {
            print_indent();
            std::cout << "with_args ";
            print_expr_idx(expr_idx);
            std::cout << " ";
            print_expr_type(expr_type);
            std::cout << "\n";
            ++current_indent;
            ast.visit(*this, arg1);
            ast.visit(*this, arg2);
            ast.visit(*this, arg3);
            --current_indent;
        } 

        void with_args(Ast& ast, ItemPoolIndex expr_idx, uint32_t expr_type, ItemPoolIndex arg1, ItemPoolIndex arg2, ItemPoolIndex arg3, ItemPoolIndex arg4) 
        {
            print_indent();
            std::cout << "with_args ";
            print_expr_idx(expr_idx);
            std::cout << " ";
            print_expr_type(expr_type);
            std::cout << "\n";
            ++current_indent;
            ast.visit(*this, arg1);
            ast.visit(*this, arg2);
            ast.visit(*this, arg3);
            ast.visit(*this, arg4);
            --current_indent;
        } 

        void with_value(Ast& ast, ItemPoolIndex expr_idx, uint32_t expr_type) 
        {
            print_indent();
            std::cout << "with_value (void) ";
            print_expr_idx(expr_idx);
            std::cout << " ";
            print_expr_type(expr_type);
            std::cout << "\n";
        } 

        void with_value(Ast& ast, ItemPoolIndex expr_idx, uint32_t expr_type, void* value) 
        {
            print_indent();
            std::cout << "with_value (void*) ";
            print_expr_idx(expr_idx);
            std::cout << " ";
            print_expr_type(expr_type);
            std::cout << " ";
            print_value(value);
            std::cout << "\n";
        } 

        void with_value(Ast& ast, ItemPoolIndex expr_idx, uint32_t expr_type, bool value) 
        {
            print_indent();
            std::cout << "with_value (bool) ";
            print_expr_idx(expr_idx);
            std::cout << " ";
            print_expr_type(expr_type);
            std::cout << " ";
            print_value(value);
            std::cout << "\n";
        } 

        void with_value(Ast& ast, ItemPoolIndex expr_idx, uint32_t expr_type, int8_t value) 
        {
            print_indent();
            std::cout << "with_value (int8_t) ";
            print_expr_idx(expr_idx);
            std::cout << " ";
            print_expr_type(expr_type);
            std::cout << " ";
            print_value(value);
            std::cout << "\n";
        } 

        void with_value(Ast& ast, ItemPoolIndex expr_idx, uint32_t expr_type, uint8_t value) 
        {
            print_indent();
            std::cout << "with_value (uint8_t) ";
            print_expr_idx(expr_idx);
            std::cout << " ";
            print_expr_type(expr_type);
            std::cout << " ";
            print_value(value);
            std::cout << "\n";
        } 

        void with_value(Ast& ast, ItemPoolIndex expr_idx, uint32_t expr_type, int16_t value) 
        {
            print_indent();
            std::cout << "with_value (int16_t) ";
            print_expr_idx(expr_idx);
            std::cout << " ";
            print_expr_type(expr_type);
            std::cout << " ";
            print_value(value);
            std::cout << "\n";
        } 

        void with_value(Ast& ast, ItemPoolIndex expr_idx, uint32_t expr_type, uint16_t value) 
        {
            print_indent();
            std::cout << "with_value (uint16_t) ";
            print_expr_idx(expr_idx);
            std::cout << " ";
            print_expr_type(expr_type);
            std::cout << " ";
            print_value(value);
            std::cout << "\n";
        } 

        void with_value(Ast& ast, ItemPoolIndex expr_idx, uint32_t expr_type, int32_t value) 
        {
            print_indent();
            std::cout << "with_value (int32_t) ";
            print_expr_idx(expr_idx);
            std::cout << " ";
            print_expr_type(expr_type);
            std::cout << " ";
            print_value(value);
            std::cout << "\n";
        } 

        void with_value(Ast& ast, ItemPoolIndex expr_idx, uint32_t expr_type, uint32_t value) 
        {
            print_indent();
            std::cout << "with_value (uint32_t) ";
            print_expr_idx(expr_idx);
            std::cout << " ";
            print_expr_type(expr_type);
            std::cout << " ";
            print_value(value);
            std::cout << "\n";
        } 

        void with_value(Ast& ast, ItemPoolIndex expr_idx, uint32_t expr_type, int64_t value) 
        {
            print_indent();
            std::cout << "with_value (int64_t) ";
            print_expr_idx(expr_idx);
            std::cout << " ";
            print_expr_type(expr_type);
            std::cout << " ";
            print_value(value);
            std::cout << "\n";
        } 

        void with_value(Ast& ast, ItemPoolIndex expr_idx, uint32_t expr_type, uint64_t value) 
        {
            print_indent();
            std::cout << "with_value (uint64_t) ";
            print_expr_idx(expr_idx);
            std::cout << " ";
            print_expr_type(expr_type);
            std::cout << " ";
            print_value(value);
            std::cout << "\n";
        } 

        void with_value(Ast& ast, ItemPoolIndex expr_idx, uint32_t expr_type, float value) 
        {
            print_indent();
            std::cout << "with_value (float) ";
            print_expr_idx(expr_idx);
            std::cout << " ";
            print_expr_type(expr_type);
            std::cout << " ";
            print_value(value);
            std::cout << "\n";
        } 

        void with_value(Ast& ast, ItemPoolIndex expr_idx, uint32_t expr_type, double value) 
        {
            print_indent();
            std::cout << "with_value (double) ";
            print_expr_idx(expr_idx);
            std::cout << " ";
            print_expr_type(expr_type);
            std::cout << " ";
            print_value(value);
            std::cout << "\n";
        } 

        void with_value(Ast& ast, ItemPoolIndex expr_idx, uint32_t expr_type, const std::string& value) 
        {
            print_indent();
            std::cout << "with_value (string) ";
            print_expr_idx(expr_idx);
            std::cout << " ";
            print_expr_type(expr_type);
            std::cout << " ";
            print_value(value);
            std::cout << "\n";
        } 

    };

} // namespace v1
} // namespace do_ast

