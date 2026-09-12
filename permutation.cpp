#include "types.hpp"
#include "permutation.hpp"

#include <cstdint>

using ascon::State;
using ascon::Word;

namespace ascon
{
    void substitution(State& s)
    // S-box is implemented using optimized Boolean instructions. //
    {
        Word x0 = s.s0;
        Word x1 = s.s1;
        Word x2 = s.s2;
        Word x3 = s.s3;
        Word x4 = s.s4;

        x0 ^= x4;
        x4 ^= x3;
        x2 ^= x1;

        Word t0 = x0;
        Word t1 = x1;
        Word t2 = x2;
        Word t3 = x3;
        Word t4 = x4;

        t0 =~ t0;
        t1 =~ t1;
        t2 =~ t2;
        t3 =~ t3;
        t4 =~ t4;

        t0 &= x1;
        t1 &= x2;
        t2 &= x3;
        t3 &= x4;
        t4 &= x0;

        x0 ^= t1;
        x1 ^= t2;
        x2 ^= t3;
        x3 ^= t4;
        x4 ^= t0;

        x1 ^= x0;
        x0 ^= x4;
        x3 ^= x2;
        x2 =~ x2;

        s.s0 = x0;
        s.s1 = x1;
        s.s2 = x2;
        s.s3 = x3;
        s.s4 = x4;
    };

    Word rotate_right(Word x, int position)
    // For a bitwise rotation, we use a right shift and left shift and OR them. //
    {
        Word right_shift =  x >> position;
        Word left_shift = x << (64 - position);

        return ( right_shift | left_shift );
    };

    void diffusion(State& s)
    {
        Word x0 = s.s0;
        Word x1 = s.s1;
        Word x2 = s.s2;
        Word x3 = s.s3;
        Word x4 = s.s4;

        x0 = x0 ^ rotate_right(x0, 19) ^ rotate_right(x0, 28);
        x1 = x1 ^ rotate_right(x1, 61) ^ rotate_right(x1, 39);
        x2 = x2 ^ rotate_right(x2, 1) ^ rotate_right(x2, 6);
        x3 = x3 ^ rotate_right(x3, 10) ^ rotate_right(x3, 17);
        x4 = x4 ^ rotate_right(x4, 7) ^ rotate_right(x4, 41);

        s.s0 = x0;
        s.s1 = x1;
        s.s2 = x2;
        s.s3 = x3;
        s.s4 = x4;
    };

    void round(State& s, Word constant) // use & for changing the original state, i.e. not making a copy
    {

    // 1. Constant-addition layer //
    s.s2 ^= constant;

    // 2. Substitution layer //
    substitution(s);

    // 3. Diffusion layer //
    diffusion(s);

    };

    /*
    void permutation(State& s, int number_of_rounds)
    {
        for(int i = 0; i < number_of_rounds; i++)
        {
            round(s, ????);
        }
    };
    */

};
