
#include <iostream>
#include <vector>
#include <iterator>
#include <type_traits>
#include <limits>

#include <do_ast/do_ast.h>
#include <do_ast/arg_template_helpers.h>
#include <do_ast/nodes.h>
#include <do_ast/visit_struct/visit_struct.h>

struct Node
{
    int type;
    double value;
};

using Nodes = Nodes_<Node>;

DO_AST_REGISTER_NODES(Nodes);

int main(int argc, char **argv)
{
    using Index = std::size_t;
    using Size = std::size_t;
    
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

    return 0;
}
