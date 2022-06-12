#pragma once

namespace do_ast {

    template<class TType = int, class TValue = int>
    struct TypeValue
    {
        using Type = TType;
        using Value = TValue;
        Type type;
        Value value;
    };

} // namespace do_ast
