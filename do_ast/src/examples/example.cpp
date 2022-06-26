
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
    int value;
};

using Nodes = do_ast::Nodes_<Node>;

// DO_AST_REGISTER_NODES(Nodes);

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
    // nodes.enabled = true;
    nodes.resize(10);
    std::cout << nodes.size() << std::endl;
    // nodes.items.enabled = false;

    nodes.clear();
    std::cout << nodes.size() << std::endl;

    // nodes.num_children.enabled = true;

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
    #define BENCHMARK_ITERATIONS 1024
    // #define BENCHMARK_ITERATIONS 1024*128
    // #define BENCHMARK_NODES_COUNT 1024
    #define BENCHMARK_NODES_COUNT 1024*1024
    nodes.reserve(BENCHMARK_NODES_COUNT);
    nodes.clear();
    benchmark.benchmark(BENCHMARK_ITERATIONS, "nodes.insert(Node{i,i+0.0})", [&nodes](){
        nodes.clear();
        for(int i = 0; i < BENCHMARK_NODES_COUNT; ++i)
        {
            nodes.insert(Node{i,i+0});
        }
    });
    benchmark.benchmark(BENCHMARK_ITERATIONS, "nodes.insert_fast(Node{i,i+0})", [&nodes](){
        nodes.clear();
        for(int i = 0; i < BENCHMARK_NODES_COUNT; ++i)
        {
            nodes.insert_fast(Node{i,i+0});
        }
    });
    benchmark.benchmark(BENCHMARK_ITERATIONS, "nodes.insert()", [&nodes](){
        nodes.clear();
        for(int i = 0; i < BENCHMARK_NODES_COUNT; ++i)
        {
            nodes.insert();
        }
    });
    benchmark.benchmark(BENCHMARK_ITERATIONS, "nodes.insert_fast()", [&nodes](){
        nodes.clear();
        for(int i = 0; i < BENCHMARK_NODES_COUNT; ++i)
        {
            nodes.insert_fast();
        }
    });
    benchmark.benchmark(BENCHMARK_ITERATIONS, "nodes.insert(node)", [&nodes](){
        nodes.clear();
        Node node;
        for(int i = 0; i < BENCHMARK_NODES_COUNT; ++i)
        {
            nodes.insert(node);
        }
    });
    benchmark.benchmark(BENCHMARK_ITERATIONS, "nodes.insert_fast(node)", [&nodes](){
        nodes.clear();
        Node node;
        for(int i = 0; i < BENCHMARK_NODES_COUNT; ++i)
        {
            nodes.insert_fast(node);
        }
    });
    benchmark.benchmark(BENCHMARK_ITERATIONS, "ptr[i] = node", [&nodes](){
        nodes.resize(BENCHMARK_NODES_COUNT);
        Node node;
        Node* ptr = &nodes.node(0);
        for(int i = 0; i < BENCHMARK_NODES_COUNT; ++i)
        {
            ptr[i] = node;
        }
    });
    benchmark.benchmark(BENCHMARK_ITERATIONS, "ptr[i] = Node{i,i+0}", [&nodes](){
        nodes.resize(BENCHMARK_NODES_COUNT);
        Node* ptr = &nodes.node(0);
        for(int i = 0; i < BENCHMARK_NODES_COUNT; ++i)
        {
            ptr[i] = Node{i,i+0};
        }
    });
    benchmark.benchmark(BENCHMARK_ITERATIONS, "ptr[i].xyz = xyz", [&nodes](){
        nodes.resize(BENCHMARK_NODES_COUNT);
        Node* ptr = &nodes.node(0);
        for(int i = 0; i < BENCHMARK_NODES_COUNT; ++i)
        {
            ptr[i].type = i;
            ptr[i].value = i + 0;
        }
    });
    std::vector<int> node_types;
    std::vector<int> node_values;
    node_types.reserve(BENCHMARK_NODES_COUNT);
    node_values.reserve(BENCHMARK_NODES_COUNT);

    benchmark.benchmark(BENCHMARK_ITERATIONS, "xyz_ptr[i] = xyz", [&node_types, &node_values](){
        node_types.resize(BENCHMARK_NODES_COUNT);
        node_values.resize(BENCHMARK_NODES_COUNT);
        auto* types_ptr = node_types.data();
        auto* values_ptr = node_values.data();
        for(int i = 0; i < BENCHMARK_NODES_COUNT; ++i)
        {
            types_ptr[i] = i;
            values_ptr[i] = i + 0;
        }
    });

    benchmark.end("total");

    Benchmark::Evaluation benchmark_eval;
    benchmark_eval.evaluate(benchmark);
    benchmark_eval.print();
    // benchmark.evaluate();

    return 0;
}
