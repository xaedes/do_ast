#pragma once

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
#include <do_ast/v2_node_type_value_inserter.h>
#include <do_ast/v2_traverse_preorder.h>
#include <do_ast/v2_traverse_postorder.h>
#include <do_ast/v2_foreach.h>

using Forest = do_ast::NodeForestV2_<do_ast::NodesV2_<int, int>>;


int main(int argc, char **argv)
{
    using Index = Forest::Index;
    using Size = Forest::Size;
    using NodeType = Forest::Nodes::NodeType;
    using NodeValue = Forest::Nodes::NodeValue;
    using Benchmark = do_ast::Benchmark<>;
    Benchmark benchmark;
    benchmark.begin("total");
    
    Forest forest;
    auto& nodes = forest.nodes();
    forest.resize(1024);
    
    do_ast::NodeTypeValueInserter<Forest> insert_node(&forest);

    auto insert_null = [&insert_node]() { return insert_node(0, 0); };
    auto insert_add = [&insert_node](auto... children) { return insert_node(1, 0, children...); };
    auto insert_val = [&insert_node](auto val) { return insert_node(2, val); };

    auto root = insert_add(
        insert_add(
            insert_val(0),
            insert_val(1)
        ),
        insert_add(
            insert_val(2),
            insert_val(3)
        ),
        insert_add(
            insert_val(4),
            insert_val(5)
        ),
        insert_val(6)
    );

    NodeType* types = nodes.type();
    NodeValue* values = nodes.value();

    std::cout << "---" << "\n";
    std::cout << "TraversePreorder" << "\n";

    do_ast::TraversePreorder<Forest> traverse_preorder;
    traverse_preorder.build(forest, root);
    do_ast::foreach(traverse_preorder, [types, values](auto nid){
        std::cout << "#:" << nid << " t:" << types[nid] << " v:" << values[nid] << "\n";
    });

    std::cout << "---" << "\n";
    std::cout << "TraversePostorder" << "\n";

    do_ast::TraversePostorder<Forest> traverse_postorder;
    traverse_postorder.build(forest, root);
    do_ast::foreach(traverse_postorder, [types, values](auto nid){
        std::cout << "#:" << nid << " t:" << types[nid] << " v:" << values[nid] << "\n";
    });

    std::cout << "---" << "\n";

    benchmark.end("total");
    Benchmark::Evaluation benchmark_eval;
    benchmark_eval.evaluate(benchmark);
    benchmark_eval.print();
    return 0;
}
