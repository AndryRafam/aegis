[![C++](https://img.shields.io/badge/C++-%2300599C.svg?logo=c%2B%2B&logoColor=white)](#)
[![Debian](https://img.shields.io/badge/Debian-A81D33?logo=debian&logoColor=fff)](#)
[![Fedora](https://img.shields.io/badge/Fedora-51A2DA?logo=fedora&logoColor=fff)](#)

<h1 align="left"> NeptuneCrypt </h1>

**Version v1.4 (and future upgrade) is (will be) also available on Gitlab: https://gitlab.com/AndryRafam/neptunecrypt.**

**Latest release:** https://github.com/AndryRafam/NeptuneCrypt/releases/tag/v1.5 and here; https://gitlab.com/AndryRafam/neptunecrypt/-/releases/v1.5

![](output/demo_v1.5.gif)

<h2 algin="left"> About </h2>

NeptuneCrypt is a lightweight CLI encryption software using XChaCha20Poly1305, SM4-GCM and Aes-256-GCM as cipher for simple encryption and Argon2id for key derivation function.
The software was built with C++ (std=23) and cryptopp-modern: https://cryptopp-modern.com/

<h2 align="left"> About the cipher </h2>

- XChaCha20Poly1305: https://en.wikipedia.org/wiki/ChaCha20-Poly1305
- SM4-GCM (ShāngMì 4): https://en.wikipedia.org/wiki/SM4_(cipher)
- Aes256-GCM (orginal name Rijndael): https://en.wikipedia.org/wiki/Advanced_Encryption_Standard 

<h2 align="left"> Tools and Requierments for building and debugging </h2>

- Programming Language: C++ (std=17/20/23)
- Debugger: GNU Debugger, Valgrind
- cryptopp-modern: 2026.6.0 (https://cryptopp-modern.com/)
- GNU Make
- GNU C++ Compiler 14.2.0 or compatible

<h2 align="left"> Build, install and run on Linux (Debian, Fedora) </h2>
  
To build and install NeptuneCrypt on Linux, go to the directory containing all the codes and type the following command in terminal (super user mode). The executable file will be installed at /usr/local/bin/ directory.

```bash
$ sudo make
```
To run NeptuneCrypt on Linux, type the following command anywhere in terminal

```bash
$ neptune
```
<h2 align="left"> How to purge </h2>

To purge NeptuneCrypt, go to the directory containing all the codes (for exemple: NeptuneCrypt-main if you downloaded from github repo) and type the following command in terminal (super user mode).
This command will remove the binary file neptune and all dependencies.

```bash
$ sudo make clean
```

<h2 align="left"> How to encrypt folders ? </h2>

Encrypting file is straightforward. To encrypt folder, first compress/archive the folder (.7z, .zip, .rar, .tar etc.) and then encrypt with NeptuneCrypt.

<h2 align="left"> WARNING </h2>

NEPTUNE-CRYPT IS A FREE SOFTWARE AND COMES WITH ABSOLUTELY NO WARRANTY.
