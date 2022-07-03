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
#include <do_ast/v2_node_depth.h>
#include <do_ast/v2_foreach.h>
#include <do_ast/v2_reorder.h>
#include <do_ast/v2_index_mapping.h>

using Forest = do_ast::NodeForestV2_<do_ast::NodesV2_<int, int>>;
using Index = Forest::Index;
using Size = Forest::Size;
using NodeType = Forest::Nodes::NodeType;
using NodeValue = Forest::Nodes::NodeValue;
using Benchmark = do_ast::Benchmark<>;
using IndexMapping = do_ast::IndexMapping<Index>;

void print_preorder(
    Forest& forest,
    do_ast::TraversePreorder<Forest>& traverse_preorder,
    do_ast::NodeDepth<Forest>& node_depth,
    Index root,
    bool rebuild = true
)
{
    auto& nodes = forest.nodes();
    NodeType* types = nodes.type();
    NodeValue* values = nodes.value();
    if (rebuild)
    {
        traverse_preorder.build(forest, root);
        node_depth.build(forest, traverse_preorder);
    }
    const auto* depth = node_depth.depth();
    auto max_depth = node_depth.max_depth();
    do_ast::foreach(traverse_preorder, [types, values, depth, max_depth](auto nid){
        auto d = depth[nid];
        for (decltype(d) i=0; i<d; ++i)
        {
            std::cout << " ";
        }
        std::cout << "*";
        for (auto i=d; i<max_depth; ++i)
        {
            std::cout << " ";
        }
        std::cout << " ";
        std::cout << "d:" << depth[nid] << " #:" << nid << " t:" << types[nid] << " v:" << values[nid] << "\n";
    });
}

void print_postorder(
    Forest& forest,
    do_ast::TraversePostorder<Forest>& traverse_postorder,
    Index root,
    bool rebuild = true
)
{
    auto& nodes = forest.nodes();
    NodeType* types = nodes.type();
    NodeValue* values = nodes.value();
    if (rebuild)
    {
        traverse_postorder.build(forest, root);
    }
    do_ast::foreach(traverse_postorder, [types, values](auto nid){
        std::cout << "#:" << nid << " t:" << types[nid] << " v:" << values[nid] << "\n";
    });

}

int main(int argc, char **argv)
{
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
    std::cout << "print_preorder" << "\n";

    do_ast::TraversePreorder<Forest> traverse_preorder;
    do_ast::NodeDepth<Forest> node_depth;
    print_preorder(
        forest, 
        traverse_preorder, 
        node_depth, 
        root 
    );

    std::cout << "---" << "\n";
    std::cout << "print_postorder" << "\n";

    do_ast::TraversePostorder<Forest> traverse_postorder;
    print_postorder(
        forest, 
        traverse_postorder, 
        root 
    );

    std::cout << "---" << "\n";
    std::cout << "do_ast::make_reorder" << "\n";
    IndexMapping map_from_preorder;
    IndexMapping map_from_postorder;
    IndexMapping map_to_preorder;
    IndexMapping map_to_postorder;
    do_ast::make_reorder(traverse_preorder, map_from_preorder, map_to_preorder);
    do_ast::make_reorder(traverse_postorder, map_from_postorder, map_to_postorder);

    std::cout << "---" << "\n";
    std::cout << "do_ast::reorder" << "\n";

    Forest forest_preorder;
    Forest forest_postorder;
    do_ast::reorder(forest, map_from_preorder, map_to_preorder, forest_preorder);
    do_ast::reorder(forest, map_from_postorder, map_to_postorder, forest_postorder);

    do_ast::TraversePreorder<Forest> preordered_traverse_preorder;
    do_ast::TraversePostorder<Forest> preordered_traverse_postorder;
    do_ast::NodeDepth<Forest> preordered_node_depth;
    do_ast::TraversePreorder<Forest> postordered_traverse_preorder;
    do_ast::TraversePostorder<Forest> postordered_traverse_postorder;
    do_ast::NodeDepth<Forest> postordered_node_depth;

    do_ast::reorder(traverse_preorder, map_from_preorder, map_to_preorder, preordered_traverse_preorder);
    do_ast::reorder(traverse_postorder, map_from_preorder, map_to_preorder, preordered_traverse_postorder);
    do_ast::reorder(node_depth, map_from_preorder, map_to_preorder, preordered_node_depth);

    do_ast::reorder(traverse_preorder, map_from_postorder, map_to_postorder, postordered_traverse_preorder);
    do_ast::reorder(traverse_postorder, map_from_postorder, map_to_postorder, postordered_traverse_postorder);
    do_ast::reorder(node_depth, map_from_postorder, map_to_postorder, postordered_node_depth);
    
    std::cout << "---" << "\n";
    std::cout << "print preordered" << "\n";
    print_preorder(
        forest_preorder, 
        preordered_traverse_preorder, 
        preordered_node_depth, 
        map_to_preorder.map()[root],
        false
    );
    std::cout << "---" << "\n";
    print_postorder(
        forest_preorder, 
        preordered_traverse_postorder, 
        map_to_preorder.map()[root],
        false
    );

    std::cout << "---" << "\n";
    std::cout << "print postordered" << "\n";
    print_preorder(
        forest_postorder, 
        postordered_traverse_preorder, 
        postordered_node_depth, 
        map_to_postorder.map()[root],
        false
    );
    std::cout << "---" << "\n";
    print_postorder(
        forest_postorder, 
        postordered_traverse_postorder, 
        map_to_postorder.map()[root],
        false
    );


    benchmark.end("total");
    Benchmark::Evaluation benchmark_eval;
    benchmark_eval.evaluate(benchmark);
    benchmark_eval.print();
    return 0;
}
