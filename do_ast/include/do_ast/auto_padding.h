#pragma once

#include <array>
#include <cstdint>
#include <type_traits>
#include <string>

namespace do_ast {

    template<uint32_t SizeOfPayload, uint32_t Alignment = 16>
    //using AutoPadding = std::enable_if_t<
    //    ((SizeOfPayload % Alignment) != 0),
    //    std::array<uint8_t, Alignment - (SizeOfPayload % Alignment)>
    //>;
    using AutoPadding = std::array<
        uint8_t, 
        ((SizeOfPayload % Alignment) == 0) ? Alignment : Alignment - (SizeOfPayload % Alignment)
    >;

} // namespace do_ast
