[![C++](https://img.shields.io/badge/C++-%2300599C.svg?logo=c%2B%2B&logoColor=white)](#)
[![Debian](https://img.shields.io/badge/Debian-A81D33?logo=debian&logoColor=fff)](#)
[![Fedora](https://img.shields.io/badge/Fedora-51A2DA?logo=fedora&logoColor=fff)](#)

<h1 align="left"> NeptuneCrypt </h1>

**Also available here:** https://gitlab.com/AndryRafam/neptunecrypt

![](output/demo_1.6.gif)

<h2 algin="left"> About </h2>

NeptuneCrypt is a lightweight CLI encryption software using XChaCha20Poly1305, SM4-GCM, Aes-256-GCM and Twofish-EAX (aes competition runner up) as cipher for simple encryption and Argon2id for key derivation function (secure password hashing).
The software was built with C++ (std=23) and cryptopp-modern: https://cryptopp-modern.com/

<h2 align="left"> About the cipher </h2>

- XChaCha20Poly1305: https://en.wikipedia.org/wiki/ChaCha20-Poly1305
- SM4-GCM (ShāngMì 4): https://en.wikipedia.org/wiki/SM4_(cipher)
- Aes256-GCM (orginal name Rijndael): https://en.wikipedia.org/wiki/Advanced_Encryption_Standard
- Twofish-EAX (aes competition runner up by Bruce Schneier): https://en.wikipedia.org/wiki/Twofish

<h2 align="left"> Tools and Requierments for building and debugging </h2>

- Programming Language: C++ (std=17/20/23)
- Debugger: GNU Debugger, Valgrind
- cryptopp-modern: 2026.6.0 (https://cryptopp-modern.com/)
- GNU Make
- GNU C++ Compiler 14.2.0 or compatible

<h2 align="left"> Build, install and run on Linux (Debian, Fedora) </h2>
  
To build and install NeptuneCrypt on Linux, type the following command in terminal. The executable file will be installed at /usr/local/bin/ directory.

```bash
$ sudo make
```
To run NeptuneCrypt on Linux, type the following command anywhere in terminal

```bash
$ neptune
```
<h2 align="left"> How to purge </h2>

```bash
$ sudo make clean
```
This command will remove the binary file neptune and all dependencies.

<h2 align="left"> How to encrypt folders ? </h2>

Encrypting file is straightforward. To encrypt folder, first compress/archive the folder (.7z, .zip, .rar, .tar etc.) and then encrypt.


**NeptuneCrypt is free software and comes with absolutely no warranty.**
