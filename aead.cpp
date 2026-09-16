#include "types.hpp"
#include "functions.hpp"

#include <cstdint>
#include <array>
#include <vector>

using ascon::State;
using ascon::Word;
using ascon::Key;
using ascon::Nonce;
using std::array;
using std::vector;
using std::size_t;

namespace ascon
{
    // initialization vector
    constexpr Word iv = 0x00001000808c0001;

    State initialization(const Key& k, const Nonce& n)
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

        for (size_t i = 0; i < padded.size(); i += rate_in_bytes)
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

    void associated_data_processing(State& s, const AssociatedData& ad)
    // associated data processing function
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
        s.s4 ^= (Word{1} << 63);
    };

    vector<Byte> padd_plaintext(const Plaintext& p)
    {
        vector<Byte> padded = p;

        // padd plaintext with a 1
        padded.push_back(0x01);

        // padd plaintext with 0s so that we have full 128-bit blocks
        while (padded.size() % rate_in_bytes != 0)
        {
            padded.push_back(0x00);
        }

        return padded;
    };

    void append_word(vector<Byte>& output, Word w)
    // splits word to bytes and appends them to output (from function argument)
    {
        for (int i = 0; i < 8; i++)
        {
            output.push_back(
                static_cast<Byte>((w >> (8 * i)) & 0xff)
            );
        };
    };

    Ciphertext plaintext_processing(State& s, const Plaintext& p)
    // plaintext processing function
    {
        // 1. padd plaintext
        vector<Byte> p_padded = padd_plaintext(p);

        // 2. parse plaintext to 128-bit blocks
        vector<Block> p_blocks = to_blocks(p_padded);

        Ciphertext c;
        // 3. proscess blocks EXCEPT the last one
        for (size_t i = 0; i + 1 < p_blocks.size(); i++)
        {
            const Block& block = p_blocks[i];

            // 3.1. absorb plaintext (XOR block to first 128 bits of state)
            s.s0 ^= block[0];
            s.s1 ^= block[1];

            // 3.2. generate ciphertext
            append_word(c, s.s0);
            append_word(c, s.s1);

            // 3.3. p[8] after each block
            permutation_8(s);
        };

        // 4. process final block
        const Block& last_block = p_blocks.back();

        // 4.1. absorb plaintext (XOR)
        s.s0 ^= last_block[0];
        s.s1 ^= last_block[1];

        // 4.2. generate ciphertext from the rest of plaintext, but NOT from padding
        size_t remaining_p = p.size() % 16;

        vector<Byte> c_last;
        append_word(c_last, s.s0);
        append_word(c_last, s.s1);

        for (size_t i = 0; i < remaining_p; i++)
        {
            c.push_back(c_last[i]);
        }

        return c;
    };

    Tag finalization(State& s, const Key& k)
    // finalization and tag generation
    {
        // 1. absorb key to state (XOR to 2nd and 3rd word of state)
        s.s2 ^= k[0];
        s.s3 ^= k[1];

        // 2. permute state by p[12]
        permutation_12(s);

        // 3. generate tag (XOR key to the last two words of state)
        Tag t;

        t[0] = s.s3 ^ k[0];
        t[1] = s.s4 ^ k[1];

        return t;
    };

    EncryptionResult encryption(const Key& k, const Nonce& n, const AssociatedData& ad, const Plaintext& p)
    {
        // 1. initialization
        State s = initialization(k, n);

        // 2. associated data processing
        associated_data_processing(s, ad);

        // 3. plaintext processing
        Ciphertext c = plaintext_processing(s, p);

        // 4. finalization + tag generation
        Tag t = finalization(s, k);

        // 5. output ciphertext and tag
        EncryptionResult result = EncryptionResult{c, t};
        return result;
    };
};
