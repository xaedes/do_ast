#include <iostream>
#include <ostream>
#include <vector>
#include <string>
#include <cstdint>
#include <cassert>
#include <type_traits>
#include <chrono>

#include "mk_reduction.h"
#include <do_ast/nodes_postorder.h>


struct Calculator
{
    enum class Type
    {
        Val,
        Add,
        Sub,
        Mul,
        Div
    };

    using CalcNodes = do_ast::NodesPostorder<do_ast::TypeValue<Type, double>>;
    using Node = typename CalcNodes::Node;
    using NodeId = typename CalcNodes::NodeId;

    struct Expr
    {
        Calculator* calc;
        NodeId id;

        Expr operator+(Expr rhs)
        {
            return calc->add(*this, rhs);
        }
        Expr operator-(Expr rhs)
        {
            return calc->sub(*this, rhs);
        }
        Expr operator*(Expr rhs)
        {
            return calc->mul(*this, rhs);
        }
        Expr operator/(Expr rhs)
        {
            return calc->div(*this, rhs);
        }
    };

    CalcNodes nodes;
    double initial_value = 0;

    void clear()
    {
        nodes.clear();
    }

    Expr operator()(double val)
    {
        return value(val);
    }

    Expr value(double val)
    {
        NodeId id = nodes.add_node(Node{Type::Val, val});
        return {this, id};
    }

    Expr add(Expr lhs, Expr rhs)
    {
        assert(lhs.calc == this);
        assert(rhs.calc == this);
        NodeId id = nodes.add_node(Node{Type::Add, initial_value}, lhs.id, rhs.id);
        return {this, id};
    }

    Expr sub(Expr lhs, Expr rhs)
    {
        assert(lhs.calc == this);
        assert(rhs.calc == this);
        NodeId id = nodes.add_node(Node{Type::Sub, initial_value}, lhs.id, rhs.id);
        return {this, id};
    }

    Expr mul(Expr lhs, Expr rhs)
    {
        assert(lhs.calc == this);
        assert(rhs.calc == this);
        NodeId id = nodes.add_node(Node{Type::Mul, initial_value}, lhs.id, rhs.id);
        return {this, id};
    }

    Expr div(Expr lhs, Expr rhs)
    {
        assert(lhs.calc == this);
        assert(rhs.calc == this);
        NodeId id = nodes.add_node(Node{Type::Div, initial_value}, lhs.id, rhs.id);
        return {this, id};
    }

    double eval()
    {
        static std::vector<double> stack;
        stack.clear();
        auto nsize = nodes.size();
        for(int idx=0; idx < nsize; ++idx)
        {
            const auto& node = nodes[idx];
            switch(node.type)
            {
                case Calculator::Type::Val: 
                    stack.push_back(node.value); 
                    break;
                case Calculator::Type::Add: 
                    stack[stack.size()-2] = (
                        stack[stack.size()-2]
                      + stack[stack.size()-1]
                    );
                    stack.pop_back(); 
                    break;
                case Calculator::Type::Sub: 
                    stack[stack.size()-2] = (
                        stack[stack.size()-2]
                      - stack[stack.size()-1]
                    );
                    stack.pop_back(); 
                    break;
                case Calculator::Type::Mul: 
                    stack[stack.size()-2] = (
                        stack[stack.size()-2]
                      * stack[stack.size()-1]
                    );
                    stack.pop_back(); 
                    break;
                case Calculator::Type::Div: 
                    stack[stack.size()-2] = (
                        stack[stack.size()-2]
                      / stack[stack.size()-1]
                    );
                    stack.pop_back(); 
                    break;
            }
        }
        assert(stack.size() == 1);
        return stack.back();

    }

