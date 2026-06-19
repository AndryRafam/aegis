#ifndef XCHACHA20CIPHER_H
#define XCHACHA20CIPHER_H

#include <fstream>
#include <cstdio>
#include <stdexcept>

#include <cryptopp/cryptlib.h>
#include <cryptopp/secblock.h>
#include <cryptopp/chachapoly.h>
#include <cryptopp/files.h>
#include <cryptopp/osrng.h>
#include <cryptopp/argon2.h>
#include <cryptopp/hex.h>
#include <cryptopp/filters.h>

#include <string>

bool xchacha20filefolder(std::string mode, std::string filePath, std::string password);

#endif

