
#include <iostream>
#include <sstream>
#include <chrono>

#include <do_ast/item_pool_tuple.h>
#include <do_ast/v2.h>

struct Operation
{
    uint32_t  res;
    uint32_t  lhs;
    uint32_t  rhs;
    uint32_t  pad;
};

// using Number = int;
template<class T>
void mk_reduction(std::vector<T>& values, std::vector<Operation>& ops)
{

    /*
     * 00  01  02  03  04  05  06  07  08
     *  \09/    \10/    \11/    \12/   |
     *    \--13--/        \--14--/     |
     *        \------15------/         |
     *                \---------16-----/
     */

    /*
     * 00  01  02  03  04  05  06  07  08  09  10
     *  \11/    \12/    \13/    \14/    \15/   |
     *    \--16--/        \--17--/        \-18-/
     *        \------19------/              |
     *                \---------20----------/
     */


    uint32_t last_overflow = 0;
    bool has_last_overflow = false;

    uint32_t last_start = 0;
    uint32_t last_count = values.size();

    while (last_count > 1)
    {
        for(uint32_t idx = 0; idx+1 < last_count; idx+=2)
        {
            uint32_t res = static_cast<uint32_t>(values.size());
            auto lhs = last_start+idx;
            auto rhs = last_start+idx+1;

            ops.push_back({res,lhs,rhs});
            values.emplace_back();
        }
        if (last_count % 2 == 1)
        {
            if (has_last_overflow)
            {
                uint32_t res = static_cast<uint32_t>(values.size());
                auto lhs = last_start+last_count-1;
                auto rhs = last_overflow;

                ops.push_back({res,lhs,rhs});
                values.emplace_back();

                has_last_overflow = false;
            }
            else
            {
                last_overflow = last_start+last_count-1;
                has_last_overflow = true;
            }
        }
        // auto new_count = values.size() - last_start;
        // if (new_count % 2 == 1)
        // {
        //     last_overflow = last_start+last_count-1;
        //     has_last_overflow = true;
        // }
        last_start = last_start + last_count;
        // auto new_count = values.size() - last_start;
        last_count = values.size() - last_start;
        // last_count = count;
    }
    if (has_last_overflow)
    {
        uint32_t res = static_cast<uint32_t>(values.size());
        auto lhs = last_start+last_count-1;
        auto rhs = last_overflow;

        ops.push_back({res,lhs,rhs});
        values.emplace_back();
    }
}

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

    using Expression = typename Expressions<>::Expression;
    using Relations = typename Expressions<>::Relations;
    using Value = typename Expressions<>::Value;

    auto null_val = exprs.insert(0);
    auto a = exprs.insert(0, Relations(), Value::Float(0.5f));
    auto b = exprs.insert(0, Relations(), Value::Float(1.5f));
    auto c = exprs.insert(1, Relations(a, b));

    auto Number = [&exprs](auto val) { return exprs.insert(0, Relations(), Value::Int32(val)); };
    auto Add = [&exprs](auto a, auto b) { return exprs.insert(1, Relations(a, b)); };
    auto Add3 = [&exprs](auto a, auto b, auto c) { return exprs.insert(1, Relations(a, b, c)); };

    auto d = Add3(Add(Number(0.1f), Add(Number(1.0f), Number(2.0f))), Add(Number(1), Number(3)), Number(7));

    // auto relations = exprs.pool.slots<1>();
    // exprs.pool.insert(0, Value::Void());
    
    std::cout << "---" << "\n";

    auto PrintTreePreOrder = [&exprs](auto expr){
        exprs.traverse_pre_order(expr, [](auto depth, auto expr_id, auto& type, auto& rel, auto& val){
            for (int k=0;k<depth; ++k)
            {
                std::cout << "  ";
            }
            std::cout << "type " << type;
            if (val.type == Value::Type::Float)
            {
                std::cout << " value " << val.as_float[0];
            }
            if (val.type == Value::Type::Int32)
            {
                std::cout << " value " << val.as_int32[0];
            }
            std::cout << "\n";
        });
    };

    PrintTreePreOrder(d);
    
    std::cout << "---" << "\n";

    auto PrintTreePostOrder = [&exprs](auto expr){
        exprs.traverse_post_order(expr, [](auto depth, auto expr_id, auto& type, auto& rel, auto& val){
            for (int k=0;k<depth; ++k)
            {
                std::cout << "  ";
            }
            std::cout << "type " << type;
            if (val.type == Value::Type::Float)
            {
                std::cout << " value " << val.as_float[0];
            }
            if (val.type == Value::Type::Int32)
            {
                std::cout << " value " << val.as_int32[0];
            }
            std::cout << "\n";
        });
    };
    PrintTreePostOrder(d);

    std::cout << "---" << "\n";

    auto PrintInOrder = [&exprs](auto expr){
        std::stringstream ss;
        // std::vector<double> values_stack;
        exprs.traverse_in_order(
            expr, 
            [&ss](auto depth, auto expr_id, auto* type, auto* rel, auto* val){
                if (!type) return;
                if (*type == 0) return;
                ss << "(";
            },
            [&ss](auto depth, auto expr_id, auto* type, auto* rel, auto* val){

                if (!type) return;
                if (*type == 0)
                {
                    // ss << val->as_float[0];
                    ss << val->as_int32[0];
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
    };
    PrintInOrder(d);

    std::cout << "---" << "\n";
    using ScalarType = int32_t;
    // using ScalarType = float;
    std::vector<ScalarType> values_stack;
    auto EvaluateAdd = [&exprs, &values_stack](auto expr) -> ScalarType {
        values_stack.clear();
        exprs.traverse_post_order(expr, [&values_stack](auto depth, auto expr_id, auto& type, auto& rel, auto& val){
            if (type == 0)
            {
                values_stack.push_back(val.as_int32[0]);
                // values_stack.push_back(val.as_float[0]);
            }
            else if (type == 1)
            {
                ScalarType sum = 0;
                for (int k=0;k<rel.num_args; ++k)
                {
                    sum += values_stack[values_stack.size()-1-k];
                }
                values_stack.resize(1 + values_stack.size() - rel.num_args);
                values_stack.back() = sum;
            }
        });
        return values_stack.back();
    };

    std::cout << "=" << EvaluateAdd(d) << "\n";

    std::cout << "---" << "\n";

    // using Number = double;
    // std::vector<double> cvalues;
    // cvalues.resize(1024*1024);
    // for (auto i = 0; i < cvalues.size(); ++i)
    // {
    //     cvalues[i] = i;
    // }
    std::vector<Expression> cexpressions;
    std::vector<Expression> expressions;
     // cexpressions.resize(1024*1024); //  1.37246e+08 fps
      //cexpressions.resize(1024*128); // 2.85103e+08 fps
      //cexpressions.resize(1024*16); // 4.67046e+08 fps
      //cexpressions.resize(1024*4);  // 5.71082e+08 fps
      //cexpressions.resize(1024*3);  // 6.42956e+08 fps
      cexpressions.resize(1024*2);  // 7.05106e+08 fps <- biggest good
      //cexpressions.resize(1024);  // 7.16236e+08 fps
     //cexpressions.resize(512); // 7.46908e+08 fps
     //cexpressions.resize(256); // 8.19056e+08 fps <- optimal
     //cexpressions.resize(128); // 7.87498e+08 fps
    // int num_it = 1024;
    //int num_it = 1024*8;
    //int num_it = 1024*8*8;
    //int num_it = 1024*8*8*4;
    // int num_it = 1024*8*8*4*2;
    int num_it = 1024*8*8*4*4;
    //int num_it = 1024*8*8*4*4*2;
    //int num_it = 1024*8*8*4*4*2*2;
    //int num_it = 1024*8*8*4*4*2*2*2;
    //int num_it = 1024*8*8*4*4*8;
    for (auto i = 0; i < cexpressions.size(); ++i)
    {
        cexpressions[i] = Number(i);
    }
    expressions = cexpressions;
    std::vector<Operation> operations;
    std::vector<Operation> operations_expr;
    struct PtrOp
    {
        int32_t* res;
        int32_t* lhs;
        int32_t* rhs;
    };
    std::vector<PtrOp> operations_ptr;
    mk_reduction(expressions, operations);
    for (auto i = 0; i < operations.size(); ++i)
    {
        auto op = operations[i];
        expressions[op.res] = Add(expressions[op.lhs], expressions[op.rhs]);
        auto op_expr = op;
        op_expr.res = expressions[op.res].index;
        op_expr.lhs = expressions[op.lhs].index;
        op_expr.rhs = expressions[op.rhs].index;
        operations_expr.push_back(op_expr);
    }
    for (auto i = 0; i < operations.size(); ++i)
    {
        auto op = operations[i];
        operations_ptr.push_back({
            &(exprs.pool.get<2>(expressions[op.res]).as_int32[0]),
            &(exprs.pool.get<2>(expressions[op.lhs]).as_int32[0]),
            &(exprs.pool.get<2>(expressions[op.rhs]).as_int32[0])
        });
    }
    auto expr = expressions.back();
    // PrintInOrder(expr);


    std::cout << "=" << EvaluateAdd(expr) << "\n";
    
    std::cout << "---" << "\n";

    auto t0 = std::chrono::system_clock::now();
    ScalarType sum0 = 0;
    for (int i=0; i<num_it; ++i)
    {
        sum0 += EvaluateAdd(expr);
    }
    auto t1 = std::chrono::system_clock::now();
    ScalarType sum1 = 0;
    for (int i=0; i<num_it; ++i)
    {
        //const auto* types     = exprs.pool.slots<0>().data();
        //const auto* relations = exprs.pool.slots<1>().data();
        auto* values    = exprs.pool.slots<2>().data();
        for (auto i = 0; i < operations.size(); ++i)
        {
            auto op = operations[i];
            auto res = expressions[op.res].index;
            auto lhs = expressions[op.lhs].index;
            auto rhs = expressions[op.rhs].index;
            values[res].as_int32[0] = values[lhs].as_int32[0] + values[rhs].as_int32[0];
            // values[res].as_float[0] = values[lhs].as_float[0] + values[rhs].as_float[0];
        }
        sum1 += values[expressions.back().index].as_int32[0];
        //sum1 += values[expressions.back().index].as_float[0];
    }
    auto t2 = std::chrono::system_clock::now();

    ScalarType sum2 = 0;
    for (int i=0; i<num_it; ++i)
    {
        //const auto* types     = exprs.pool.slots<0>().data();
        //const auto* relations = exprs.pool.slots<1>().data();
        auto* values    = exprs.pool.slots<2>().data();
        for (auto i = 0; i < operations_expr.size(); ++i)
        {
            auto op = operations_expr[i];
            auto res = op.res;
            auto lhs = op.lhs;
            auto rhs = op.rhs;
            values[res].as_int32[0] = values[lhs].as_int32[0] + values[rhs].as_int32[0];
            values[res].as_int32[1] = values[lhs].as_int32[1] + values[rhs].as_int32[1];
            // values[res].as_float[0] = values[lhs].as_float[0] + values[rhs].as_float[0];
        }
        sum2 += values[expressions.back().index].as_int32[0];
        //sum2 += values[expressions.back().index].as_float[0];
    }
    
    auto t3 = std::chrono::system_clock::now();

    ScalarType sum3 = 0;
    for (int i=0; i<num_it; ++i)
    {
        //const auto* types     = exprs.pool.slots<0>().data();
        //const auto* relations = exprs.pool.slots<1>().data();
        auto* values    = exprs.pool.slots<2>().data();
        for (auto i = 0; i < operations_ptr.size(); ++i)
        {
            auto op = operations_ptr[i];
            auto* res = op.res;
            auto* lhs = op.lhs;
            auto* rhs = op.rhs;
            *res = *lhs + *rhs;
        }
        sum3 += *operations_ptr.back().res;
    }
    
    auto t4 = std::chrono::system_clock::now();

    // double dnorm = static_cast<double>(operations.size() * num_it); 
    double dnorm = static_cast<double>(num_it); 
    std::chrono::duration<double> d0 = t1-t0;
    std::chrono::duration<double> d1 = t2-t1;
    std::chrono::duration<double> d2 = t3-t2;
    std::chrono::duration<double> d3 = t4-t3;
    double fps0 = abs(d0.count()) > 1e-12 ? (dnorm / d0.count()) : 0;
    double fps1 = abs(d1.count()) > 1e-12 ? (dnorm / d1.count()) : 0;
    double fps2 = abs(d2.count()) > 1e-12 ? (dnorm / d2.count()) : 0;
    double fps3 = abs(d3.count()) > 1e-12 ? (dnorm / d3.count()) : 0;
    std::cout << "phase0: " << (d0.count() / dnorm) * 1000 << " ms " << fps0 << " fps\n";
    std::cout << "phase1: " << (d1.count() / dnorm) * 1000 << " ms " << fps1 << " fps\n";
    std::cout << "phase2: " << (d2.count() / dnorm) * 1000 << " ms " << fps2 << " fps\n";
    std::cout << "phase3: " << (d3.count() / dnorm) * 1000 << " ms " << fps3 << " fps\n";
    std::cout << " sum0 " << sum0 << "\n";
    std::cout << " sum1 " << sum1 << "\n";
    std::cout << " sum2 " << sum2 << "\n";
    std::cout << " sum3 " << sum3 << "\n";

    std::cout << "---" << "\n";


    return 0;
}
