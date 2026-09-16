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

    State initialization(const Key& k, const Nonce& n);
    vector<Byte> padd_associated_data(const AssociatedData& ad);
    vector<Block> to_blocks(vector<Byte>& padded);
    void associated_data_processing(State& s, const AssociatedData& ad);
    vector<Byte> padd_plaintext(const Plaintext& p);
    void append_word(vector<Byte>& output, Word w);
    Ciphertext plaintext_processing(State& s, const Plaintext& p);
    Tag finalization(State& s, const Key& k);
    EncryptionResult encryption(const Key& k, const Nonce& n, const AssociatedData& ad, const Plaintext& p);
};
