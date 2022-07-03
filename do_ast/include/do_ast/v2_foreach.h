#pragma once

namespace do_ast { 

    template<class T, class F, class I = typename T::Index>
    void foreach(const T& traversal, F func)
    {
        foreach(traversal, traversal.front(), func);
    }
    template<class T, class F, class I = typename T::Index>
    void foreach_until(const T& traversal, F func)
    {
        foreach_until(traversal, traversal.front(), func);
    }
    template<class T, class F, class I = typename T::Index>
    void foreach_skippable_until(const T& traversal, F func)
    {
        foreach_skippable_until(traversal, traversal.front(), func);
    }

    template<class T, class F, class I = typename T::Index>
    void foreach(const T& traversal, I begin, F func)
    {
        foreach(traversal.next(), begin, T::InvalidIndex(), func);
    }
    template<class T, class F, class I = typename T::Index>
    void foreach_until(const T& traversal, I begin, F func)
    {
        foreach_until(traversal.next(), begin, T::InvalidIndex(), func);
    }
    template<class T, class F, class I = typename T::Index>
    void foreach_skippable_until(const T& traversal, I begin, F func)
    {
        foreach_skippable_until(traversal.next(), traversal.skip(), begin, T::InvalidIndex(), func);
    }

    template<class I, class F>
    void foreach(const I* next, I begin, I invalid, F func)
    {
        I current = begin;
        while (current != invalid)
        {
            func(current);
            current = next[current];
        }
    }

    template<class I, class F>
    void foreach_until(const I* next, I begin, I invalid, F func)
    {
        I current = begin;
        while (current != invalid)
        {
            if (!func(current)) break;
            current = next[current];
        }
    }

    template<class I, class F>
    void foreach_skippable_until(const I* next, const I* skip, I begin, I invalid, F func)
    {
        I current = begin;
        while (current != invalid)
        {
            bool do_skip = false;
            if (!func(current, &do_skip)) break;
            current = do_skip ? skip[current] : next[current];
        }
    }

} // namespace do_ast

