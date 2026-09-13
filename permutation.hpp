#pragma once

#include "types.hpp"

#include <vector>

using std::vector;

namespace ascon
{

void substitution(State& s);
void diffusion(State& s);
void round(State& s, Word constant);
void permutation_12(State& s);
void permutation_8(State& s);

State initialization(Key k, Nonce n);
vector<Byte> padd_associated_data(const AssociatedData& ad);
vector<Block> to_blocks(vector<Byte>& padded);
void associated_data_processing(State& s, AssociatedData ad);

};
