#pragma once

#include <vector>
#include <cassert>
#include <do_ast/v2_index_mapping.h>

namespace do_ast {

    template<
        class TTraversal,
        class I1, class S1,
        class I2, class S2,
        template<class...> class C1,
        template<class...> class C2
    >
    void make_reorder(
        const TTraversal& traversal, 
        IndexMapping<I1,S1,C1>& map_from_ordered,
        IndexMapping<I2,S2,C2>& map_to_ordered
    )
    {
        map_from_ordered.resize(traversal.count());
        map_to_ordered.resize(traversal.size());
        auto* from_ordered = map_from_ordered.map();
        auto* to_ordered  = map_to_ordered.map();
        const auto invalid = TTraversal::InvalidIndex();
        auto current = traversal.front();
        const auto* next = traversal.next();
        I2 new_idx = 0;
        while (current != invalid)
        {
            from_ordered[new_idx] = current;
            to_ordered[current] = new_idx;
            ++new_idx;
            current = next[current];
        }
        map_from_ordered.resize(new_idx);
    }

    template<
        class TTraversal,
        class I1, class S1,
        template<class...> class C1
    >
    void make_reorder(
        const TTraversal& traversal, 
        IndexMapping<I1,S1,C1>& map_from_ordered
    )
    {
        map_from_ordered.resize(traversal.count());
        TIndex* from_ordered = map_from_ordered.map();
        const auto invalid = TTraversal::InvalidIndex();
        auto current = traversal.front();
        const auto* next = traversal.next();
        I1 new_idx = 0;
        while (current != invalid)
        {
            from_ordered[new_idx] = current;
            ++new_idx;
            current = next[current];
        }
        map_from_ordered.resize(new_idx);
    }

    template<
        class T1, class V1, class S1,
        class I2, class S2,
        class T3, class V3, class S3,
        template<class...> class C1,
        template<class...> class C2,
        template<class...> class C3
    >
    void reorder(
        const NodesV2_<T1,V1,S1,C1>& items,
        const IndexMapping<I2,S2,C2>& map_from_ordered,
        NodesV2_<T3,V3,S3,C3>& items_reordered
    )
    {
        const S2 size = map_from_ordered.size();
        items_reordered.resize(size);

        const auto* from_ordered  = map_from_ordered.map();

        const auto* in_type  = items.type();
        const auto* in_value = items.value();

        auto* out_type  = items_reordered.type();
        auto* out_value = items_reordered.value();

        for(S2 i = 0; i < size; ++i)
        {
            const auto j = from_ordered[i];
            out_type[i] = in_type[j];
            out_value[i] = in_value[j];
        }
    }

    template<
        class N1, class I1, class S1,
        class I2, class S2,
        class I3, class S3,
        class N4, class I4, class S4,
        template<class...> class C1,
        template<class...> class C2,
        template<class...> class C3,
        template<class...> class C4
    >
    void reorder(
        const NodeForestV2_<N1,I1,S1,C1>& items,
        const IndexMapping<I2,S2,C2>& map_from_ordered,
        const IndexMapping<I3,S3,C3>& map_to_ordered,
        NodeForestV2_<N4,I4,S4,C4>& items_reordered
    )
    {
        const S2 size = map_from_ordered.size();
        items_reordered.resize(size);
        reorder(items.nodes(), map_from_ordered, items_reordered.nodes());

        const auto invalid_in = NodeForestV2_<N1,I1,S1,C1>::InvalidIndex();
        const auto invalid_out = NodeForestV2_<N4,I4,S4,C4>::InvalidIndex();

        const auto* from_ordered  = map_from_ordered.map();
        const auto* to_ordered  = map_to_ordered.map();
        
        const auto* in_up   = items.up();
        const auto* in_down = items.down();
        const auto* in_prev = items.prev();
        const auto* in_next = items.next();

        auto* out_up   = items_reordered.up();
        auto* out_down = items_reordered.down();
        auto* out_prev = items_reordered.prev();
        auto* out_next = items_reordered.next();

        for(S2 i = 0; i < size; ++i)
        {
            const auto j = from_ordered[i];
            const auto up = in_up[j];
            const auto down = in_down[j];
            const auto prev = in_prev[j];
            const auto next = in_next[j];
            out_up[i]   = (up   == invalid_in) ? invalid_out : to_ordered[up];
            out_down[i] = (down == invalid_in) ? invalid_out : to_ordered[down];
            out_prev[i] = (prev == invalid_in) ? invalid_out : to_ordered[prev];
            out_next[i] = (next == invalid_in) ? invalid_out : to_ordered[next];
        }
    }

