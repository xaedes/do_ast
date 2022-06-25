#pragma once

#include <type_traits>

namespace do_ast {

    struct visit_n_args_impl
    {
        template<int N, int K, class F>
        static void call(F f)
        {}
        
        template<
            int N, int K, class F, class Arg, class... Args,
            std::enable_if_t<(K < N), bool> = true
        >
        static void call(F f, Arg arg, Args... args)
        {
            f(arg);
            call<N, K+1>(f, args...);
        }
        
        template<
            int N, int K, class F, class... Args,
            std::enable_if_t<(K == N), bool> = true
        >
        static void call(F f, Args... args)
        {
        }
    };

    struct foldl_args_impl
    {
        template<int N, int K, class F>
        static void call(F f)
        {}

        template<int N, int K, class F, class Arg>
        static void call(F f, Arg arg)
        {}

        template<
            int N, int K, class F, 
            class Lhs, class Rhs, class... Args,
            std::enable_if_t<(K < N), bool> = true
        >
        static void call(F f, Lhs lhs, Rhs rhs, Args... args)
        {
            f(lhs, rhs);
            call<N,K+1>(f, rhs, args...);
        }

        template<
            int N, int K, class F, 
            class Lhs, class Rhs, class... Args,
            std::enable_if_t<(K == N), bool> = true
        >
        static void call(F f, Lhs lhs, Rhs rhs, Args... args)
        {}
    };

    template<int N, class F, class... Args>
    void visit_n_args(F f, Args... args)
    {
        visit_n_args_impl::call<N, 0>(f, args...);
    }

    template<class F, class... Args>
    void visit_args(F f, Args... args)
    {
        visit_n_args<sizeof...(Args)>(f, args...);
    }

    template<int N, class F, class... Args>
    void n_foldl_args(F f, Args... args)
    {
        foldl_args_impl::call<N,0>(f, args...);
    }

    template<class F, class... Args>
    void foldl_args(F f, Args... args)
    {
        n_foldl_args<sizeof...(Args)>(f, args...);
    }


} // namespace do_ast