    Expr simplify(Calculator& out)
    {
        return simplify(out, nodes.root_id());
    }
    Expr simplify(Calculator& out, NodeId id)
    {
        assert(&out != this);
        const auto& node = nodes[id];
        switch(node.type)
        {
            case Calculator::Type::Val: 
            {
                return out(node.value);
            }
            case Calculator::Type::Add: 
            {
                auto arg1 = nodes.down[id];
                auto arg2 = nodes.next[arg1];
                const auto& n1 = nodes[arg1];
                const auto& n2 = nodes[arg2];
                bool n1_is_zero = (
                    (n1.type == Calculator::Type::Val)
                 && (n1.value == 0)
                );
                bool n2_is_zero = (
                    (n2.type == Calculator::Type::Val)
                 && (n2.value == 0)
                );
                if (n1_is_zero && n2_is_zero)
                {
                    return out(0);
                }
                else if (n1_is_zero && !n2_is_zero)
                {
                    return simplify(out, arg2);
                }
                else if (!n1_is_zero && n2_is_zero)
                {
                    return simplify(out, arg1);
                }
                else
                {
                    auto o1 = simplify(out, arg1);
                    auto o2 = simplify(out, arg2);
                    return out.add(o1, o2);
                }
                break;
            }
            case Calculator::Type::Sub: 
            {
                auto arg1 = nodes.down[id];
                auto arg2 = nodes.next[arg1];
                const auto& n1 = nodes[arg1];
                const auto& n2 = nodes[arg2];
                bool n1_is_zero = (
                    (n1.type == Calculator::Type::Val)
                 && (n1.value == 0)
                );
                bool n2_is_zero = (
                    (n2.type == Calculator::Type::Val)
                 && (n2.value == 0)
                );
                if (n1_is_zero && n2_is_zero)
                {
                    return out(0);
                }
                else if (!n1_is_zero && n2_is_zero)
                {
                    return simplify(out, arg1);
                }
                else
                {
                    auto o1 = simplify(out, arg1);
                    auto o2 = simplify(out, arg2);
                    return out.sub(o1, o2);
                }
                break;
            }
            case Calculator::Type::Mul: 
            {
                auto arg1 = nodes.down[id];
                auto arg2 = nodes.next[arg1];
                const auto& n1 = nodes[arg1];
                const auto& n2 = nodes[arg2];
                bool n1_is_zero = (
                    (n1.type == Calculator::Type::Val)
                 && (n1.value == 0)
                );
                bool n2_is_zero = (
                    (n2.type == Calculator::Type::Val)
                 && (n2.value == 0)
                );
                bool n1_is_one = (
                    (n1.type == Calculator::Type::Val)
                 && (n1.value == 1)
                );
                bool n2_is_one = (
                    (n2.type == Calculator::Type::Val)
                 && (n2.value == 1)
                );
                bool any_zero = (n1_is_zero || n2_is_zero);

                if (any_zero)
                {
                    return out(0);
                }
                else if (n1_is_one && n2_is_one)
                {
                    return out(1);
                }
                else if (n1_is_one && !n2_is_one)
                {
                    return simplify(out, arg2);
                }
                else if (!n1_is_one && n2_is_one)
                {
                    return simplify(out, arg1);
                }
                else
                {
                    auto o1 = simplify(out, arg1);
                    auto o2 = simplify(out, arg2);
                    return out.mul(o1, o2);
                }
                break;
            }
            case Calculator::Type::Div: 
            {
                auto arg1 = nodes.down[id];
                auto arg2 = nodes.next[arg1];
                const auto& n1 = nodes[arg1];
                const auto& n2 = nodes[arg2];
                bool n1_is_one = (
                    (n1.type == Calculator::Type::Val)
                 && (n1.value == 1)
                );
                bool n2_is_one = (
                    (n2.type == Calculator::Type::Val)
                 && (n2.value == 1)
                );
                if (n1_is_one && n2_is_one)
                {
                    return out(1);
                }
                else if (n2_is_one)
                {
                    return simplify(out, arg1);
                }
                else
                {
                    auto o1 = simplify(out, arg1);
                    auto o2 = simplify(out, arg2);
                    return out.div(o1, o2);
                }
                break;
            }
        }
    }
};

