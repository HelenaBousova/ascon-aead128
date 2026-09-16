#include "types.hpp"
#include "functions.hpp"

#include <array>
#include <vector>
#include <string>
#include <fstream>
#include <cstdint>
#include <iostream>

using std::array;
using std::vector;
using std::string;
using std::size_t;
using std::invalid_argument;
using std::runtime_error;
using std::ifstream;
using std::getline;
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
using ascon::KAT;

using ascon::encryption;
using ascon::decryption;

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
        word = word | static_cast<Word>(bytes[i]) << (8 * i);
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

string get_value(const string& line)
// extracts a string value from a line of file
{
    size_t position = line.find('=');

    // if there is no '=' to be found
    if (position == string::npos)
        return "";

    string value = line.substr(position + 1);

    // remove spaces
    while (value.empty() == 0 && value.front() == ' ')
        value.erase(value.begin());

    return value;
};

vector<KAT> load_kats(const string& filename)
// loads data from file
{
    // load file
    ifstream file(filename);

    if (!file) // if  there is no file to be loaded
        throw runtime_error("Could not open KAT file.");

    vector<KAT> kats;
    KAT current = KAT{};
    string line;

    // extract data from file
    while (getline(file, line))
    {
        if (line.rfind("Count", 0) == 0)
        {
            current = KAT{};
            current.count = std::stoi(get_value(line));
        }
        else if (line.rfind("Key", 0) == 0)
        {
            current.key = get_value(line);
        }
        else if (line.rfind("Nonce", 0) == 0)
        {
            current.nonce = get_value(line);
        }
        else if (line.rfind("PT", 0) == 0)
        {
            current.plaintext = get_value(line);
        }
        else if (line.rfind("AD", 0) == 0)
        {
            current.associated_data = get_value(line);
        }
        else if (line.rfind("CT", 0) == 0)
        {
            current.ciphertext_and_tag = get_value(line);

            // CT is the last field of one KAT
            kats.push_back(current);
        }
    };

    return kats;
};

void run_encryption_test()
{
    // load KATs file
    vector<KAT> kats = load_kats("KATs.txt");

    // counter for passed and failed tests
    size_t passed = 0;
    size_t failed = 0;

    constexpr size_t tag_size = 16;

    // test each individual KAT
    for (const KAT& kat : kats)
    {
        // 1. KAT preparation
        // convert to Key, Nonce, Plaintext, AD types
        Key k = hex_to_128_bits(kat.key);
        Nonce n = hex_to_128_bits(kat.nonce);
        Plaintext p = hex_to_bytes(kat.plaintext);
        AssociatedData ad = hex_to_bytes(kat.associated_data);

        // convert to Ciphertext and Tag types
        vector<Byte> c_and_t = hex_to_bytes(kat.ciphertext_and_tag);

         // expected ciphertext: everything except final 16 bytes
        Ciphertext c_expected(
            c_and_t.begin(),
            c_and_t.end() - tag_size
        );

        // expected tag: last 16 bytes
        const Byte* tag_start = c_and_t.data() + c_and_t.size() - tag_size;

        Tag t_expected{
            bytes_to_word(tag_start),
            bytes_to_word(tag_start + 8)
        };

        // 2. encryption
        EncryptionResult result = encryption(k, n, ad, p);

        // 3. comparison: KAT vs this implementation
        bool ciphertext_correct = (result.c == c_expected);
        bool tag_correct = (result.t == t_expected);

        // 4. evaluation
        if (ciphertext_correct && tag_correct)
        {
            passed++;
        }
        else
        {
            failed++;

            cout << "KAT " << kat.count << ": Failed\n";

            if (!ciphertext_correct)
            {
                cout << "Ciphertext incorrect\n";
            }

            if (!tag_correct)
            {
                cout << "Tag incorrect\n";
            }
        }
    };

    cout << "KAT encryption testing finished." << endl;
    cout << "Passed: " << passed << endl;
    cout << "Failed: " << failed << endl;
    cout << "Total:  " << kats.size() << endl;

    if (failed == 0)
        cout << "Test of encryption was successful.\n" << endl;
};

void run_decryption_test()
{
    // load KATs file
    vector<KAT> kats = load_kats("KATs.txt");

    // counter for passed and failed tests
    size_t passed = 0;
    size_t failed = 0;

    constexpr size_t tag_size = 16;

    // test each individual KAT
    for (const KAT& kat : kats)
    {
        // 1. KAT preparation
        // convert to Key, Nonce, Plaintext, AD types
        Key k = hex_to_128_bits(kat.key);
        Nonce n = hex_to_128_bits(kat.nonce);
        Plaintext p_expected = hex_to_bytes(kat.plaintext);
        AssociatedData ad = hex_to_bytes(kat.associated_data);

        // convert to Ciphertext and Tag types
        vector<Byte> c_and_t = hex_to_bytes(kat.ciphertext_and_tag);

         // ciphertext: everything except final 16 bytes
        Ciphertext c(
            c_and_t.begin(),
            c_and_t.end() - tag_size
        );

        // tag: last 16 bytes
        const Byte* tag_start = c_and_t.data() + c_and_t.size() - tag_size;

        Tag t{
            bytes_to_word(tag_start),
            bytes_to_word(tag_start + 8)
        };

        // 2. decryption
        optional<Plaintext> result = decryption(k, n, ad, c, t);

        // 3. comparison and evaluation
        if (result.has_value() && result.value() == p_expected)
        {
            passed++;
        }
        else
        {
            failed++;

            cout << "KAT " << kat.count << ": FAILED\n";

            if (!result.has_value())
            {
                cout << "  Authentication failed\n";
            }
            else
            {
                cout << "  Plaintext incorrect\n";
            }
        }
    };

    cout << "KAT decryption testing finished." << endl;
    cout << "Passed: " << passed << endl;
    cout << "Failed: " << failed << endl;
    cout << "Total:  " << kats.size() << endl;

    if (failed == 0)
        cout << "Test of decryption was successful.\n" << endl;
};

/*
int main()
{
    run_encryption_test();
    run_decryption_test();

    return 0;
};
*/
