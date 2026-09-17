#include "types.hpp"
#include "functions.hpp"

#include <array>
#include <cstdint>
#include <iostream>

using ascon::State;
using ascon::Word;
using ascon::substitution;
using std::array;
using std::cout;
using std::endl;

// Input states, round constants and expected output states to test. Taken from independent and official python implementation.
const int number_of_test_vectors = 5;
array<State, number_of_test_vectors> round_input = {
    State{
    0x0000000000000000,
    0x0000000000000000,
    0x0000000000000000,
    0x0000000000000000,
    0x0000000000000000
    },
    State{
    0x0123456789abcdef,
    0xfedcba9876543210,
    0x0f0f0f0f0f0f0f0f,
    0xf0f0f0f0f0f0f0f0,
    0xaaaaaaaaaaaaaaaa
    },
    State{
    0x60daba1af85e2c62,
    0x927f6c956fa2b1d8,
    0x6e25f95240cb1737,
    0x5f8a15866dfc1b78,
    0xbf1551fbbf1551fb
    },
    State{
    0x883f9fcccf86c75a,
    0x0e2e17aebb3f26bb,
    0xb53cec76ec02e3f1,
    0xe705c56e525e9cf7,
    0xd1ceba764bf8b868
    },
    State{
    0xc6f11dfff48d4955,
    0x706a2a703a5a1315,
    0xdb643dd4af569f65,
    0x1cbcfdb30bbb40a4,
    0x92b5b45411fa08e8
    }
};

array<Word, number_of_test_vectors> round_constant = {
    Word{0xf0},
    Word{0xf0},
    Word{0xe1},
    Word{0xb4},
    Word{0xa5}
};

array<State, number_of_test_vectors> round_expected_output = {
    State{
    0x001e0f00000000f0,
    0x00000001e0000770,
    0x3fffffffffffff74,
    0x3c780000000000f0,
    0x0000000000000000
    },
    State{
    0x60daba1af85e2c62,
    0x927f6c956fa2b1d8,
    0x6e25f95240cb1737,
    0x5f8a15866dfc1b78,
    0xbf1551fbbf1551fb
    },
    State{
    0xf1f0a5638c114a5e,
    0x5f60ce2347d62aa2,
    0x38e6ffc8e6d6c9ae,
    0x43b4c4a08e7fc794,
    0x53933a0c484d0ef6
    },
    State{
    0xc6f11dfff48d4955,
    0x706a2a703a5a1315,
    0xdb643dd4af569f65,
    0x1cbcfdb30bbb40a4,
    0x92b5b45411fa08e8
    },
    State{
    0x202caaa308f6e2b0,
    0xffe8b912453958f1,
    0x36d35fb0ba000b3e,
    0xe52a057258368019,
    0xecff298d481ed95d
    }
};

void run_round_test()
// round test function
{
    cout << "Round test running..." << endl;

    int all_correct = 1;

    for(int test_int = 0; test_int < number_of_test_vectors; test_int++)
    {
        // take input state
        State s = round_input[test_int];

        // apply one round
        round(s, round_constant[test_int]);

        // check output state against expected output state
        if (s == round_expected_output[test_int])
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
        cout << "Test successful, round implementation is correct." << endl << endl;
    } else {
        cout << "Round test failed." << endl << endl;
    };
};