    template<
        class F1,
        class I2, class S2,
        class I3, class S3,
        class F4,
        template<class...> class C1,
        template<class...> class C2,
        template<class...> class C3,
        template<class...> class C4
    >
    void reorder(
        const TraversePostorder<F1,C1>& items,
        const IndexMapping<I2,S2,C2>& map_from_ordered,
        const IndexMapping<I3,S3,C3>& map_to_ordered,
        TraversePostorder<F4,C4>& items_reordered
    )
    {
        const S2 size = map_from_ordered.size();
        items_reordered.set_count(size);
        items_reordered.resize(size);

        const auto invalid_in = TraversePostorder<F1,C1>::InvalidIndex();
        const auto invalid_out = TraversePostorder<F4,C4>::InvalidIndex();

        const auto* from_ordered  = map_from_ordered.map();
        const auto* to_ordered  = map_to_ordered.map();
        
        const auto* in_next = items.next();

        auto* out_next = items_reordered.next();

        auto front = items.front();
        auto back = items.back();
        items_reordered.set_front((front == invalid_in) ? invalid_out : to_ordered[front]);
        items_reordered.set_back((back == invalid_in) ? invalid_out : to_ordered[back]);

        for(S2 i = 0; i < size; ++i)
        {
            const auto j = from_ordered[i];
            const auto next = in_next[j];
            out_next[i] = (next == invalid_in) ? invalid_out : to_ordered[next];
        }

    }

    template<
        class F1,
        class I2, class S2,
        class I3, class S3,
        class F4,
        template<class...> class C1,
        template<class...> class C2,
        template<class...> class C3,
        template<class...> class C4
    >
    void reorder(
        const TraversePreorder<F1,C1>& items,
        const IndexMapping<I2,S2,C2>& map_from_ordered,
        const IndexMapping<I3,S3,C3>& map_to_ordered,
        TraversePreorder<F4,C4>& items_reordered
    )
    {
        const S2 size = map_from_ordered.size();
        items_reordered.set_count(size);
        items_reordered.resize(size);

        const auto invalid_in = TraversePreorder<F1,C1>::InvalidIndex();
        const auto invalid_out = TraversePreorder<F4,C4>::InvalidIndex();

        const auto* from_ordered  = map_from_ordered.map();
        const auto* to_ordered  = map_to_ordered.map();
        
        const auto* in_next = items.next();
        const auto* in_skip = items.skip();

        auto* out_next = items_reordered.next();
        auto* out_skip = items_reordered.skip();

        auto front = items.front();
        auto back = items.back();
        items_reordered.set_front((front == invalid_in) ? invalid_out : to_ordered[front]);
        items_reordered.set_back((back == invalid_in) ? invalid_out : to_ordered[back]);

        for(S2 i = 0; i < size; ++i)
        {
            const auto j = from_ordered[i];
            const auto next = in_next[j];
            const auto skip = in_skip[j];
            out_next[i] = (next == invalid_in) ? invalid_out : to_ordered[next];
            out_skip[i] = (skip == invalid_in) ? invalid_out : to_ordered[skip];
        }

    }

    template<
        class F1, class D1,
        class I2, class S2,
        class I3, class S3,
        class F4, class D4,
        template<class...> class C1,
        template<class...> class C2,
        template<class...> class C3,
        template<class...> class C4
    >
    void reorder(
        const NodeDepth<F1,D1,C1>& items,
        const IndexMapping<I2,S2,C2>& map_from_ordered,
        const IndexMapping<I3,S3,C3>& map_to_ordered,
        NodeDepth<F4,D4,C4>& items_reordered
    )
    {
        const S2 size = map_from_ordered.size();
        items_reordered.resize(size);

        items_reordered.set_max_depth(items.max_depth());
        
        const auto* from_ordered  = map_from_ordered.map();
        const auto* to_ordered  = map_to_ordered.map();
        
        const auto* in_depth = items.depth();
        auto* out_depth = items_reordered.depth();
        for(S2 i = 0; i < size; ++i)
        {
            const auto j = from_ordered[i];
            out_depth[i] = in_depth[j];
        }

    }

} // namespace do_ast
