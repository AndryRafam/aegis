#ifndef SM4_H
#define SM4_H

#include <fstream>
#include <cstdio>
#include <stdexcept>

#include <cryptopp/cryptlib.h>
#include <cryptopp/secblock.h>
#include <cryptopp/sm4.h>
#include <cryptopp/gcm.h>
#include <cryptopp/files.h>
#include <cryptopp/osrng.h>
#include <cryptopp/argon2.h>
#include <cryptopp/hex.h>
#include <cryptopp/filters.h>

#include <string>

bool sm4filefolder(std::string mode, std::string filePath, std::string password);

#endif
