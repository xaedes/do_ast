
#include <iostream>

#include <do_ast/do_ast.h>
#include <do_ast/item_pool.h>

int main(int argc, char **argv)
{
    using namespace do_ast;

    Ast ast;

    auto a = ast.create_with_value(0, 0.5f);
    auto b = ast.create_with_value(0, 0.5f);
    auto c = ast.create_with_args(1, a, b);
    auto d = ast.create_with_value(0, true);
    auto e = ast.create_with_args(0, c, d);

    ast.visit(e, TreePrinterVisitor());

    ast.erase_expr_recursive(c);

    ast.visit(e, TreePrinterVisitor());

    auto f = ast.create_with_args(0, e);

    ast.visit(f, TreePrinterVisitor());

    return 0;
}
