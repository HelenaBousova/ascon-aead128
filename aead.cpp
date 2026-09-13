#include "types.hpp"
#include "permutation.hpp"

#include <cstdint>
#include <array>
#include <vector>

using ascon::State;
using ascon::Word;
using ascon::Key;
using ascon::Nonce;
using std::array;
using std::vector;

namespace ascon
{
    // initialization vector
    constexpr Word iv = 0x00001000808c0001;

    State initialization(Key k, Nonce n)
    // initialization of Ascon-AEAD128
    {
        // 1. initialize state as iv || k || n
        Word s0 = iv;
        Word s1 = k[0];
        Word s2 = k[1];
        Word s3 = n[0];
        Word s4 = n[1];

        State s = State{s0, s1, s2, s3, s4};

        // 2. apply permutation p[12]
        permutation_12(s);

        // 3. XOR key to the end of the state
        s.s3 ^= k[0];
        s.s4 ^= k[1];

        return s;
    };

    constexpr int rate_in_bytes = 16;

    vector<Byte> padd_associated_data(const AssociatedData& ad)
    {
        vector<Byte> padded = ad;

        // padd AD with a 1
        padded.push_back(0x01);

        // padd AD with 0s so that we have full 128-bit blocks
        while (padded.size() % rate_in_bytes != 0)
        {
            padded.push_back(0x00);
        }

        return padded;
    };

    vector<Block> to_blocks(vector<Byte>& padded)
    {
        vector<Block> blocks;

        for (std::size_t i = 0; i < padded.size(); i += rate_in_bytes)
        {
            Block block{0, 0};

            // convert 8 bytes to block[0]
            for (int j = 0; j < 8; j++)
            {
                block[0] = block[0] | static_cast<Word>(padded[i + j]) << (8 * j);
            };

            // convert next 8 bytes to block[1]
            for (int j = 0; j < 8; j++)
            {
                block[1] = block[1] | static_cast<Word>(padded[i + 8 + j]) << (8 * j);
            };

        blocks.push_back(block);
        };

        return blocks;
    };

    void associated_data_processing(State& s, AssociatedData ad)
    {
        if (ad.size() > 0)
        {
            // 1. padd associated data
            vector<Byte> ad_padded = padd_associated_data(ad);

            // 2. parse associated data to 128-bit blocks
            vector<Block> ad_blocks = to_blocks(ad_padded);

            // 3. XOR the blocks to the first 128 bits of state (absorption of AD)
            for (const Block& block : ad_blocks)
            {
                s.s0 ^= block[0];
                s.s1 ^= block[1];

                // 4. p[8] after each block
                permutation_8(s);
            };
        };

        // 5. XOR a 1 to the end of state
        Word one = Word{0x01};
        s.s4 ^= one;
    };

    void plaintext_processing(State& s, Plaintext p)
    {

    };

    void finalization(State& s, Key k)
    {

    };

};
