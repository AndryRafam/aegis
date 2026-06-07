![](https://img.shields.io/badge/Code-C++-orange.svg?style=for-the-badge&logo=c%2B%2B)
![](https://img.shields.io/badge/OS-Linux-orange.svg?style=for-the-badge&logo=Linux)
![](https://img.shields.io/badge/Algorithm-XCHACHA20POLY1305-orange.svg?style=for-the-badge&logo)
![](https://img.shields.io/badge/Algorithm-SHA-orange.svg?style=for-the-badge&logo)

<h1 align="left"> NeptuneCrypt (v1) </h1>

![Output](https://github.com/AndryRafam/Program-Output/blob/master/enc_dec_neptune.gif)

<h2 algin="left"> About </h2>

NeptuneCrypt is a lightweight CLI encryption (decryption) software using XChaCha20Poly1305, Aes256-gcm as ciphers; XChaCha-Aes for cascade cipher (xchacha over aes) and SHA256 as hash key derivation function. This V1 version encrypts files only, including compressed files such as .7z, .zip, .tar, .rar etc and uses XChaCha as cipher.
The software was built with C++ (std=23) and CryptoPP: https://github.com/weidai11/cryptopp

<h3 align="left"> Coming up </h3>

- V2 version: V1 version + Aes256-gcm as cipher and also encrypts folders.
- V3 version: V2 version + cascade cipher XChaCha-Aes (xchacha over aes).
