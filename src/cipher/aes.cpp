#include <fstream>
#include <cstdio>
#include <stdexcept>
#include <iostream>

#include <cryptopp/cryptlib.h>
#include <cryptopp/secblock.h>
#include <cryptopp/aes.h>
#include <cryptopp/gcm.h>
#include <cryptopp/files.h>
#include <cryptopp/osrng.h>
#include <cryptopp/argon2.h>
#include <cryptopp/hex.h>
#include <cryptopp/filters.h>

#include "../core/aegis.hpp"

constexpr std::string_view RESET = "\033[0m";
constexpr std::string_view BOLD_RED = "\033[1;31m";

using namespace CryptoPP;

bool Aegis::aes_cipher(const std::string& mode, const std::string& filePath, const std::string& password) {

    // number of threads
    const int threads = 4;

    // temporary file to avoid data loss
    std::string tempfile = filePath+".tmp";
    std::string tempfile_hex = filePath+".tmphex";

    const int IV_SIZE = 12; // iv size
    const int TAG_SIZE = 16; // standard 128-bit authentication tag
    const int SALT_SIZE = 16; // argon2 recommends at least 16 bytes

    try {

        AutoSeededRandomPool rng;

        /*encryption mode*/
        if(mode=="encrypt") {

            // setup key, IV and unique random salt
            SecByteBlock key(AES::MAX_KEYLENGTH); // 256 bits
            rng.GenerateBlock(key, key.size());

            SecByteBlock iv(IV_SIZE); // 96 bits for GCM
            rng.GenerateBlock(iv, iv.size());

            SecByteBlock salt(SALT_SIZE);
            rng.GenerateBlock(salt, salt.size());

            // derive key using argond2id
            Argon2 argon2(Argon2::ARGON2ID);
            argon2.DeriveKey(key, key.size(),
                (const byte*)password.data(), password.size(),
                salt.data(), salt.size(),
                3, // time cost (iterations)
                65536, // memory cost (64 mb)
                threads // number of threads
            );

            {
                FileSink binarySink(tempfile.c_str());

                // write cipher ID for AES
                byte cipherID = 0x01;
                binarySink.Put(&cipherID, 1);

                // write salt + iv + ciphertext into a temporary binary file
                binarySink.Put(salt, salt.size());
                binarySink.Put(iv, iv.size());

                GCM<AES>::Encryption aes_enc;
                aes_enc.SetKeyWithIV(key, key.size(), iv, iv.size());

                // stream the file payload right after the header data
                FileSource(filePath.c_str(), true,
                    new AuthenticatedEncryptionFilter(aes_enc, new Redirector(binarySink), false, TAG_SIZE)
                );
            }

            // hexadecimal encoding for pretty looking
            FileSource(tempfile.c_str(), true, new HexEncoder(new FileSink(tempfile_hex.c_str())));

            // cleanup and swap files
            std::remove(filePath.c_str());
            std::remove(tempfile.c_str());
            std::rename(tempfile_hex.c_str(), filePath.c_str());

            return true;
        }
        /*decryption mode*/
        else {

            // hexadecimal decoding
            FileSource(filePath.c_str(), true, new HexDecoder(new FileSink(tempfile_hex.c_str())));

            // extract salt and IV out of the decoded temporary file
            std::ifstream in(tempfile_hex.c_str(), std::ios::binary);

            // extract and validate cipher ID
            byte cipherID = 0;
            in.read((char*)&cipherID, 1);
            if (in.gcount()!=1) throw std::runtime_error("File truncated: Missing Cipher ID.");
            if (cipherID != 0x01) throw std::runtime_error("Cipher ID mismatch: This file was not encrypted with AES.");
            
            // extract salt and iv
            SecByteBlock salt(SALT_SIZE);
            in.read((char*)salt.data(), salt.size());
            if (in.gcount() != SALT_SIZE) throw std::runtime_error("File truncated: Missing salt.");

            SecByteBlock iv(IV_SIZE);
            in.read((char*)iv.data(), iv.size());
            if (in.gcount() != IV_SIZE) throw std::runtime_error("File truncated: Missing IV.");

            // re derive the exact key using the extracted salt
            SecByteBlock key(AES::MAX_KEYLENGTH);
            Argon2 argon2(Argon2::ARGON2ID);
            argon2.DeriveKey(
                key, key.size(),
                (const byte*)password.data(), password.size(),
                salt, salt.size(),
                3, 65536, threads // must exactly match encryption parameters
            );

            // decryption object
            GCM<AES>::Decryption aes_dec;
            aes_dec.SetKeyWithIV(key, key.size(), iv, iv.size());
            
            AuthenticatedDecryptionFilter df(aes_dec, new FileSink(tempfile.c_str()));
            FileSource(in, true, new Redirector(df));

            // only swap files if GCM authenticated successfully
            if(true==df.GetLastResult()) {
                in.close(); // release file handle before deleting
                std::remove(filePath.c_str());
                std::remove(tempfile_hex.c_str());
                std::rename(tempfile.c_str(), filePath.c_str());
                return true;
            } else {
                throw::std::runtime_error("\nAuthentication failed.\n");
            }
        }
    }

    catch(Exception& ex) {
        std::cout << BOLD_RED << "\nError: Wrong password or Corrupted data.\n";
		std::cout << ex.what() << "\n";
		std::cout << "Cannot decrypt." << RESET << "\n\n";

        // remove the tempfile even if decryption failed
        std::remove(tempfile.c_str());
        std::remove(tempfile_hex.c_str());

        return false;
    }
}
