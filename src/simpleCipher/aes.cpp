#include <iostream>

#include "aes.hpp"

using namespace CryptoPP;

bool aesfilefolder(std::string mode, std::string filePath, std::string password) {

    // temporary file to avoid
    // data loss
    std::string tempfile = filePath+".tmp";
    std::string tempfile_hex = filePath+".tmphex";

    try {

        std::string salt(""); // null salt string

        AutoSeededRandomPool rng;
        HKDF<SHA256> hkdf; // hash key derivation function based on hmac

        // Setup key and Initialization Vector (IV)
        SecByteBlock key(AES::MAX_KEYLENGTH); // 32 bytes (256 bits)
        rng.GenerateBlock(key, key.size());
        SecByteBlock iv(12); // 12 bytes (96 bits) for GCM
        rng.GenerateBlock(iv, iv.size());

        const int TAG_SIZE = 16; // standard 128-bit authentication tag

        hkdf.DeriveKey(key, key.size(), (const byte*)password.data(), password.size(),
                (const byte*)salt.data(), salt.size(), NULL, 0);
        hkdf.DeriveKey(iv, iv.size(), (const byte*)password.data(), password.size(),
                (const byte*)salt.data(), salt.size(), NULL, 0);

        // encryption
        if(mode=="encrypt") {

            // encryption object
            GCM<AES>::Encryption aes_enc;
            aes_enc.SetKeyWithIV(key, key.size(), iv, iv.size());
            FileSource(filePath.c_str(), true, new AuthenticatedEncryptionFilter(aes_enc, new FileSink(tempfile.c_str()), false, TAG_SIZE));

            // hexadecimal encoding
            // for pretty looking
            FileSource(tempfile.c_str(), true, new HexEncoder(new FileSink(tempfile_hex.c_str())));

            // remove non needed file
            std::remove(filePath.c_str());
            std::remove(tempfile.c_str());
            // rename the hex tempfile
            std::rename(tempfile_hex.c_str(), filePath.c_str());

            return true;
        }
        // decryption
        else {

            // hexadecimal decoding
            FileSource(filePath.c_str(), true, new HexDecoder(new FileSink(tempfile_hex.c_str())));

            // decryption object
            GCM<AES>::Decryption aes_dec;
            aes_dec.SetKeyWithIV(key, key.size(), iv, iv.size());
            AuthenticatedDecryptionFilter df(aes_dec, new FileSink(tempfile.c_str()));
            FileSource(tempfile_hex.c_str(), true, new Redirector(df));

            if(true==df.GetLastResult()) {
                std::remove(filePath.c_str());
                std::remove(tempfile_hex.c_str());
                std::rename(tempfile.c_str(), filePath.c_str());
            }
            return true;
        }
    }

    catch(Exception& ex) {
        std::cout << std::endl;
        std::cout << "Attempting to decrypt Aes256-GCM" << "\n";
		std::cout << ex.what() << "\n";

        // remove the tempfile even if decryption failed
        std::remove(tempfile.c_str());
        std::remove(tempfile_hex.c_str());

        return false;
    }
}
