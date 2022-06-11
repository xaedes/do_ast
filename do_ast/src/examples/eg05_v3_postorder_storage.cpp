#include <iostream>
#include <ostream>
#include <vector>
#include <string>
#include <cstdint>
#include <cassert>
#include <type_traits>

// store tree in post order 

// inspired by https://stackoverflow.com/a/28643465/798588 which suggests storing ordered by preorder for preorder recursion.
// during construction by code the items are in a natural postorder.
// by keeping track of up,down,prev,next and next_or_up pointer indices next_preorder indices can be computed for easy preorder traversal.
// during evaluation postorder storage is optimal: evaluation requires postorder traversal, so that evaluated arguments exist for evaluation of the 'current' node.
// preorder traversal is necessary for printing or more generally tree rewriting


template<class TType = int, class TValue = int>
struct TypeValue
{
    using Type = TType;
    using Value = TValue;
    Type type;
    Value value;
};

template<class TNode=TypeValue<>, class TNodeId = int64_t, class TDepth = uint32_t, class TSize = int64_t>
struct Nodes
{
    static_assert(std::is_integral<TNodeId>::value, "std::is_integral<TNodeId>::value");
    static_assert(std::is_integral<TDepth>::value, "std::is_integral<TDepth>::value");
    static_assert(std::is_integral<TSize>::value, "std::is_integral<TSize>::value");
    
    using Node = TNode;
    using NodeId = TNodeId;
    using Depth = TDepth;
    using Size = TSize;

    std::vector<Node> postorder;
    std::vector<Size> num_args;
    std::vector<Depth> depth;
    std::vector<NodeId> preorder;
    std::vector<NodeId> up;
    std::vector<NodeId> down;
    std::vector<NodeId> prev;
    std::vector<NodeId> next;
    std::vector<NodeId> next_or_up;
    std::vector<NodeId> next_preorder;
    std::vector<NodeId> skip_preorder; 
    
    Node& operator[](NodeId id) { return postorder[id]; }
    const Node& operator[](NodeId id) const { return postorder[id]; }
    
    NodeId root_id() const { return size() - 1; }
    Node& root() { return postorder.back(); }
    
    Size size() const { return postorder.size(); }
    template<class... Args>
    NodeId operator()(const Node& node, Args... args)
    {
        NodeId i = postorder.size();
        num_args.push_back(sizeof...(Args));
        depth.push_back(0);
        postorder.push_back(node);
        up.push_back(i);
        down.push_back(i);
        prev.push_back(i);
        next.push_back(i);
        next_or_up.push_back(i);
        next_preorder.push_back(i);
        skip_preorder.push_back(i);
        
        set_up(i, args...);
        set_down(i, args...);
        set_prev(i, args...);
        set_next(i, args...);
        
        m_next_preorder_valid = false;
        m_preorder_valid = false;
        m_depth_valid = false;
        return i;
    }
    
    bool is_next_preorder_valid() const { return m_next_preorder_valid; }
    bool is_preorder_valid() const { return m_preorder_valid; }
    bool is_depth_valid() const { return m_depth_valid; }
    
    void build()
    {
        build_next_preorder();
        build_preorder();
        build_depth();
    }
    
    void build_next_preorder()
    {
        build_next_preorder(root_id());
    }
    void build_next_preorder(NodeId root)
    {
        if (is_preorder_valid()) return;
        next_preorder.resize(size());
        NodeId last = root;
        NodeId here = root;
        for(Size ctr = 0; ctr < size(); ++ctr)
        {
            bool has_next = (next[here] != here);
            if (has_next)
            {
                skip_preorder[here] = next[here];
            }
            else
            {
                skip_preorder[here] = skip_preorder[up[here]];
            }
            if (num_args[here] > 0)
            {
                next_preorder[here] = down[here];
                here = down[here];
            }
            else
            {
                if(skip_preorder[here] == skip_preorder[root])
                {
                    // reached root
                    // i.e. no further iteration
                    skip_preorder[here] = here; 
                    next_preorder[here] = here;
                    break;
                }
                else
                {
                    next_preorder[here] = skip_preorder[here];
                    here = skip_preorder[here];
                }
            }
        }
        m_next_preorder_valid = true;
    }
    void build_preorder()
    {
        build_preorder(root_id());
    }
    
    void build_preorder(NodeId root)
    {
        if (is_preorder_valid()) return;
        if (!is_next_preorder_valid()) build_next_preorder();
        
        preorder.clear();
        NodeId current = root;
        for(Size ctr = 0; ctr < size(); ++ctr)
        {
            preorder.push_back(current);
            NodeId next = next_preorder[current];
            if (next == current)
            {
                break;
            }
            current = next;
        }
        assert(preorder.size() == size());
        m_preorder_valid = true;
    }
    
    void build_depth()
    {
        build_depth(root_id());
    }
    void build_depth(NodeId root)
    {
        if (is_depth_valid()) return;
        if (!is_next_preorder_valid()) build_next_preorder();
        
        depth.resize(size());
        // depth[root] = 0;
        NodeId current = root;
        for(Size ctr = 0; ctr < size(); ++ctr)
        {
            NodeId next = next_preorder[current];
            if (next == current)
            {
                break;
            }
            current = next;
            depth[current] = depth[up[current]]+1;
        }
        m_depth_valid = true;
    }

protected:
    
    bool m_next_preorder_valid = true;
    bool m_preorder_valid = true;
    bool m_depth_valid = true;
    
    void set_up(NodeId parent)
    {}
    template<class Arg, class... Args>
    void set_up(NodeId parent, Arg arg, Args... more)
    {
        up[arg] = parent;
        set_up(parent, more...);
    }
    
    void set_down(NodeId parent)
    {}
    template<class Arg, class... Args>
    void set_down(NodeId parent, Arg arg, Args... more)
    {
        down[parent] = arg;
    }
    
    void set_prev(NodeId parent)
    {}
    template<class Arg>
    void set_prev(NodeId previous, Arg arg)
    {
        prev[arg] = previous;
    }
    template<class Arg, class... Args>
    void set_prev(NodeId previous, Arg arg,  Args... more)
    {
        prev[arg] = previous;
        set_prev(arg, more...);
    }
    
    void set_next(NodeId parent)
    {}
    template<class Arg>
    void set_next(NodeId parent, Arg arg)
    {
        next_or_up[arg] = parent;
    }
    template<class Arg1, class Arg2, class... Args>
    void set_next(NodeId parent, Arg1 arg1,  Arg2 arg2, Args... more)
    {
        next[arg1] = arg2;
        next_or_up[arg1] = arg2;
        set_next(parent, arg2, more...);
    }
};

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

    using CalcNodes = Nodes<TypeValue<Type, double>>;
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
            auto node = nodes[idx];
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

int main() {
    Nodes<> n;
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
}
