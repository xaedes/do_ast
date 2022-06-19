
#include <iostream>
#include <unordered_map>
#include <string>
#include <cassert>
#include <chrono>

#include <do_ast/v1.h>
#include <do_ast/item_pool.h>

struct Calculator
{
    using Ast = do_ast::v1::Ast;
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
            ast.visit(*this, arg1);
            std::cout << calc->str_expr_type[expr_type];
            ast.visit(*this, arg2);
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
            ast.visit(*this, arg1);
            auto eval_1 = stack.back(); stack.pop_back();
            ast.visit(*this, arg2);
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

template <class T>
do_ast::ItemPoolIndex mk_long_add(Calculator& calc, T begin, T end)
{
    auto count = std::distance(begin, end);
    if (count == 0)
    {
        return calc.value(0);
    }
    else if (count == 1)
    {
        return calc.value(*begin);
    }
    if (count == 2)
    {
        return calc.add(
            calc.value(begin[0]),
            calc.value(begin[1])
        );
    }
    else
    {
        static std::vector<do_ast::ItemPoolIndex> s_exprs;
        s_exprs.clear();
        for(uint32_t idx = 0; idx < count; idx++)
        {
            s_exprs.push_back(calc.value(begin[idx]));
        }
        uint32_t last_start = 0;
        uint32_t last_count = s_exprs.size();

        while (last_count > 1)
        {

            for(uint32_t idx = 0; idx+1 < last_count; idx+=2)
            {
                auto a = s_exprs[last_start+idx];
                auto b = s_exprs[last_start+idx+1];
                s_exprs.push_back(calc.add(a,b));
            }
            if (last_count % 2 == 1)
            {
                s_exprs.push_back(s_exprs[last_start+last_count-1]);
            }
            last_start = last_start + last_count;
            last_count = s_exprs.size() - last_start;
            // last_count = count;
        }
        assert(last_count == 1);
        auto result = s_exprs.back();
        return result;

        // return calc.add(
        //     calc.value(begin[0]),
        //     mk_long_add<T>(calc, begin+1, end)
        // );
    }
}

int main(int argc, char **argv)
{
    using namespace do_ast::v1;

    Calculator calc;
    auto a = calc.mul( calc.add(calc.value(0.5f), calc.value(0.5f)), calc.value(10) );

    calc.ast.visit(TreePrinterVisitor(), a);

    std::cout << "\n";

    calc.ast.visit(Calculator::PrintVisitor(calc), a);

    std::cout << " = ";

    Calculator::EvaluationVisitor eval(calc);
    calc.ast.visit(eval, a);

    std::cout  << eval.stack.back() << "\n";

    std::vector<double> values;
    values.resize(1024*1024);
    for (auto i = 0; i < values.size(); ++i)
    {
        values[i] = i;
    }
    
    auto b = mk_long_add(calc, values.begin(), values.end());

    std::cout << "\n";

    // calc.ast.visit(TreePrinterVisitor(), b);

    std::cout << "\n";

    // calc.ast.visit(Calculator::PrintVisitor(calc), b);
    std::cout << " = ";
    calc.ast.visit(eval, b);
    std::cout  << eval.stack.back() << "\n";
    
    auto t0 = std::chrono::system_clock::now();
    int num_it = 1024;
    double sum = 0;
    for (int i=0; i<num_it; ++i)
    {
        Calculator::EvaluationVisitor eval(calc);
        calc.ast.visit(eval, b);
        sum += eval.stack.back();
    }
    auto t1 = std::chrono::system_clock::now();
    std::chrono::duration<double> d0 = t1-t0;
    double fps0 = abs(d0.count()) > 1e-12 ? (num_it / d0.count()) : 0;
    std::cout << "phase0: " << (d0.count() / num_it) * 1000 << " ms " << fps0 << " fps\n";
    std::cout << " sum " << sum;
    return 0;
}
