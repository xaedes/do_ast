
#include <iostream>
#include <sstream>
#include <chrono>

#include <do_ast/item_pool_tuple.h>
#include <do_ast/v2.h>

namespace do_ast {

    struct Lisp
    {
        using Expression = typename Expressions<>::Expression;
        using Relations = typename Expressions<>::Relations;
        using Value = typename Expressions<>::Value;
        Expressions<> exprs;

        Expression nil()
        {
            return exprs.insert(0);
        }

        Expression value(int32_t value)
        {
            return exprs.insert(1, Relations(), Value::Int32(value));
        }

        Expression cons(Expression car, Expression cdr)
        {
            return exprs.insert(2, Relations(car, cdr));
        }

        Expression list(const std::vector<Expression>& values, std::size_t begin=0)
        {
            return (
                (begin < values.size())
                ? cons(values[begin], list(values, begin+1))
                : nil()
            );
        }
        
        Expression list(const std::vector<int32_t>& values, std::size_t begin=0)
        {
            return (
                (begin < values.size())
                ? cons(value(values[begin]), list(values, begin+1))
                : nil()
            );
        }

        std::string to_string_prefix(const Expression& expr)
        {
            std::stringstream ss;
            exprs.traverse_pre_order(expr, [&ss](auto depth, auto expr_id, auto& type, auto& rel, auto& val){
                for (int k=0;k<depth; ++k)
                {
                    ss << "  ";
                }
                switch(type)
                {
                    case 0:
                    {
                        ss << "nil";
                        break;
                    }
                    case 1:
                    {
                        ss << "value";
                        break;
                    }
                    case 2:
                    {
                        ss << "cons";
                        break;
                    }
                }
                // ss << "type " << type;
                if (val.type == Value::Type::Float)
                {
                    ss << " value " << val.as_float[0];
                }
                if (val.type == Value::Type::Int32)
                {
                    ss << " value " << val.as_int32[0];
                }
                ss << "\n";
            });
            return ss.str();
        }

    };

} // namespace do_ast


int main(int argc, char **argv)
{
    using namespace do_ast;

    using Expression = Lisp::Expression;

    Lisp lisp;

    auto list = lisp.cons(lisp.value(0), lisp.cons(lisp.value(1), lisp.cons(lisp.value(2), lisp.nil())));
    auto list2 = lisp.list({lisp.value(3), lisp.value(4), lisp.value(5)});
    auto list3 = lisp.list({6,7,8});
    auto list4 = lisp.list({lisp.value(1), lisp.list({lisp.value(2), lisp.value(3), lisp.value(4)}), lisp.value(5)});

    std::cout << lisp.to_string_prefix(list) << "\n";
    std::cout << lisp.to_string_prefix(list2) << "\n";
    std::cout << lisp.to_string_prefix(list3) << "\n";
    std::cout << lisp.to_string_prefix(list4) << "\n";

    return 0;
}
