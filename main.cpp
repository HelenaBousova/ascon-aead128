#include "functions.hpp"
#include "types.hpp"

#include <iostream>
#include <string>
#include <vector>
#include <random>
#include <iomanip>
#include <sstream>

using ascon::Byte;
using ascon::Word;
using ascon::Key;
using ascon::Nonce;
using ascon::AssociatedData;
using ascon::Plaintext;
using ascon::EncryptionResult;

using ascon::encryption;
using ascon::decryption;

using Value128 = std::array<Word, 2>;

using namespace std;

Value128 random_128()
// generates a random 128-bit value (32 hexdec characters into two words)
{
    random_device rd;
    Value128 value{0, 0};

    for (int word = 0; word < 2; word++)
    {
        for (int byte = 0; byte < 8; byte++)
        {
            Word random_byte = static_cast<Word>(rd() & 0xff);

            value[word] = value[word] | random_byte << (8 * byte);
        };
    };

    return value;
};

string value_128_to_string(const Value128& value)
{
    ostringstream output;

    output << hex << setfill('0');

    for (Word word : value)
    {
        for (int i = 0; i < 8; i++)
        {
            Byte byte =
                static_cast<Byte>((word >> (8 * i)) & 0xff);

            output << setw(2)
                   << static_cast<int>(byte);
        };
    };

    return output.str();
}

string bytes_to_string(const vector<Byte>& data)
{
    ostringstream output;

    output << hex << setfill('0');

    for (Byte byte : data)
    {
        output << setw(2)
                << static_cast<int>(byte);
    }

    return output.str();
}

int main()
{
    // display intro
    cout << "This is an educational implementation of the lightweight cryptography standard Ascon-AEAD 128, which was released by NIST in 2025." << endl << endl;
    cout << "You are now interacting with the demo version." << endl << endl;
    cout << "You can encrypt and decrypt your own message here, using Ascon-AEAD 128. I'll guide you through the process. Let's begin!" << endl << endl;
    cout << "Are you ready? Press [Enter]." << endl;

    string wait1;
    getline(cin, wait1);

    // choose key
    cout << "First, let's choose a key. For generating a valid 128-bit key, press [Enter]." << endl;

    string wait2;
    getline(cin, wait2);

    Key key = random_128();
    string key_string = value_128_to_string(key);

    cout << "Your key is: " << key_string << endl << endl;

    // choose nonce
    cout << "Second, we need a nonce. For generating a valid 128-bit nonce, press [Enter]." << endl;

    string wait3;
    getline(cin, wait3);

    Nonce nonce = random_128();
    string nonce_string = value_128_to_string(nonce);

    cout << "Your nonce is: " << nonce_string << endl << endl;

    // choose associated data
    cout << "Now it's time for your message! In addition to the data you want to encrypt, "
        "you can provide associated data. Associated data is not encrypted, so it remains readable, "
        "but it is authenticated together with your message. If it is modified, authentication will fail." << endl << endl;
    cout << "Type your associated data now. If you don't want to send any associated data, press [Enter]." << endl;

    string ad_input;
    getline(cin, ad_input);

    AssociatedData ad(ad_input.begin(), ad_input.end());

    // choose plaintext
    cout << "Very well! Finally, it is time for your secret message. Type it here." << endl;

    string p_input;
    getline(cin, p_input);

    Plaintext plaintext = Plaintext(p_input.begin(), p_input.end());

    // encryption
    cout << "Perfect! Your message is going to be encrypted now. To see the result, press [Enter]." << endl;

    string wait4;
    getline(cin, wait4);

    EncryptionResult result = encryption(key, nonce, ad, plaintext);

    string c_string = bytes_to_string(result.c);
    string t_string = value_128_to_string(result.t);

    cout << "Your message was encrypted as: " << c_string << endl;
    cout << "Your authentication tag is: " << t_string << endl << endl;

    // decryption
    cout << "To see it worked, you can now decrypt the ciphertext to recover you original message." << endl;
    cout << "Also, the authentication tag you received will be checked to make sure the data has not been tampered with." << endl << endl;
    cout << "Press [Enter] to decrypt." << endl;

    string wait5;
    getline(cin, wait5);

    optional<Plaintext> decrypted = decryption(key, nonce, ad, result.c, result.t);

    // check authentication
    if (decrypted.has_value())
    {
        // convert recovered plaintext bytes back to text
        Plaintext decrypted_plaintext = decrypted.value();
        string decrypted_message(decrypted_plaintext.begin(), decrypted_plaintext.end());

        cout << "Authentication successful!" << endl;
        cout << "Your decrypted message is: " << decrypted_message << endl;
    }
    else
    {
        cout << "Authentication failed!" << endl;
        cout << "The ciphertext, associated data, key, nonce, or authentication tag may have been changed." << endl;
    };

    return 0;
};

