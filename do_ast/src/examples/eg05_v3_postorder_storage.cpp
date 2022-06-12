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

    Expr operator()(double value)
    {
        NodeId id = nodes(Node{Type::Val, value});
        return {this, id};
    }

    Expr add(Expr lhs, Expr rhs)
    {
        assert(lhs.calc == this);
        assert(rhs.calc == this);
        NodeId id = nodes(Node{Type::Add, initial_value}, lhs.id, rhs.id);
        return {this, id};
    }

    Expr sub(Expr lhs, Expr rhs)
    {
        assert(lhs.calc == this);
        assert(rhs.calc == this);
        NodeId id = nodes(Node{Type::Sub, initial_value}, lhs.id, rhs.id);
        return {this, id};
    }

    Expr mul(Expr lhs, Expr rhs)
    {
        assert(lhs.calc == this);
        assert(rhs.calc == this);
        NodeId id = nodes(Node{Type::Mul, initial_value}, lhs.id, rhs.id);
        return {this, id};
    }

    Expr div(Expr lhs, Expr rhs)
    {
        assert(lhs.calc == this);
        assert(rhs.calc == this);
        NodeId id = nodes(Node{Type::Div, initial_value}, lhs.id, rhs.id);
        return {this, id};
    }

    double eval()
    {
        static std::vector<double> stack;
        stack.clear();
        for(int idx=0; idx < nodes.size(); ++idx)
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
     n({1,0}, n({0,1}), n({0,2}));
    //n({2,0}, n({1,0}, n({0,1}), n({0,2})), n({0,3}));
    n.build();
    print_postorder(n);
    print_preorder(n);
    
    Calculator c;
    auto e = (c(0.1) + c(2.4)) * c(2.3);
    c.nodes.build();
    print_postorder(c.nodes);
    print_preorder(c.nodes);

    std::cout << c.eval() << "\n";
    std::cout << ((0.1) + (2.4)) * (2.3) << "\n";

    // std::vector<Operation> operations;
    // mk_reduction(1024*1024, operations);


    std::vector<double> values;

    // note: eg03 measured only add expressions in tight loop, but with scattered mem access
    
    values.resize(1024*1024);   //  eg03: 1.37246e+08 fps, eg05: 5.70746e+08 fps
    // values.resize(1024*128);    //  eg03: 2.85103e+08 fps, eg05: 6.2889e+08 fps
    // values.resize(1024*16);     //  eg03: 4.67046e+08 fps, eg05: 6.29774e+08 fps
    // values.resize(1024*4);      //  eg03: 5.71082e+08 fps, eg05: 6.18016e+08 fps
    // values.resize(1024*3);      //  eg03: 6.42956e+08 fps, eg05: 6.09944e+08 fps
    // values.resize(1024*2);      //  eg03: 7.05106e+08 fps <- biggest good, eg05:  6.25081e+08 fps
      // values.resize(1024);        //  eg03: 7.16236e+08 fps, eg05: 6.15694e+08 fps
    // values.resize(512);         //  eg03: 7.46908e+08 fps, eg05: 6.06257e+08 fps
    // values.resize(256);         //  eg03: 8.19056e+08 fps <- optimal, eg05: 6.07088e+08 fps
    // values.resize(128);         //  eg03: 7.87498e+08 fps, eg05: 6.1159e+08 fps


    // values.resize(128);
    // values.resize(1024*1024);
    for (int i = 0; i < values.size(); ++i)
    {
        values[i] = i;
    }

    // int num_it = 128;
    
    int num_it = 1024;
    // int num_it = 1024*8;
    // int num_it = 1024*8*8;
    // int num_it = 1024*8*8*4;
    // int num_it = 1024*8*8*4*2;
    // int num_it = 1024*8*8*4*4;
    // int num_it = 1024*8*8*4*4*2;
    // int num_it = 1024*8*8*4*4*2*2;
    // int num_it = 1024*8*8*4*4*2*2*2;
    // int num_it = 1024*8*8*4*4*8;

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
}
