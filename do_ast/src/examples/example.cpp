
#include <iostream>
#include <vector>
#include <iterator>
#include <type_traits>
#include <limits>
#include <chrono>

#include <do_ast/do_ast.h>
#include <do_ast/benchmark.h>
#include <do_ast/arg_template_helpers.h>
#include <do_ast/nodes.h>
#include <do_ast/visit_struct/visit_struct.h>

struct Node
{
    int type;
    double value;
};

using Nodes = do_ast::Nodes_<Node>;

DO_AST_REGISTER_NODES(Nodes);

int main(int argc, char **argv)
{
    using Index = std::size_t;
    using Size = std::size_t;

    using Benchmark = do_ast::Benchmark<>;

    Benchmark benchmark;

    benchmark.begin("total");
    
    Nodes nodes;
    // nodes.enabled = true;
    // nodes.items.enabled = true;
    nodes.enabled = true;
    nodes.resize(10);
    std::cout << nodes.size() << std::endl;
    // nodes.items.enabled = false;

    nodes.clear();
    std::cout << nodes.size() << std::endl;

    nodes.num_children.enabled = true;

    auto a = nodes.insert(Node{0,0});
    auto b = nodes.insert(Node{0,0});
    auto c = nodes.insert(Node{1,0});
    nodes.set_children(a);
    nodes.set_children(b);
    nodes.set_children(c, a, b);

    auto Foo = [](auto... args)
    {
        do_ast::visit_args([](auto arg){
            std::cout << " " << arg;
        }, args...);
        std::cout << "\n";
    };

    benchmark.benchmark(5, "arg_template_helpers", [&Foo](){
        do_ast::drop_args<1>(Foo, 1, 2, 3);
        do_ast::reverse_args(Foo, 1, 2);
        do_ast::reverse_args(Foo, 1, 2, 3, 4, 5, 6);
        do_ast::take_args<4>(Foo, 1, 2, 3, 4, 5, 6);
        do_ast::visit_args_between<2,3>(Foo, 1, 2, 3, 4, 5, 6);
    });



    benchmark.end();

    Benchmark::Evaluation benchmark_eval;
    benchmark_eval.evaluate(benchmark);
    benchmark_eval.print();
    // benchmark.evaluate();

    return 0;
}
