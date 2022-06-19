
#include <iostream>

#include <do_ast/v1.h>
#include <do_ast/item_pool.h>

int main(int argc, char **argv)
{
    using namespace do_ast::v1;

    Ast ast;

    auto a = ast.create_with_value(0, 0.5f);
    auto b = ast.create_with_value(0, 0.5f);
    auto c = ast.create_with_args(1, a, b);
    auto d = ast.create_with_value(0, true);
    auto e = ast.create_with_args(0, c, d);

    ast.visit(TreePrinterVisitor(), e);

    ast.erase_expr_recursive(c);

    ast.visit(TreePrinterVisitor(), e);

    auto f = ast.create_with_args(0, e);

    ast.visit(TreePrinterVisitor(), f);

    return 0;
}
