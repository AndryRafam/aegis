[![C++](https://img.shields.io/badge/C++-%2300599C.svg?logo=c%2B%2B&logoColor=white)](#)
[![Debian](https://img.shields.io/badge/Debian-A81D33?logo=debian&logoColor=fff)](#)
[![Fedora](https://img.shields.io/badge/Fedora-51A2DA?logo=fedora&logoColor=fff)](#)

<h1 align="left"> NeptuneCrypt (v1) </h1>

![Output](https://github.com/AndryRafam/Program-Output/blob/master/enc_dec_neptune.gif)

<h2 algin="left"> About </h2>

NeptuneCrypt is a lightweight CLI encryption (decryption) software using XChaCha20Poly1305, Aes256-gcm as ciphers; XChaCha-Aes for cascade cipher (xchacha over aes) and SHA256 as hash key derivation function. 

This V1 version encrypts files only, including compressed files such as .7z, .zip, .tar, .rar etc and uses XChaCha as cipher.
The software was built with C++ (std=23) and CryptoPP: https://github.com/weidai11/cryptopp

<h2 align="left"> Tools </h2>

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

<h2 align="left"> Future version </h2>

- V2 version: V1 version + Aes256-gcm and also encrypts folders.
- V3 version: V2 version + cascade cipher XChaCha-Aes (xchacha over aes).
