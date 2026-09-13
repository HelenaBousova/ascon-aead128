#include "types.hpp"
#include "permutation.hpp"

#include <array>
#include <cstdint>
#include <iostream>

using ascon::State;
using ascon::Word;
using ascon::permutation_12;
using ascon::permutation_8;
using std::array;
using std::cout;
using std::endl;

// Input states, round constants and expected output states to test. Taken from independent and official python implementation.
const int number_of_test_vectors = 5;
array<State, number_of_test_vectors> permutation_input = {
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

array<State, number_of_test_vectors> permutation_12_expected_output = {
    State{
    0x78ea7ae5cfebb108,
    0x9b9bfb8513b560f7,
    0x6937f83e03d11a50,
    0x3fe53f36f2c1178c,
    0x045d648e4def12c9
    },
    State{
    0x8bd8f6d718dafd12,
    0x7acc2835bccaa85f,
    0xde93ec3b252c99fd,
    0x5fa639e871f24373,
    0xf5725dcf7011ce03
    },
    State{
    0xf89f3c6917b8f7f3,
    0xad6958e76ee546ca,
    0xcd41bbbe5d15182a,
    0x23d2cc4c0f59feb5,
    0xd930168b48291c8a
    },
    State{
    0xf92580fc624ce1b3,
    0x712f83bdad2773f5,
    0x8f84c0dda271c3bb,
    0x0557f2baf193dc4b,
    0x75adf3de3234fb7c
    },
    State{
    0xb8974637eddb63c3,
    0xad067b41fc6f8066,
    0x8f84bf33fa32025b,
    0xcdc514bc7a7f2471,
    0x1c1748f31bd2c54f
    }
};

array<State, number_of_test_vectors> permutation_8_expected_output = {
    State{
    0x1418f8af721aa830,
    0xa5425f1f8cb31388,
    0xa01ef761bf8e1652,
    0xf01fdabf8c8a82b4,
    0x0168260badf76a06
    },
    State{
    0xc42d9512e5c43e0f,
    0x4e424535d95746d9,
    0x7a92b3a8bea42f29,
    0xeeb061717623bd7d,
    0x07e4f3da2bf1da93
    },
    State{
    0xe14e070d50782aee,
    0x347c19f923cfeb32,
    0xba0b7160464f9f2a,
    0x2848ac5d478b64a8,
    0x606a010c2561e819
    },
    State{
    0x78ea7ae5cfebb108,
    0x9b9bfb8513b560f7,
    0x6937f83e03d11a50,
    0x3fe53f36f2c1178c,
    0x045d648e4def12c9
    },
    State{
    0x7bf9919ee7bf2f8c,
    0x7210282131a5786e,
    0x8411e8d2a78672b7,
    0x42eb4c4f23ab6197,
    0x0fcf141816db7b9a
    }
};

void run_permutation_12_test()
// p[12] test function
{
    int all_correct = 1;

    for(int test_int = 0; test_int < number_of_test_vectors; test_int++)
    {
        // take input state
        State s = permutation_input[test_int];

        // apply p[12]
        permutation_12(s);

        // check output state against expected output state
        if (s == permutation_12_expected_output[test_int])
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
        cout << "Test of p[12] implementation was successful." << endl;
    } else {
        cout << "p[12] test failed." << endl;
    };
};

void run_permutation_8_test()
// p[8] test function
{
    int all_correct = 1;

    for(int test_int = 0; test_int < number_of_test_vectors; test_int++)
    {
        // take input state
        State s = permutation_input[test_int];

        // apply p[8]
        permutation_8(s);

        // check output state against expected output state
        if (s == permutation_8_expected_output[test_int])
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
        cout << "Test of p[8] implementation was successful." << endl;
    } else {
        cout << "p[8] test failed." << endl;
    };
};

int main()
{
    run_permutation_12_test();
    run_permutation_8_test();

    return 0;
};

