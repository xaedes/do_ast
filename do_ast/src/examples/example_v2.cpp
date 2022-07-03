
#include <iostream>
#include <vector>
#include <iterator>
#include <type_traits>
#include <limits>
#include <chrono>

#include <do_ast/do_ast.h>
#include <do_ast/benchmark.h>
#include <do_ast/arg_template_helpers.h>
#include <do_ast/v2_nodes.h>
#include <do_ast/v2_node_forest.h>
#include <do_ast/v2_traverse_preorder.h>

// struct Nodes
// {
//     std::vector<int> m_types;
//     std::vector<int> m_values;
// };

using Nodes = do_ast::NodeForestV2_<do_ast::NodesV2_<int, int>>;

// DO_AST_REGISTER_NODES(Nodes);

int main(int argc, char **argv)
{
    using Index = std::size_t;
    using Size = std::size_t;

    using Benchmark = do_ast::Benchmark<>;

    Nodes nodes;

    Benchmark benchmark;

    benchmark.begin("total");
    
    #define BENCHMARK_ITERATIONS 1024
    // #define BENCHMARK_ITERATIONS 1024*128
    // #define BENCHMARK_NODES_COUNT 1024
    #define BENCHMARK_NODES_COUNT 1024*1024

    nodes.resize(BENCHMARK_NODES_COUNT);
    nodes.clear();

    benchmark.benchmark(BENCHMARK_ITERATIONS, "node_type[i] = i; node_value[i] = i", [&nodes](){
        nodes.clear();
        nodes.resize(BENCHMARK_NODES_COUNT);
        //Node node;
        auto* node_type = nodes.nodes().type();
        auto* node_value = nodes.nodes().value();
        for(int i = 0; i < BENCHMARK_NODES_COUNT; ++i)
        {
            node_type[i] = i;
            node_value[i] = i;
        }
    });
    benchmark.benchmark(BENCHMARK_ITERATIONS, "set_children", [&nodes](){
        nodes.clear();
        nodes.resize(BENCHMARK_NODES_COUNT);
        //Node node;
        auto* up = nodes.up();
        auto* down = nodes.down();
        auto* prev = nodes.prev();
        auto* next = nodes.next();
        auto* node_value = nodes.nodes().value();
        auto invalid_index = Nodes::InvalidIndex();
        for(Index i = 0; i < BENCHMARK_NODES_COUNT-1; ++i)
        {
            do_ast::set_children(up, down, prev, next, i, i+1);
            // node_type[i] = i;
            // node_value[i] = i;
        }
    });
    benchmark.benchmark(BENCHMARK_ITERATIONS, "set_children_fast", [&nodes](){
        nodes.clear();
        nodes.resize(BENCHMARK_NODES_COUNT);
        for(Index i = 0; i < BENCHMARK_NODES_COUNT-1; ++i)
        {
            nodes.set_children(i, i+1);
        }
    });
    benchmark.benchmark(BENCHMARK_ITERATIONS, "down[i] = ...", [&nodes](){
        nodes.clear();
        nodes.resize(BENCHMARK_NODES_COUNT);
        //Node node;
        auto* up = nodes.up();
        auto* down = nodes.down();
        auto* prev = nodes.prev();
        auto* next = nodes.next();
        auto* node_value = nodes.nodes().value();
        auto invalid_index = Nodes::InvalidIndex();
        for(Index i = 0; i < BENCHMARK_NODES_COUNT-1; ++i)
        {
            up[i] = i-1;
            down[i] = i+1;
            // prev[i] = invalid_index;
            // next[i] = invalid_index;
            // node_type[i] = i;
            // node_value[i] = i;
        }
    });

    benchmark.end("total");

    Benchmark::Evaluation benchmark_eval;
    benchmark_eval.evaluate(benchmark);
    benchmark_eval.print();
    // benchmark.evaluate();

    return 0;
}
