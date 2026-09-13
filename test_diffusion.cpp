#include "types.hpp"
#include "permutation.hpp"

#include <array>
#include <cstdint>
#include <iostream>

using ascon::State;
using ascon::diffusion;
using std::array;
using std::cout;
using std::endl;

// Define input states and their expected output states. Input/output states taken from independent implementation.
const int number_of_test_vectors = 5;
array<State, number_of_test_vectors> diffusion_input = {
    State{0x0000000000000000,
    0x0000000000000000,
    0x0000000000000000,
    0x0000000000000000,
    0x0000000000000000},
    State{0xFFFFFFFFFFFFFFFF,
    0xFFFFFFFFFFFFFFFF,
    0xFFFFFFFFFFFFFFFF,
    0xFFFFFFFFFFFFFFFF,
    0xFFFFFFFFFFFFFFFF},
    State{0x0000000000000001,
    0x0000000000000001,
    0x0000000000000001,
    0x0000000000000001,
    0x0000000000000001},
    State{0x0123456789ABCDEF,
    0xFEDCBA9876543210,
    0x0F0F0F0F0F0F0F0F,
    0xF0F0F0F0F0F0F0F0,
    0xAAAAAAAAAAAAAAAA},
    State{0x0001020304050607,
    0x08090A0B0C0D0E0F,
    0x1011121314151617,
    0x18191A1B1C1D1E1F,
    0x2021222324252627}
};

array<State, number_of_test_vectors> diffusion_expected_output = {
    State{0x0000000000000000,
    0x0000000000000000,
    0x0000000000000000,
    0x0000000000000000,
    0x0000000000000000},
    State{0xFFFFFFFFFFFFFFFF,
    0xFFFFFFFFFFFFFFFF,
    0xFFFFFFFFFFFFFFFF,
    0xFFFFFFFFFFFFFFFF,
    0xFFFFFFFFFFFFFFFF},
    State{0x0000201000000001,
    0x0000000002000009,
    0x8400000000000001,
    0x0040800000000001,
    0x0200000000800001},
    State{0xE2227BB3F3336AA2,
    0x38D5C63FE5081BE2,
    0xB4B4B4B4B4B4B4B4,
    0xB4B4B4B4B4B4B4B4,
    0xAAAAAAAAAAAAAAAA},
    State{ 0xE0918273245546B7,
    0x5E59404F72756C63,
    0xC459DF52D24FC944,
    0x10D0905117D79756,
    0x7FF372F471FD7CFA}
};

void run_diffusion_test()
// diffusion test function
{
    int all_correct = 1;

    for(int test_int = 0; test_int < number_of_test_vectors; test_int++)
    {
        // take input state
        State s = diffusion_input[test_int];

        // apply diffusion
        diffusion(s);

        // check output state against expected output state
        if (s == diffusion_expected_output[test_int])
        {
            cout << "Input " << test_int << ": Correct." << endl;
        } else {
            cout << "Input " << test_int << ": Failed." << endl;
            all_correct = 0;
        };
    };

    // decide if the test was successful
    if (all_correct == 1)
    {
        cout << "Test successful, diffusion implementation is correct." << endl;
    } else {
        cout << "Diffusion test failed." << endl;
    };
};

/*
int main()
{
    run_diffusion_test();

    return 0;
};
*/
