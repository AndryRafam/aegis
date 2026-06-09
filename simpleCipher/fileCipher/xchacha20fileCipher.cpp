#include <iostream>

#include "xchacha20fileCipher.hpp"

#define yellow "\x1B[33m"
#define reset "\x1B[0m"

using namespace CryptoPP;

void xchacha20_file(std::string mode, std::string filePath, std::string password) {
	
	// temporary file to avoid
	// data loss
	std::string tempfile = filePath+".tmp";
	std::string tempfile_hex = filePath+".tmphex"; // temporary file for hexadecimal encoding
	
	try {
		
		std::string salt(""); // null salt string
		
		AutoSeededRandomPool prng;
		HKDF<SHA256> hkdf; // hash key derivation function based on hmac
		
		const size_t XCHACHA20_KEY_SIZE = 32; // key length 32 bytes
		const size_t XCHACHA20_NONCE = 24; // nonce length 24 bytes
		const size_t TAG_SIZE = 16; // tag size 16 bytes
		
		SecByteBlock key(XCHACHA20_KEY_SIZE);
		SecByteBlock nonce(XCHACHA20_NONCE);
		prng.GenerateBlock(key, key.size());
		prng.GenerateBlock(nonce, nonce.size());
		
		hkdf.DeriveKey(key, key.size(), (const byte*)password.data(), password.size(),
			(const byte*)salt.data(), salt.size(), NULL, 0);
		hkdf.DeriveKey(nonce, nonce.size(), (const byte*)password.data(), password.size(),
			(const byte*)salt.data(), salt.size(), NULL, 0);
			
		// encryption
		if(mode=="encrypt" || mode=="e") {
		
			// encryption object
			XChaCha20Poly1305::Encryption encryptor;
			encryptor.SetKeyWithIV(key, key.size(), nonce, nonce.size());
			FileSource(filePath.c_str(), true, new AuthenticatedEncryptionFilter(encryptor, new FileSink(tempfile.c_str()), false, TAG_SIZE));
			
			// hexadecimal encoding
			// for pretty looking
			FileSource(tempfile.c_str(), true, new HexEncoder(new FileSink(tempfile_hex.c_str())));
			
			std::remove(filePath.c_str()); // remove the former filePath
			std::remove(tempfile.c_str()); // remove the tempfile
			std::rename(tempfile_hex.c_str(), filePath.c_str()); // rename the hexadecimal encrypted file
			
			std::cout << "\e[1m" << yellow << "Encrypted" << "\e[0m" << reset << "\n\n";
			
			return;
		}
		// decryption
		else {
			
			// hexadecimal decoding
			// before decrypting
			FileSource(filePath.c_str(), true, new HexDecoder(new FileSink(tempfile_hex.c_str())));
			
			// decryption object
			XChaCha20Poly1305::Decryption decryptor;
			decryptor.SetKeyWithIV(key, key.size(), nonce, nonce.size());
			AuthenticatedDecryptionFilter df(decryptor, new FileSink(tempfile.c_str()));
			FileSource(tempfile_hex.c_str(), true, new Redirector(df));
			
			if(df.GetLastResult()==true) {
				std::cout << "\e[1m" << yellow << "Decrypted" << "\e[0m" << reset << "\n\n";
			}
			
			std::remove(filePath.c_str()); // remove the former filePath
			std::remove(tempfile_hex.c_str()); // remove the intermediate file
			std::rename(tempfile.c_str(), filePath.c_str()); // rename the decrypted file
			
			return;
			
		}
	}
	
	catch(Exception& ex) {
		std::cout << std::endl;
		std::cout << ex.what() << std::endl;
		std::cout << "\e[1m" << "Decryption failed" << "\e[0m" << std::endl;
		std::cout << "\e[1m" << yellow << "Verify your password. Try again." << "\e[0m" << reset << "\n\n";
		
		// remove the temporary file even decryption failed.
		std::remove(tempfile.c_str());
		std::remove(tempfile_hex.c_str());
		exit(1);
	}
	return;
}
