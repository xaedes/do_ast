#pragma once
#include <vector>
#include <cstdint>

struct Operation
{
    uint32_t  res;
    uint32_t  lhs;
    uint32_t  rhs;
    uint32_t  pad;
};

// using Number = int;
uint32_t mk_reduction(uint32_t num_values, std::vector<Operation>& ops)
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
    uint32_t last_count = num_values;

    while (last_count > 1)
    {
        for(uint32_t idx = 0; idx+1 < last_count; idx+=2)
        {
            uint32_t res = num_values;
            auto lhs = last_start+idx;
            auto rhs = last_start+idx+1;

            ops.push_back({res,lhs,rhs});
            num_values++;
        }
        if (last_count % 2 == 1)
        {
            if (has_last_overflow)
            {
                uint32_t res = num_values;
                auto lhs = last_start+last_count-1;
                auto rhs = last_overflow;

                ops.push_back({res,lhs,rhs});
                num_values++;

                has_last_overflow = false;
            }
            else
            {
                last_overflow = last_start+last_count-1;
                has_last_overflow = true;
            }
        }
        // auto new_count = num_values - last_start;
        // if (new_count % 2 == 1)
        // {
        //     last_overflow = last_start+last_count-1;
        //     has_last_overflow = true;
        // }
        last_start = last_start + last_count;
        // auto new_count = num_values - last_start;
        last_count = num_values - last_start;
        // last_count = count;
    }
    if (has_last_overflow)
    {
        uint32_t res = num_values;
        auto lhs = last_start+last_count-1;
        auto rhs = last_overflow;

        ops.push_back({res,lhs,rhs});
        num_values++;
    }
    return num_values;
}

