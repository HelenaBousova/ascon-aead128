# Ascon-AEAD128 in C++

An educational implementation of Ascon-AEAD128 in C++, based on the Ascon standard published by NIST in 2025 as NIST SP 800-232.

The project implements authenticated encryption and decryption from scratch, including the Ascon permutation, associated data processing, authentication tag, and testing against 1089 official Known Answer Tests.

## Features

- Ascon 320-bit internal state
- S-box and linear diffusion
- `p[12]` and `p[8]` permutations
- encryption and decryption
- associated data support
- authentication tag generation and verification
- interactive console demo
- automated testing with official test vectors

## How It Works

Ascon-AEAD128 is a lightweight, permutation-based authenticated encryption algorithm built around a 320-bit internal state.

Its design is related to the sponge and duplex approach. The state is repeatedly updated while data is absorbed and, during encryption, output is produced from the same evolving state.

The encryption process consists of four main stages: initialization, associated data processing, plaintext encryption, and finalization. The result is the ciphertext together with a 128-bit authentication tag.

Each permutation round consists of three steps:

- addition of a round constant

- substitution using the S-box

- linear diffusion

Ascon-AEAD128 uses two versions of the permutation: p[12] with 12 rounds and p[8] with 8 rounds.

Associated data is authenticated but not encrypted, so any modification to either the associated data or ciphertext causes authentication to fail.

## Project Structure

The project is divided into several parts:

- `headers/` contains shared types and function declarations
- `sources/` contains the Ascon permutation and AEAD implementation
- `tests/` contains tests for individual components and complete encryption/decryption
- `main.cpp` contains the console demo and test menu
- `KATs.txt` contains the official Known Answer Test vectors used for verification

## Running the Project

After starting the program, the user can choose between the interactive encryption/decryption demo and the automated tests.

## Testing

The project includes separate tests for the S-box, diffusion layer, permutation rounds, encryption, and decryption.

The complete implementation is also checked against the official Ascon-AEAD128 Known Answer Tests (KATs).

**All 1089 available KAT vectors passed successfully**.

## Project Takeaways

This project gave me a better understanding of modern cryptography and C++ programming.

I gained practical experience with:
- implementing a cryptographic algorithm from a technical specification
- testing individual components of an algorithm
- working with official KAT vectors
- bitwise operations
- designing, structuring, debugging and testing a larger C++ project

Since I studied introductory cryptography before implementing this algorithm, I particularly value that I could put my theoretical knowledge into practice and gain deeper understanding of key cryptographic concepts. 

## References

- [NIST SP 800-232 — Ascon-Based Lightweight Cryptography Standards for Constrained Devices](https://csrc.nist.gov/pubs/sp/800/232/final)
- [Official Ascon C implementation — ascon/ascon-c](https://github.com/ascon/ascon-c)
- [Ascon Python reference implementation — pyascon](https://github.com/meichlseder/pyascon)
- Official Ascon-AEAD128 Known Answer Tests from the `ascon-c` repository

## Disclaimer

This project was created for educational purposes and is not intended for production use.
