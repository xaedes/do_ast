
#include <iostream>
#include <sstream>

#include <do_ast/item_pool_tuple.h>
#include <do_ast/v2.h>

int main(int argc, char **argv)
{
    using namespace do_ast;

    {
        ItemPoolTuple<float, int> pool;
        auto a = pool.insert(0.5, 2);
        std::cout << pool.get<0>(a) << "\n";
        std::cout << pool.get<1>(a) << "\n";
    }

    Expressions<> exprs;

    auto null_val = exprs.insert(0);
    auto a = exprs.insert(0, Relations(), Value::Float(0.5f));
    auto b = exprs.insert(0, Relations(), Value::Float(1.5f));
    auto c = exprs.insert(1, Relations(a, b));

    auto Number = [&exprs](auto val) { return exprs.insert(0, Relations(), Value::Float(val)); };
    auto Add = [&exprs](auto a, auto b) { return exprs.insert(1, Relations(a, b)); };
    auto Add3 = [&exprs](auto a, auto b, auto c) { return exprs.insert(1, Relations(a, b, c)); };

    auto d = Add3(Add(Number(0.1f), Add(Number(1.0f), Number(2.0f))), Add(Number(1), Number(3)), Number(7));

    // auto relations = exprs.pool.slots<1>();
    // exprs.pool.insert(0, Value::Void());
    
    std::cout << "---" << "\n";

    exprs.traverse_pre_order(d, [](auto depth, auto expr_id, auto& type, auto& rel, auto& val){
        for (int k=0;k<depth; ++k)
        {
            std::cout << "  ";
        }
        std::cout << "type " << type;
        if (val.type == Value::Type::Float)
        {
            std::cout << " value " << val.as_float[0];
        }
        std::cout << "\n";
    });
    
    std::cout << "---" << "\n";

    exprs.traverse_post_order(d, [](auto depth, auto expr_id, auto& type, auto& rel, auto& val){
        for (int k=0;k<depth; ++k)
        {
            std::cout << "  ";
        }
        std::cout << "type " << type;
        if (val.type == Value::Type::Float)
        {
            std::cout << " value " << val.as_float[0];
        }
        std::cout << "\n";
    });

    std::cout << "---" << "\n";

    std::stringstream ss;
    // std::vector<double> values_stack;
    exprs.traverse_in_order(
        d, 
        [&ss](auto depth, auto expr_id, auto* type, auto* rel, auto* val){
            if (!type) return;
            if (*type == 0) return;
            ss << "(";
        },
        [&ss](auto depth, auto expr_id, auto* type, auto* rel, auto* val){

            if (!type) return;
            if (*type == 0)
            {
                ss << val->as_float[0];
            }
            else if (*type == 1)
            {
                ss << " + ";
            }
        },
        [&ss](auto depth, auto expr_id, auto* type, auto* rel, auto* val){
            if (!type) return;
            if (*type == 0) return;
            ss << ")";
        }
    );
    std::cout << ss.str() << "\n";

    std::cout << "---" << "\n";

    std::vector<double> values_stack;
    exprs.traverse_post_order(d, [&values_stack](auto depth, auto expr_id, auto& type, auto& rel, auto& val){
        if (type == 0)
        {
            values_stack.push_back(val.as_float[0]);
        }
        else if (type == 1)
        {
            double sum = 0;
            for (int k=0;k<rel.num_args; ++k)
            {
                sum += values_stack[values_stack.size()-1-k];
            }
            values_stack.resize(1 + values_stack.size() - rel.num_args);
            values_stack.back() = sum;
        }
    });

    std::cout << values_stack.back() << "\n";

    return 0;
}