std::ostream& operator<<(std::ostream& os, const Calculator::Type& t)
{
    switch(t)
    {
        case Calculator::Type::Val: return (os << "Val");
        case Calculator::Type::Add: return (os << "Add");
        case Calculator::Type::Sub: return (os << "Sub");
        case Calculator::Type::Mul: return (os << "Mul");
        case Calculator::Type::Div: return (os << "Div");
    }
    return os;
}
template<class TNodes>
void print_postorder(const TNodes& nodes)
{
    for(int idx=0; idx < nodes.size(); ++idx)
    {
        auto node = nodes[idx];
        auto nargs = nodes.num_args[idx];
        std::cout << "#:" << idx << " ";
        std::cout << "t:" << node.type << " ";
        std::cout << "v:" << node.value << " ";
        std::cout << "a:" << nargs << " ";
        std::cout << "n:" << nodes.next[idx] << " ";
        std::cout << "u:" << nodes.up[idx] << " ";
        std::cout << "d:" << nodes.down[idx] << " ";
        //std::cout << "nu:" << nodes.next_or_up[idx] << " ";
        std::cout << "nx:" << nodes.next_preorder[idx] << " ";
        std::cout << "\n";
    }
    std::cout << "---\n";
}

template<class TNodes>
void print_preorder(const TNodes& nodes, const std::string& indent="  ")
{

    for(int idx=0; idx < nodes.size(); ++idx)
    {
        auto nidx = nodes.preorder[idx];
        auto node = nodes[nidx];
        auto nargs = nodes.num_args[nidx];
        auto depth = nodes.depth[nidx];
        for (auto i=0; i<depth; ++i)
        {
            std::cout << indent;
        }
        std::cout << "d:" << depth << " ";
        std::cout << "t:" << node.type << " ";
        std::cout << "v:" << node.value << " ";
        std::cout << "a:" << nargs << "\n";
    }
    std::cout << "---\n";
}

Calculator::Expr recursiveDeepAdd(
    Calculator& calc, 
    const double* begin, 
    const double* end
)
{
    auto count = std::distance(begin, end);
    assert(count > 0);
    if (count == 1)
    {
        return calc(*begin);
    }
    else
    {
        auto mid = begin + count / 2;
        auto lhs = recursiveDeepAdd(calc, begin, mid);
        auto rhs = recursiveDeepAdd(calc, mid, end);
        return calc.add(lhs, rhs);
    }
}

