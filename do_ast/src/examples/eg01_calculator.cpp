
#include <iostream>
#include <unordered_map>
#include <string>

#include <do_ast/do_ast.h>
#include <do_ast/item_pool.h>

struct Calculator
{
    using Ast = do_ast::Ast;
    using ItemPoolIndex = do_ast::ItemPoolIndex;
    
    Ast ast;

    using Expr_Value = std::integral_constant<uint32_t, 0>;
    using Expr_Add = std::integral_constant<uint32_t, 1>;
    using Expr_Sub = std::integral_constant<uint32_t, 2>;
    using Expr_Mul = std::integral_constant<uint32_t, 3>;
    using Expr_Div = std::integral_constant<uint32_t, 4>;

    std::unordered_map<uint32_t, std::string> str_expr_type;

    Calculator()
    {
        str_expr_type[Expr_Value::value] = "";
        str_expr_type[Expr_Add::value] = " + ";
        str_expr_type[Expr_Sub::value] = " - ";
        str_expr_type[Expr_Mul::value] = " * ";
        str_expr_type[Expr_Div::value] = " / ";
    }

    template<class T>
    ItemPoolIndex value(T value)
    {
        return ast.create_with_value(Expr_Value::value, value);
    }

    //template<class T>
    //ItemPoolIndex value(const T& value)
    //{
    //    return ast.create_with_value(Expr_Value::value, value);
    //}

    ItemPoolIndex add(ItemPoolIndex a, ItemPoolIndex b)
    {
        return ast.create_with_args(Expr_Add::value, a, b);
    }
    ItemPoolIndex sub(ItemPoolIndex a, ItemPoolIndex b)
    {
        return ast.create_with_args(Expr_Sub::value, a, b);
    }
    ItemPoolIndex mul(ItemPoolIndex a, ItemPoolIndex b)
    {
        return ast.create_with_args(Expr_Mul::value, a, b);
    }
    ItemPoolIndex div(ItemPoolIndex a, ItemPoolIndex b)
    {
        return ast.create_with_args(Expr_Div::value, a, b);
    }

    struct PrintVisitor
    {
        PrintVisitor(Calculator& calc) : calc(&calc) {}

        Calculator* calc;
        void nil(Ast& ast, ItemPoolIndex expr_idx) {} 
        void no_args(Ast& ast, ItemPoolIndex expr_idx, uint32_t expr_type) {} 
        void with_args(Ast& ast, ItemPoolIndex expr_idx, uint32_t expr_type, ItemPoolIndex arg1) {} 
        void with_args(Ast& ast, ItemPoolIndex expr_idx, uint32_t expr_type, ItemPoolIndex arg1, ItemPoolIndex arg2) 
        { 
            std::cout << "(";
            ast.visit(arg1, *this);
            std::cout << calc->str_expr_type[expr_type];
            ast.visit(arg2, *this);
            std::cout << ")";
        } 
        void with_args(Ast& ast, ItemPoolIndex expr_idx, uint32_t expr_type, ItemPoolIndex arg1, ItemPoolIndex arg2, ItemPoolIndex arg3) {} 
        void with_args(Ast& ast, ItemPoolIndex expr_idx, uint32_t expr_type, ItemPoolIndex arg1, ItemPoolIndex arg2, ItemPoolIndex arg3, ItemPoolIndex arg4) {} 
        void with_value(Ast& ast, ItemPoolIndex expr_idx, uint32_t expr_type) {} 
        void with_value(Ast& ast, ItemPoolIndex expr_idx, uint32_t expr_type, void* value) {} 
        void with_value(Ast& ast, ItemPoolIndex expr_idx, uint32_t expr_type, bool value)     { std::cout << value; } 
        void with_value(Ast& ast, ItemPoolIndex expr_idx, uint32_t expr_type, int8_t value)   { std::cout << value; } 
        void with_value(Ast& ast, ItemPoolIndex expr_idx, uint32_t expr_type, uint8_t value)  { std::cout << value; } 
        void with_value(Ast& ast, ItemPoolIndex expr_idx, uint32_t expr_type, int16_t value)  { std::cout << value; } 
        void with_value(Ast& ast, ItemPoolIndex expr_idx, uint32_t expr_type, uint16_t value) { std::cout << value; } 
        void with_value(Ast& ast, ItemPoolIndex expr_idx, uint32_t expr_type, int32_t value)  { std::cout << value; } 
        void with_value(Ast& ast, ItemPoolIndex expr_idx, uint32_t expr_type, uint32_t value) { std::cout << value; } 
        void with_value(Ast& ast, ItemPoolIndex expr_idx, uint32_t expr_type, int64_t value)  { std::cout << value; } 
        void with_value(Ast& ast, ItemPoolIndex expr_idx, uint32_t expr_type, uint64_t value) { std::cout << value; } 
        void with_value(Ast& ast, ItemPoolIndex expr_idx, uint32_t expr_type, float value)    { std::cout << value; } 
        void with_value(Ast& ast, ItemPoolIndex expr_idx, uint32_t expr_type, double value)   { std::cout << value; } 
        void with_value(Ast& ast, ItemPoolIndex expr_idx, uint32_t expr_type, const std::string& value) {} 
    };    
    struct EvaluationVisitor
    {
        EvaluationVisitor(Calculator& calc) : calc(&calc) {}

