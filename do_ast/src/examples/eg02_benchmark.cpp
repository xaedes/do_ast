
#include <iostream>
#include <unordered_map>
#include <string>
#include <cassert>
#include <chrono>
#include <cstdint>
#include <vector>

struct Addition
{
    uint32_t  res;
    uint32_t  lhs;
    uint32_t  rhs;
    uint32_t  pad;
};

using Number = int;
void mk_add(std::vector<Number>& values, std::vector<Addition>& ops)
{

    /*
     * 00  01  02  03  04  05  06  07  08
     *  \09/    \10/    \11/    \12/   |
     *    \--13--/        \--14--/     |
     *        \------15------/         |
     *                \---------16-----/
     */

    /*
     * 00  01  02  03  04  05  06  07  08  09  10
     *  \11/    \12/    \13/    \14/    \15/   |
     *    \--16--/        \--17--/        \-18-/
     *        \------19------/              |
     *                \---------20----------/
     */


    uint32_t last_overflow = 0;
    bool has_last_overflow = false;

    uint32_t last_start = 0;
    uint32_t last_count = values.size();

    while (last_count > 1)
    {
        for(uint32_t idx = 0; idx+1 < last_count; idx+=2)
        {
            uint32_t res = static_cast<uint32_t>(values.size());
            auto lhs = last_start+idx;
            auto rhs = last_start+idx+1;

            ops.push_back({res,lhs,rhs});
            values.push_back(0);
        }
        if (last_count % 2 == 1)
        {
            if (has_last_overflow)
            {
                uint32_t res = static_cast<uint32_t>(values.size());
                auto lhs = last_start+last_count-1;
                auto rhs = last_overflow;

                ops.push_back({res,lhs,rhs});
                values.push_back(0);

                has_last_overflow = false;
            }
            else
            {
                last_overflow = last_start+last_count-1;
                has_last_overflow = true;
            }
        }
        // auto new_count = values.size() - last_start;
        // if (new_count % 2 == 1)
        // {
        //     last_overflow = last_start+last_count-1;
        //     has_last_overflow = true;
        // }
        last_start = last_start + last_count;
        // auto new_count = values.size() - last_start;
        last_count = values.size() - last_start;
        // last_count = count;
    }
    if (has_last_overflow)
    {
        uint32_t res = static_cast<uint32_t>(values.size());
        auto lhs = last_start+last_count-1;
        auto rhs = last_overflow;

        ops.push_back({res,lhs,rhs});
        values.push_back(0);
    }
}

Number eval(std::vector<Number>& values, const std::vector<Addition>& ops)
{
    Number* val = values.data();
    for (int k=0; k<ops.size(); ++k)
    {
        auto op = ops[k];
        val[op.res] = val[op.lhs] + val[op.rhs];
    }
    return values.back();
}

Number add_for_loop(const std::vector<Number>& values)
{
    Number sum = 0;
    const Number* val = values.data();
    for (int k=0; k<values.size(); ++k)
    {
        sum += val[k];
    }
    return sum;
}

int main(int argc, char **argv)
{

    std::vector<Number> cvalues;
    std::vector<Number> values;
    std::vector<Addition> ops;
    //cvalues.resize(11);
    cvalues.resize(1024*1024);
    for (auto i = 0; i < cvalues.size(); ++i)
    {
        cvalues[i] = i;
    }
    values = cvalues;
    mk_add(values, ops);
    
    int num_it = 1024;
    Number sum0 = 0;
    Number sum1 = 0;
    
    auto t0 = std::chrono::system_clock::now();
    {
        sum0 = 0;
        for (int i=0; i<num_it; ++i)
        {
            sum0 += eval(values, ops);
        }
    }
    auto t1 = std::chrono::system_clock::now();
    {
        sum1 = 0;
        for (int i=0; i<num_it; ++i)
        {
            sum1 += add_for_loop(cvalues);
        }
    }
    auto t2 = std::chrono::system_clock::now();
    std::chrono::duration<double> d0 = t1-t0;
    std::chrono::duration<double> d1 = t2-t1;
    double fps0 = abs(d0.count()) > 1e-12 ? (num_it / d0.count()) : 0;
    double fps1 = abs(d1.count()) > 1e-12 ? (num_it / d1.count()) : 0;
    std::cout << "phase0: " << (d0.count() / num_it) * 1000 << " ms " << fps0 << " fps\n";
    std::cout << "phase1: " << (d1.count() / num_it) * 1000 << " ms " << fps1 << " fps\n";
    std::cout << " sum0 " << sum0;
    std::cout << " sum1 " << sum1;
    return 0;
}
