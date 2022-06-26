#pragma once

#include <do_ast/arg_template_helpers.h>

namespace do_ast {

    struct drop_args_impl
    {
        template<int D, class F>
        static void call(F f)
        {
            f();
        }

        template<
            int D, class F, class Arg, class... Args,
            std::enable_if_t<(D > 0), bool> = true
        >
        static void call(F f, Arg arg, Args... args)
        {
            call<D-1>(f, args...);
        }
        
        template<
            int D, class F, class... Args,
            std::enable_if_t<(D == 0), bool> = true
        >
        static void call(F f, Args... args)
        {
            f(args...);
        }
    };

    struct reverse_args_impl
    {
        template<class F>
        static void call(F f)
        {
            f();
        }

        template<
            class F, class Arg, class... Args
        >
        static void call(F f, Arg arg, Args... args)
        {
            reverse_args([&f,&arg](auto... rargs){
                f(rargs..., arg);
            }, args...);
        }
    };

    struct take_args_impl
    {
        template<int N, class F>
        static void call(F f)
        {
            f();
        }

        template<
            int N, class F, class... Args
        >
        static void call(F f, Args... args)
        {
            reverse_args(
                [&f](auto... rargs)
                {
                    drop_args<sizeof...(Args) - N>(
                        [&f](auto... rargs_dropped)
                        {
                            reverse_args(
                                [&f](auto... result)
                                {
                                    f(result...);
                                }, 
                                rargs_dropped...
                            );
                        }, 
                        rargs...
                    );
                }, 
                args...
            );
        }
        
    };

    struct visit_args_impl
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

    template<int N, class... Args>
    struct get_nth_arg_impl
    {
        using result_type = std::tuple_element_t<N, std::tuple<Args...>>;

        template<
            int I, class Arg, class... Rest,
            std::enable_if_t<(I == N), bool> = true
        >
        result_type call(Arg arg, Rest... rest)
        {
            return arg;
        }
        template<
            int I, class Arg, class... Rest,
            std::enable_if_t<(I < N), bool> = true
        >
        result_type call(Arg arg, Rest... rest)
        {
            return call<I+1>(rest...);
        }
    };

    template<int D, class F, class... Args>
    void drop_args(F f, Args... args)
    {
        drop_args_impl::call<D>(f, args...);
    }

    template<int N, class F, class... Args>
    void take_args(F f, Args... args)
    {
        take_args_impl::call<N>(f, args...);
    }

    template<class F, class... Args>
    void reverse_args(F f, Args... args)
    {
        reverse_args_impl::call(f, args...);
    }

    template<int N, class F, class... Args>
    void visit_n_args(F f, Args... args)
    {
        visit_args_impl::call<N, 0>(f, args...);
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

    template<int I, class... Args>
    std::tuple_element_t<I, std::tuple<Args...>>
    get_nth_arg(Args... args)
    {
        return get_nth_arg_impl<I, Args...>::call<0>(args...);
    }
    
} // namespace do_ast
