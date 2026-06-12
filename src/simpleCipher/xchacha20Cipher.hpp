#ifndef XCHACHA20CIPHER_H
#define XCHACHA20CIPHER_H

#include <cryptopp/cryptlib.h>
#include <cryptopp/secblock.h>
#include <cryptopp/chachapoly.h>
#include <cryptopp/files.h>
#include <cryptopp/osrng.h>
#include <cryptopp/sha.h>
#include <cryptopp/hkdf.h>
#include <cryptopp/hex.h>

#include <string>

bool xchacha20filefolder(std::string mode, std::string filePath, std::string password);

#endif
