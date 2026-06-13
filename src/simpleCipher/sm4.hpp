#ifndef SM4_H
#define SM4_H

#include <cryptopp/cryptlib.h>
#include <cryptopp/secblock.h>
#include <cryptopp/sm4.h>
#include <cryptopp/gcm.h>
#include <cryptopp/files.h>
#include <cryptopp/osrng.h>
#include <cryptopp/sha.h>
#include <cryptopp/hkdf.h>
#include <cryptopp/hex.h>

#include <string>

bool sm4filefolder(std::string mode, std::string filePath, std::string password);

#endif