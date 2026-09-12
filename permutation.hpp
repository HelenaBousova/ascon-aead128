#pragma once

#include "types.hpp"

namespace ascon
{

void substitution(State& s);
void diffusion(State& s);
void round(State& s, Word constant);
void permutation(State& s, int number_of_rounds);

};
