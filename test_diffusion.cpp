#include "types.hpp"
#include "permutation.hpp"

#include <array>
#include <cstdint>
#include <iostream>

using ascon::State;
//using ascon::Word;
using ascon::diffusion;
using std::array;
using std::cout;
using std::endl;

// Define input states and their expected output states. Input/output states taken from independent implementation.
const int number_of_test_vectors = 5;
array<State, number_of_test_vectors> diffusion_input = {
    State{},
    State{},
    State{},
    State{},
    State{}
};

array<State, number_of_test_vectors> diffusion_expected_output = {
    State{},
    State{},
    State{},
    State{},
    State{}
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


int main()
{
    run_diffusion_test();

    return 0;
};

