#pragma once

#include <iostream>
#include <array>
#include <vector>
#include <chrono>

namespace do_ast {

    template<
        int Capacity = 128,
        class TClock = std::chrono::system_clock
    >
    struct Benchmark
    {
        using Clock = TClock;
        using Time = typename Clock::time_point;
        using Duration = std::chrono::duration<double>;
        using Label = const char*;
        enum class Type
        {
            Begin = 0,
            End
        };

        template<class F, class... Args>
        void benchmark(
            int num_iterations, 
            Label label, 
            F f, 
            Args... args
        )
        {
            begin(label, num_iterations);
            for(int i = 0; i < num_iterations; ++i)
            {
                f(args...);
            }
            end(label, num_iterations);
        }

        void begin(Label label = nullptr, int num_iterations = 1)
        {
            times[num_times] = Clock::now();
            types[num_times] = Type::Begin;
            labels[num_times] = label;
            iterations[num_times] = num_iterations;
            num_times++;
        }
        void end(Label label = nullptr, int num_iterations = 1)
        {
            times[num_times] = Clock::now();
            types[num_times] = Type::End;
            labels[num_times] = label;
            iterations[num_times] = num_iterations;
            num_times++;
        }

        std::array<Time, Capacity> times;
        std::array<Type, Capacity> types;
        std::array<Label, Capacity> labels;
        std::array<int, Capacity> iterations;

        int num_times = 0;



        struct Evaluation
        {
            struct Result
            {
                double   duration;
                double   duration_per_iteration;
                double   iterations_per_second;
                int      iterations_plan;
                int      iterations_done;
                int      begin;
                int      end;
                Time     begin_time;
                Time     end_time;
                Label    begin_label;
                Label    end_label;
                int      depth;
            };

            void evaluate(Benchmark& benchmark)
            {
                num_results = 0;
                struct StackItem
                {
                    int begin;
                    int end;
                };
                static std::vector<int> s_stack;
                s_stack.clear();
                for (int i = 0; i < benchmark.num_times; ++i)
                {
                    if (benchmark.types[i] == Type::Begin)
                    {
                        s_stack.push_back(i);
                    }
                    else if (
                        (benchmark.types[i] == Type::End)
                     && (s_stack.size() > 0)
                    )
                    {
                        auto begin = s_stack.back();
                        auto end = i;
                        Duration duration = benchmark.times[end] - benchmark.times[begin];
                        Result& result = results[num_results];
                        result.begin           = begin;
                        result.end             = end;
                        result.begin_time      = benchmark.times[begin];
                        result.end_time        = benchmark.times[end];
                        result.begin_label     = benchmark.labels[begin];
                        result.end_label       = benchmark.labels[end];
                        result.duration        = duration.count();
                        result.iterations_plan = benchmark.iterations[begin];
                        result.iterations_done = benchmark.iterations[end];
                        result.iterations_per_second = (
                            (abs(result.duration) > 1e-9)
                            ? result.iterations_done / result.duration
                            : 0
                        );
                        result.duration_per_iteration = (
                            (result.iterations_done > 0)
                            ? result.duration / result.iterations_done
                            : result.duration
                        );
                        result.depth           = s_stack.size()-1;
                        num_results++;
                        s_stack.pop_back();
                    }
                }
            }

            std::array<Result, Capacity> results;

            int num_results = 0;

            void print()
            {
                const char *indent = "  ";
                for (int i = 0; i < num_results; ++i)
                {
                    auto& result = results[i];
                    for(int d=0; d < result.depth; ++d)
                    {
                        std::cout << indent;
                    }
                    if (result.begin_label != nullptr)
                    {
                        std::cout << result.begin_label;
                    }
                    std::cout << " its: " << result.iterations_done;
                    std::cout << " sec: " << result.duration;
                    std::cout << " spi: " << result.duration_per_iteration;
                    std::cout << " ips: " << result.iterations_per_second;
                    std::cout << "\n";
                }
            }
        };
    };

} // namespace do_ast
