#include "types.hpp"
#include "functions.hpp"

#include <array>
#include <vector>
#include <string>
#include <cstdint>
#include <iostream>

using std::array;
using std::vector;
using std::string;
using std::size_t;
using std::invalid_argument;
using std::cout;
using std::endl;

using ascon::Byte;
using ascon::Word;
using ascon::Key;
using ascon::Nonce;
using ascon::Plaintext;
using ascon::AssociatedData;
using ascon::Ciphertext;
using ascon::EncryptionResult;
using ascon::Tag;

using ascon::encryption;

int hex_value(char c)
{
    if (c >= '0' && c <= '9')
        return c - '0';

    if (c >= 'A' && c <= 'F')
        return 10 + (c - 'A');

    if (c >= 'a' && c <= 'f')
        return 10 + (c - 'a');

    throw invalid_argument("Invalid hexadecimal character");
};

vector<Byte> hex_to_bytes(const string& hex)
{
    vector<Byte> bytes;
    bytes.reserve(hex.size() / 2);

    for (std::size_t i = 0; i < hex.size(); i += 2)
    {
        Byte value = static_cast<Byte>(
            (hex_value(hex[i]) << 4) | hex_value(hex[i + 1])
        );

        bytes.push_back(value);
    };

    return bytes;
};

Word bytes_to_word(const Byte* bytes)
{
    Word word = 0;

    for (int i = 0; i < 8; ++i)
    {
        word |= static_cast<Word>(bytes[i]) << (8 * i);
    }

    return word;
};

array<Word, 2> hex_to_128_bits(const string& hex)
{
    vector<Byte> bytes = hex_to_bytes(hex);

    if (bytes.size() != 16)
        throw invalid_argument("Expected exactly 16 bytes");

    return {
        bytes_to_word(&bytes[0]),
        bytes_to_word(&bytes[8])
    };
};

void run_test()
{
    Key k = hex_to_128_bits("000102030405060708090A0B0C0D0E0F");
    Nonce n = hex_to_128_bits("101112131415161718191A1B1C1D1E1F");
    Plaintext p = hex_to_bytes("202122232425262728292A2B2C2D2E2F303132333435363738393A3B3C3D3E3F");
    AssociatedData ad = hex_to_bytes("303132333435363738393A3B3C3D3E3F404142434445464748494A4B");
    vector<Byte> c_plus_t = hex_to_bytes("52476FE14A8119E7CE3696C48930208F3DAEA77D67142DC97EEB7C934166A3F3C57C4E944A374176C7465FFE44C951AC");

    constexpr size_t tag_size = 16;

    Ciphertext c_expected(
        c_plus_t.begin(),
        c_plus_t.end() - tag_size
    );

    const Byte* tag_start =
        c_plus_t.data() + c_plus_t.size() - tag_size;

    Tag t_expected{
        bytes_to_word(tag_start),
        bytes_to_word(tag_start + 8)
    };

    EncryptionResult result = encryption(k, n, ad, p);

    bool ciphertext_correct = (result.c == c_expected);
    bool tag_correct = (result.t == t_expected);

    if (ciphertext_correct && tag_correct)
    {
        cout << "Test was successful." << endl;
    }
    else
    {
        cout << "Test failed." << endl;

        if (ciphertext_correct == 0)
        {
            cout << "Ciphertext is incorrect." << endl;
        };

        if (tag_correct == 0)
        {
            cout << "Tag is incorrect." << endl;
        };
    };
};

int main()
{
    run_test();

    return 0;
};


