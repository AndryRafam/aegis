#include <iostream>
#include <thread>

#include "xchacha20Cipher.hpp"

using namespace CryptoPP;

bool xchacha20filefolder(std::string mode, std::string filePath, std::string password) {
	
	// since Argon2id is multithreading
	// check the number of threads
	unsigned int threads = std::thread::hardware_concurrency();

	// temporary file to avoid
	// data loss
	std::string tempfile = filePath+".tmp";
	std::string tempfile_hex = filePath+".tmphex"; // temporary file for hexadecimal encoding
	
	const int XCHACHA20_KEY_SIZE = 32; // key length 32 bytes
	const int XCHACHA20_NONCE_SIZE = 24; // nonce length 24 bytes
	const int TAG_SIZE = 16; // tag size 16 bytes
	const int SALT_SIZE = 16; // Argon2 recommends at least 16 bytes

	try {
		
		AutoSeededRandomPool rng;
		
		/*ENCRYPTION MODE*/
		if(mode=="encrypt") {
			SecByteBlock key(XCHACHA20_KEY_SIZE);
			rng.GenerateBlock(key, key.size());

			SecByteBlock nonce(XCHACHA20_NONCE_SIZE);
			rng.GenerateBlock(nonce, nonce.size());

			SecByteBlock salt(SALT_SIZE);
			rng.GenerateBlock(salt, salt.size());

			// derive key using Argon2id
			Argon2 argon2(Argon2::ARGON2ID);
			argon2.DeriveKey(key, key.size(),
				(const byte*)password.data(), password.size(),
				salt.data(), salt.size(),
				3, // time cost (iterations)
				65536, // memory cost (64 MB)
				threads // number of threads
			);

			//write Salt + IV + Ciphertext into a temporary binary file
			{
				FileSink binarySink(tempfile.c_str());
				binarySink.Put(salt, salt.size());
				binarySink.Put(nonce, nonce.size());

				XChaCha20Poly1305::Encryption xchacha_enc;
				xchacha_enc.SetKeyWithIV(key, key.size(), nonce, nonce.size());

				// stream the file payload right after the header data
				FileSource(filePath.c_str(), true,
					new AuthenticatedEncryptionFilter(xchacha_enc, new Redirector(binarySink), false, TAG_SIZE)
				);
			}
		
			// hexadecimal encoding
			// for pretty looking
			FileSource(tempfile.c_str(), true, new HexEncoder(new FileSink(tempfile_hex.c_str())));
			
			// cleanup and swap files
			std::remove(filePath.c_str()); // remove the former filePath
			std::remove(tempfile.c_str()); // remove the tempfile
			std::rename(tempfile_hex.c_str(), filePath.c_str()); // rename the hexadecimal encrypted file
			
			return true;
		}
		/*DECRYPTION MODE*/
		else {
			
			// hexadecimal decoding
			FileSource(filePath.c_str(), true, new HexDecoder(new FileSink(tempfile_hex.c_str())));
			
			// extract salt and IV out of the decoded temporary file
            std::ifstream in(tempfile_hex.c_str(), std::ios::binary);

			SecByteBlock salt(SALT_SIZE);
			in.read((char*)salt.data(), salt.size());
			if (in.gcount() != SALT_SIZE) throw std::runtime_error("File truncated: Missing salt.");

			SecByteBlock nonce(XCHACHA20_NONCE_SIZE);
			in.read((char*)nonce.data(), nonce.size());
			if (in.gcount() != XCHACHA20_NONCE_SIZE) throw std::runtime_error("File truncated: Missing NONCE.");

			SecByteBlock key(XCHACHA20_KEY_SIZE);
			Argon2 argon2(Argon2::ARGON2ID);
			argon2.DeriveKey(
				key, key.size(),
				(const byte*)password.data(), password.size(),
				salt, salt.size(),
				3, 65536, threads // must exactly match encryption parameters
			);

			// decryption object
			XChaCha20Poly1305::Decryption xchacha_dec;
			xchacha_dec.SetKeyWithIV(key, key.size(), nonce, nonce.size());
			
			AuthenticatedDecryptionFilter df(xchacha_dec, new FileSink(tempfile.c_str()));
			FileSource(in, true, new Redirector(df));
			
			if(true==df.GetLastResult()) {
				in.close(); // release file handle before deleting
				std::remove(filePath.c_str()); // remove the former filePath
				std::remove(tempfile_hex.c_str()); // remove the intermediate file
				std::rename(tempfile.c_str(), filePath.c_str()); // rename the decrypted file
				return true;
			} else {
				throw::std::runtime_error("\nAuthentication failed. Wrong password or corrupted data.\n");
			}
		}
	}
	
	catch(Exception& ex) {
		std::cout << "\nError encountered during XChaCha20Poly1305:\n";
		std::cout << ex.what() << "\n";
		
		// remove the temporary file even decryption failed.
		std::remove(tempfile.c_str());
		std::remove(tempfile_hex.c_str());
		
		return false;
	}
}
