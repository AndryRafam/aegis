[![C++](https://img.shields.io/badge/C++-%2300599C.svg?logo=c%2B%2B&logoColor=white)](#)
[![Debian](https://img.shields.io/badge/Debian-A81D33?logo=debian&logoColor=fff)](#)
[![Fedora](https://img.shields.io/badge/Fedora-51A2DA?logo=fedora&logoColor=fff)](#)

<h1 align="left"> NeptuneCrypt (v1) </h1>

![Output](https://github.com/AndryRafam/NeptuneCrypt/blob/main/output/demo.gif)

<h2 algin="left"> About </h2>

NeptuneCrypt is a lightweight CLI encryption software using XChaCha20Poly1305 as cipher for simple encryption and XChaCha-Aes for cascade encryption (xchacha over aes) and SHA256 as hash key derivation function. 

This V1 version encrypts files and folders using simple encryption only.
The software was built with C++ (std=23) and CryptoPP: https://github.com/weidai11/cryptopp

<h2 align="left"> Tools and Requierments for building </h2>

- Programming Language: C++ (std=17/20/23)
- Debugger: GNU Debugger, Valgrind
- CryptoPP: Version 8.9.0-2

<h2 align="left"> Building for Linux </h2>

- GNU Make
- GNU C++ Compiler 14.2.0 or compatible
  
To build NeptuneCrypt, run the following command:

```bash
$ make
```
<h2 align="left"> How to encrypt folders ? </h2>

Encrypting file is straightforward. To encrypt folder, first compress/archive the folder (.7z, .zip, .rar, .tar etc.) and then encrypt with NeptuneCrypt.

<h2 align="left"> Future upgrade </h2>

- V1 version + cascade cipher XChaCha-Aes (xchacha over aes).
- V1 version + SM4 + AES as single cipher.


