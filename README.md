[![C++](https://img.shields.io/badge/C++-%2300599C.svg?logo=c%2B%2B&logoColor=white)](#)
[![Debian](https://img.shields.io/badge/Debian-A81D33?logo=debian&logoColor=fff)](#)
[![Fedora](https://img.shields.io/badge/Fedora-51A2DA?logo=fedora&logoColor=fff)](#)

<h1 align="left"> Aegis (formerly NeptuneCrypt) </h1>

<h2 algin="left"> About </h2>

Aegis is a lightweight TUI (text user interface) encryption software using Aes-256-GCM, SM4-GCM, Twofish-EAX and  XChaCha20Poly1305 as cipher for simple encryption and Argon2id for key derivation function (secure password hashing). The software was built with C++ (std=23) and cryptopp-modern: https://cryptopp-modern.com/. The user can choose between generating a random password or enter manually a custom password.

<h2 align="left"> Supported Ciphers </h2>

- Aes256-GCM (orginal name Rijndael): https://en.wikipedia.org/wiki/Advanced_Encryption_Standard
- SM4-GCM (ShāngMì 4): https://en.wikipedia.org/wiki/SM4_(cipher)
- Twofish-EAX (Aes contest runner up developped by Bruce Schneier): https://en.wikipedia.org/wiki/Twofish
- XChaCha20Poly1305: https://en.wikipedia.org/wiki/ChaCha20-Poly1305

<h2 align="left"> Tools and Requierments for building and debugging </h2>

- Programming Language: C++ (std=17/20/23)
- Debugger: GNU Debugger, Valgrind
- cryptopp-modern: 2026.6.0 (https://cryptopp-modern.com/)
- GNU C++ Compiler 14.2.0 or compatible
- CMake (>= 3.22 version)

<h2 align="left"> Build and install on Linux (Debian, Fedora) </h2>
<h3 align="left"> CMake </h3>
To build and install using cmake, type the following command in terminal. The executable file will be installed at /usr/local/bin/ directory.
Create and enter a temporary build directory

```json
$ cmake -B build -S .
```
Compile the executable

```json
$ cmake --build build
```
Install 'aegis' straight to /usr/local/bin

```json
$ sudo cmake --install build
```
To run on Linux, type the following command anywhere in terminal

```json
$ aegis
```
<h2 align="left"> Cleaning up </h2>
<h3 align="left"> CMake </h3>

```json
$ rm -rf build/
```
To purge the installed binary from /usr/local/bin, run:

```json
$ sudo rm -f /usr/local/bin/aegis
```
<h2 align="left"> How to encrypt folders ? </h2>

To encrypt folder, first compress/archive the folder (.7z, .zip, .rar, .tar etc.) and then encrypt.

<h2 align="left"> Acknowledgement </h2>

Aegis is free software and comes with absolutely no warranty. This software is intended for personal use and educational use only. It is not suitable for industrial/professional use.

