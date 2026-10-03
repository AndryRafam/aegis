#include <vector>
#include <iostream>
#include <fstream>

#include "decryption.hpp"

bool Decryption::decryptionMode() {
    this->clearScreen();
	std::cout << "\n";
	std::cout << "Decryption Mode" << "\n";
	std::cout << "===============" << "\n\n";
	std::string path = this->getValidPath();

	std::ifstream file(path, std::ios::binary);
	if(!file) {
		std::cout << "\nError opening file for reading.\n";
		if(this->askYN("Continue ?")) return true;
		return false;
	}

	char header[2];
	file.read(header, 2);
	file.close();

	std::string cipherID(header, 2);
	std::string password;

	std::cout << "Enter Password >: ";
	this->setEcho(false);
	std::getline(std::cin, password);
	this->setEcho(true);
	std::cout << "\n";
	std::cout << "\nFile to Process: " << path << "\n";

	bool success = false;

	if(cipherID=="01") success = this->aes_cipher("decrypt", path, password);
	else if(cipherID=="02") success = this->sm4_cipher("decrypt", path, password);
	else if(cipherID=="03") success = this->twofish_cipher("decrypt", path, password);
	else if(cipherID=="04") success = this->xchacha20_cipher("decrypt", path, password);
	else {
		this->secure_clear(password); // wipe password contents
		std::cout << "\nCannot decrypt. Encryption algorithm not recognized." << "\n\n";
		std::cout << "Program Terminated.\n\n";
        return false;
	}

	// wipe password contents
	this->secure_clear(password);

	if(success) std::cout << "\n" << "Decrypted Successfully" << "\n";
	if(this->askYN("Continue ?")) return true;

	std::cout << "\033[H\033[J"; // clear the screen
	
	std::cout << "Program Terminated.\n\n";
	return false;
}
