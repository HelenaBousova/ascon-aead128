#include "types.hpp"
#include "functions.hpp"

#include <bitset>
#include <array>
#include <cstdint>
#include <iostream>

using ascon::State;
using ascon::Word;
using ascon::substitution;
using std::bitset;
using std::array;
using std::cout;
using std::endl;

State make_test_state(int number)
// Creates a state from integer. Integers to be used are from 0 to 31.
{
    bitset<5> bits(number); // makes 5-bit binary from integer "number"

    return State{
        static_cast<Word>(bits[4]),
        static_cast<Word>(bits[3]),
        static_cast<Word>(bits[2]),
        static_cast<Word>(bits[1]),
        static_cast<Word>(bits[0])
    };
};

// Official lookup table for S-box, outputs as integers.
array<int, 32> expected_sbox_int = {
    4, 11, 31, 20, 26, 21, 9, 2, 27, 5, 8, 18, 29, 3, 6, 28, 30, 19, 7, 14, 0, 13, 17, 24, 16, 12, 1, 25, 22, 10, 15, 23
};

// Official lookup table for S-box, outputs are converted to state type.
array<State, 32> make_expected_sbox()
{
    array<State, 32> result{};

    for (int i = 0; i < 32; i++)
    {
        result[i] = make_test_state(expected_sbox_int[i]);
    };

    return result;
};

array<State, 32> expected_sbox = make_expected_sbox();

// Helper function to compare output to expected output.
bool same_sbox_bits(const State& a, const State& b)
{
    return (a.s0 & 1) == (b.s0 & 1) &&
           (a.s1 & 1) == (b.s1 & 1) &&
           (a.s2 & 1) == (b.s2 & 1) &&
           (a.s3 & 1) == (b.s3 & 1) &&
           (a.s4 & 1) == (b.s4 & 1);
}

void run_sbox_test()
// S-box test function.
{
    int all_correct = 1;

    for(int test_int = 0; test_int < 32; test_int++)
    {
        // make state from integer
        State s = make_test_state(test_int);

        // run state through implemented s-box
        substitution(s);

        // compare output to expected output
        if (same_sbox_bits(s, expected_sbox[test_int]) == 1)
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
        cout << "Test successful, S-box implementation is correct." << endl;
    } else {
        cout << "S-box test failed." << endl;
    };
};

/*
int main()
{
    run_sbox_test();

    return 0;
};
*/
