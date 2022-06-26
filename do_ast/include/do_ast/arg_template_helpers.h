#pragma once

#include <type_traits>
#include <tuple>


namespace do_ast {


    template<int D, class F, class... Args>
    void drop_args(F f, Args... args);

    template<int N, class F, class... Args>
    void take_args(F f, Args... args);

    template<class F, class... Args>
    void reverse_args(F f, Args... args);

    template<int N, class F, class... Args>
    void visit_n_args(F f, Args... args);

    template<class F, class... Args>
    void visit_args(F f, Args... args);

    template<int N, class F, class... Args>
    void n_foldl_args(F f, Args... args);

    template<class F, class... Args>
    void foldl_args(F f, Args... args);

    template<int I, class... Args>
    std::tuple_element_t<I, std::tuple<Args...>>
    get_nth_arg(Args... args);

} // namespace do_ast

#include <do_ast/arg_template_helpers.impl.h>