        Calculator* calc;

        std::vector<double> stack;

        void nil(Ast& ast, ItemPoolIndex expr_idx) {} 
        void no_args(Ast& ast, ItemPoolIndex expr_idx, uint32_t expr_type) {} 
        void with_args(Ast& ast, ItemPoolIndex expr_idx, uint32_t expr_type, ItemPoolIndex arg1) {} 
        void with_args(Ast& ast, ItemPoolIndex expr_idx, uint32_t expr_type, ItemPoolIndex arg1, ItemPoolIndex arg2) 
        { 
            ast.visit(arg1, *this);
            ast.visit(arg2, *this);
            auto eval_1 = stack.back(); stack.pop_back();
            auto eval_2 = stack.back(); stack.pop_back();
            switch (expr_type)
            {
                case Expr_Add::value: stack.push_back(eval_1 + eval_2); return;
                case Expr_Sub::value: stack.push_back(eval_1 - eval_2); return;
                case Expr_Mul::value: stack.push_back(eval_1 * eval_2); return;
                case Expr_Div::value: stack.push_back(eval_1 / eval_2); return;
            }
        } 
        void with_args(Ast& ast, ItemPoolIndex expr_idx, uint32_t expr_type, ItemPoolIndex arg1, ItemPoolIndex arg2, ItemPoolIndex arg3) {} 
        void with_args(Ast& ast, ItemPoolIndex expr_idx, uint32_t expr_type, ItemPoolIndex arg1, ItemPoolIndex arg2, ItemPoolIndex arg3, ItemPoolIndex arg4) {} 
        void with_value(Ast& ast, ItemPoolIndex expr_idx, uint32_t expr_type) {} 
        void with_value(Ast& ast, ItemPoolIndex expr_idx, uint32_t expr_type, void* value) {} 
        void with_value(Ast& ast, ItemPoolIndex expr_idx, uint32_t expr_type, bool value)     { stack.push_back(value); } 
        void with_value(Ast& ast, ItemPoolIndex expr_idx, uint32_t expr_type, int8_t value)   { stack.push_back(value); } 
        void with_value(Ast& ast, ItemPoolIndex expr_idx, uint32_t expr_type, uint8_t value)  { stack.push_back(value); } 
        void with_value(Ast& ast, ItemPoolIndex expr_idx, uint32_t expr_type, int16_t value)  { stack.push_back(value); } 
        void with_value(Ast& ast, ItemPoolIndex expr_idx, uint32_t expr_type, uint16_t value) { stack.push_back(value); } 
        void with_value(Ast& ast, ItemPoolIndex expr_idx, uint32_t expr_type, int32_t value)  { stack.push_back(value); } 
        void with_value(Ast& ast, ItemPoolIndex expr_idx, uint32_t expr_type, uint32_t value) { stack.push_back(value); } 
        void with_value(Ast& ast, ItemPoolIndex expr_idx, uint32_t expr_type, int64_t value)  { stack.push_back(value); } 
        void with_value(Ast& ast, ItemPoolIndex expr_idx, uint32_t expr_type, uint64_t value) { stack.push_back(value); } 
        void with_value(Ast& ast, ItemPoolIndex expr_idx, uint32_t expr_type, float value)    { stack.push_back(value); } 
        void with_value(Ast& ast, ItemPoolIndex expr_idx, uint32_t expr_type, double value)   { stack.push_back(value); } 
        void with_value(Ast& ast, ItemPoolIndex expr_idx, uint32_t expr_type, const std::string& value) {} 
    };    
};

int main(int argc, char **argv)
{
    using namespace do_ast;

    Calculator calc;
    auto a = calc.mul( calc.add(calc.value(0.5f), calc.value(0.5f)), calc.value(10) );

    calc.ast.visit(a, TreePrinterVisitor());

    std::cout << "\n";

    calc.ast.visit(a, Calculator::PrintVisitor(calc));

    std::cout << " = ";

    Calculator::EvaluationVisitor eval(calc);
    calc.ast.visit(a, eval);

    std::cout  << eval.stack.back() << "\n";


    return 0;
}
