#ifndef XCHACHA20FILECIPHER_H
#define XCHACHA20FILECIPHER_H

#include <cryptopp/cryptlib.h>
#include <cryptopp/secblock.h>
#include <cryptopp/chachapoly.h>
#include <cryptopp/files.h>
#include <cryptopp/osrng.h>
#include <cryptopp/sha.h>
#include <cryptopp/hkdf.h>
#include <cryptopp/hex.h>

#include <string>

void xchacha20_file(std::string mode, std::string filePath, std::string password);

#endif
