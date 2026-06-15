#ifndef AES_H
#define AES_H

#include <cryptopp/cryptlib.h>
#include <cryptopp/secblock.h>
#include <cryptopp/aes.h>
#include <cryptopp/gcm.h>
#include <cryptopp/files.h>
#include <cryptopp/osrng.h>
#include <cryptopp/sha.h>
#include <cryptopp/hkdf.h>
#include <cryptopp/hex.h>

#include <string>

bool aesfilefolder(std::string mode, std::string filePath, std::string password);

#endif