int main() {
    std::cout << "size(Calculator::Node) " << sizeof(Calculator::Node) << "\n";
    do_ast::NodesPostorder<> n;
    {
        auto a = n.add_node({0,1});
        auto b = n.add_node({0,2});
        auto c = n.add_node({1,0}, a, b);
    }
    //n({2,0}, n({1,0}, n({0,1}), n({0,2})), n({0,3}));
    n.build();
    print_postorder(n);
    print_preorder(n);
    
    Calculator calc;
    {
        // create in post order
        auto a = calc(0.1);
        auto b = calc(2.4);
        auto c = a + b;
        auto d = calc(2.3);
        auto e = c * d;
    }
    calc.nodes.build();
    print_postorder(calc.nodes);
    print_preorder(calc.nodes);

    std::cout << calc.eval() << "\n";
    std::cout << ((0.1) + (2.4)) * (2.3) << "\n";

    // std::vector<Operation> operations;
    // mk_reduction(1024*1024, operations);


    std::vector<double> values;

    // note: eg03 measured only add expressions in tight loop, but with scattered mem access
    
    // values.resize(1024*1024);   //  eg03: 1.37246e+08 fps, eg05: 5.70746e+08 fps
    // values.resize(1024*128);    //  eg03: 2.85103e+08 fps, eg05: 6.2889e+08 fps
    // values.resize(1024*16);     //  eg03: 4.67046e+08 fps, eg05: 6.29774e+08 fps
    // values.resize(1024*4);      //  eg03: 5.71082e+08 fps, eg05: 6.18016e+08 fps
    // values.resize(1024*3);      //  eg03: 6.42956e+08 fps, eg05: 6.09944e+08 fps
    // values.resize(1024*2);      //  eg03: 7.05106e+08 fps <- biggest good, eg05:  6.25081e+08 fps
      // values.resize(1024);        //  eg03: 7.16236e+08 fps, eg05: 6.15694e+08 fps
    // values.resize(512);         //  eg03: 7.46908e+08 fps, eg05: 6.06257e+08 fps
    // values.resize(256);         //  eg03: 8.19056e+08 fps <- optimal, eg05: 6.07088e+08 fps
    values.resize(128);         //  eg03: 7.87498e+08 fps, eg05: 6.1159e+08 fps


    // values.resize(128);
    // values.resize(1024*1024);
    for (int i = 0; i < values.size(); ++i)
    {
        values[i] = i;
    }

    // int num_it = 128;
    
    // int num_it = 1024;
    // int num_it = 1024*8;
    // int num_it = 1024*8*8;
    // int num_it = 1024*8*8*4;
    // int num_it = 1024*8*8*4*2;
    // int num_it = 1024*8*8*4*4;
    // int num_it = 1024*8*8*4*4*2;
    // int num_it = 1024*8*8*4*4*2*2;
    // int num_it = 1024*8*8*4*4*2*2*2;
    int num_it = 1024*8*8*4*4*8;

    Calculator clongadd;
    auto longadd = recursiveDeepAdd(clongadd, values.data(), values.data() + values.size());
    clongadd.nodes.build();
    // print_postorder(clongadd.nodes);
    // print_preorder(clongadd.nodes);

    std::cout << clongadd.eval() << "\n";

    auto t0 = std::chrono::system_clock::now();
    double sum0 = 0;
    for (int i=0; i<num_it; ++i)
    {
        sum0 += clongadd.eval();
    }
    auto t1 = std::chrono::system_clock::now();

    double dnorm = static_cast<double>(clongadd.nodes.size()*num_it); 
    // double dnorm = static_cast<double>(num_it); 
    std::chrono::duration<double> d0 = t1-t0;
    // std::chrono::duration<double> d1 = t2-t1;
    // std::chrono::duration<double> d2 = t3-t2;
    // std::chrono::duration<double> d3 = t4-t3;
    double fps0 = abs(d0.count()) > 1e-12 ? (dnorm / d0.count()) : 0;
    // double fps1 = abs(d1.count()) > 1e-12 ? (dnorm / d1.count()) : 0;
    // double fps2 = abs(d2.count()) > 1e-12 ? (dnorm / d2.count()) : 0;
    // double fps3 = abs(d3.count()) > 1e-12 ? (dnorm / d3.count()) : 0;
    std::cout << "phase0: " << (d0.count() / dnorm) * 1000 << " ms " << fps0 << " fps\n";
    // std::cout << "phase1: " << (d1.count() / dnorm) * 1000 << " ms " << fps1 << " fps\n";
    // std::cout << "phase2: " << (d2.count() / dnorm) * 1000 << " ms " << fps2 << " fps\n";
    // std::cout << "phase3: " << (d3.count() / dnorm) * 1000 << " ms " << fps3 << " fps\n";
    std::cout << " sum0 " << sum0 << "\n";
    // std::cout << " sum1 " << sum1 << "\n";
    // std::cout << " sum2 " << sum2 << "\n";
    // std::cout << " sum3 " << sum3 << "\n";

    std::cout << "---" << "\n";

    calc.clear();
    {
        // create in post order
        // e = (calc(0) + (calc(2.5)/calc(1))) * calc(1) + calc(3);
        auto a = calc(0);
        auto b = calc(2.5);
        auto c = calc(1);
        auto d = b / c;
        auto e = a + d;
        auto f = calc(1);
        auto g = e * f;
        auto h = calc(3);
        auto i = g + h;
    }
    calc.nodes.build();
    print_preorder(calc.nodes);

    Calculator csimple;
    auto simple = calc.simplify(csimple, calc.nodes.root_id());
    csimple.nodes.build();
    print_preorder(csimple.nodes);
}
