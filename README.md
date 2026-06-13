[![C++](https://img.shields.io/badge/C++-%2300599C.svg?logo=c%2B%2B&logoColor=white)](#)
[![Debian](https://img.shields.io/badge/Debian-A81D33?logo=debian&logoColor=fff)](#)
[![Fedora](https://img.shields.io/badge/Fedora-51A2DA?logo=fedora&logoColor=fff)](#)

<h1 align="left"> PoseidonCrypt </h1>

<h2 algin="left"> About </h2>

PoseidonCrypt (formerly NeptuneCrypt) is a lightweight CLI encryption software using XChaCha20Poly1305, SM4-GCM and Aes-256-GCM (will be added soon) as cipher for simple encryption and SHA256 as hash key derivation function. The software was built with C++ (std=23) and CryptoPP: https://github.com/weidai11/cryptopp

<h2 align="left"> About the cipher </h2>

- XChaCha20Poly1305: https://en.wikipedia.org/wiki/ChaCha20-Poly1305
- SM4-GCM (ShāngMì 4): https://en.wikipedia.org/wiki/SM4_(cipher)
- Aes256-GCM (orginal name Rijndael): https://en.wikipedia.org/wiki/Advanced_Encryption_Standard 

<h2 align="left"> Tools and Requierments for building and debugging </h2>

- Programming Language: C++ (std=17/20/23)
- Debugger: GNU Debugger, Valgrind
- CryptoPP: Version 8.9.0-2
- GNU Make
- GNU C++ Compiler 14.2.0 or compatible

<h2 align="left"> Build, install and run on Linux </h2>
  
To build and install PoseidonCrypt on Linux, go to the directory PoseidonCrypt or PoseidonCrypt-main and type the following command in terminal (super user mode). The executable file will be installed at /usr/local/bin/ directory.

```bash
$ sudo make install
```
To run PoseidonCrypt on Linux, type the following command anywhere in terminal

```bash
$ poseidon
```
<h2 align="left"> How to uninstall and purge </h2>

To uninstall and purge PoseidonCrypt, go to the directory PoseidonCrypt or PoseidonCrypt-main and type the following command in terminal (super user mode).

```bash
$ sudo make uninstall && sudo make clean
```

<h2 align="left"> How to encrypt folders ? </h2>

Encrypting file is straightforward. To encrypt folder, first compress/archive the folder (.7z, .zip, .rar, .tar etc.) and then encrypt with PoseidonCrypt.

<h2 align="left"> Future upgrade </h2>

- Adding Aes256-gcm as third cipher.


