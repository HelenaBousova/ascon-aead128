#pragma once

#include <cstdint>
#include <array>
#include <vector>


namespace ascon
{
    using Byte = std::uint8_t;
    using Word = std::uint64_t;
    using Key = std::array<Word, 2>;
    using Nonce = std::array<Word, 2>;
    using Block = std::array<Word, 2>;
    using AssociatedData = std::vector<Byte>;
    using Plaintext = std::vector<Byte>;

    struct State {
        Word s0;
        Word s1;
        Word s2;
        Word s3;
        Word s4;

        bool operator==(const State& to_compare) const
        // == overload to compare two states
        {
            return s0 == to_compare.s0 &&
                   s1 == to_compare.s1 &&
                   s2 == to_compare.s2 &&
                   s3 == to_compare.s3 &&
                   s4 == to_compare.s4;
        };
    };
};
