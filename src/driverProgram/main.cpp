/* THIS PART OF THE PROGRAM WILL BE MODIFIED WITH FUTRE UPGRADE*/

#include <iostream>
#include <string>
#include <filesystem>
#include <fstream>
#include <termios.h>
#include <unistd.h>

#include "../simpleCipher/xchacha20Cipher.hpp"
#include "../simpleCipher/sm4.hpp"
//#include "../simpleCipher/aes.hpp"
#include "../password/password_generator.hpp"
#include "../password/set_echo.hpp"

#define yellow "\x1B[33m"
#define reset "\x1B[0m"

// helper function to show about the program
void about() {
	std::cout << std::endl;
	const std::string aboutText = R"(
  =============================================================
   NeptuneCrypt Copyright © June 2026 Andry RAFAM ANDRIANJAFY.
   Password is randomly generated.
	 
   This software comes with ABSOLUTELY NO WARRANTY.
	 
   License: MIT
  =============================================================
	)";
	std::cout << "\e[1m" << aboutText << "\e[0m"; 
}

// helper to safely get an existing path via user input
std::string getValidFilePath() {
	std::string filePath;
	while(true) {
		std::cout << "File absolute path >: ";
		std::getline(std::cin, filePath);

		if(std::filesystem::is_regular_file(filePath)) {
			return filePath;
		}
		// if file doesn't exist repeat the process
		std::cout << "\e[1m" << yellow << "File doesn't exist" << "\e[0m" << reset << std::endl;
	}
}

// main function
int main(/*int argc, char **argv*/) {
		
	std::ios::sync_with_stdio(false);
	
	system("clear"); // clear the screen

	about();

	// encryption or decryption
	std::cout << std::endl;
	std::cout << "encrypt/decrypt (e/d) ? >: ";
	std::string mode;
	std::cin >> mode;
	std::cin.ignore();
	
	// encryption
	if (mode=="e" || mode=="encrypt") {
	
		std::cout << "\e[1mEnrolling Encryption Mode\e[0m" << std::endl;
		std::string filePath = getValidFilePath();

		int cipher_selection = 0; // initialize selection
		while(cipher_selection != 1 && cipher_selection != 2 /*&& cipher_selection*/) {
			std::cout << "\n\e[1mSelect cipher\e[0m\n1: SM4-GCM\n2: XChaCha20Poly1305\nChoice >: ";
			std::cin >> cipher_selection;
			std::cin.ignore();

			if(cipher_selection !=1 && cipher_selection != 2 /*&& cipher_selection !=3*/) {
				std::cout << "\e[1m" << yellow << "Unknown Encryption Cipher" << "\e[0m" << reset << std::endl;
			}
		}

		/*Initialize random number [16,64] length
		using mersene twister*/
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution<int> distrib(16,64);
		int passLen = distrib(gen);
		std::string password = generatePassword(passLen);

		if(cipher_selection==1) { // SM4-GCM
				
			std::cout << "\n" << "\e[1m" << yellow << "SM4-GCM Cihper" << "\e[0m" << reset << std::endl;
			std::cout << "Generated Password >: " << password << std::endl;
			sm4filefolder(mode, filePath, password);
			std::cout << "\e[1m" << yellow << "Encrypted Successfully" << "\e[0m" << reset << "\n\n";
		}
		else if(cipher_selection==2) { // XChaCha20Poly1305
			
			std::cout << "\n" << "\e[1m" << yellow << "XChaCha20Poly1305 Cihper" << "\e[0m" << reset << std::endl;
			std::cout << "Generated Password >: " << password << std::endl;
			xchacha20filefolder(mode, filePath, password);
			std::cout << "\e[1m" << yellow << "Encrypted Successfully" << "\e[0m" << reset << "\n\n";
		}
		/*else if(cipher_selection==3) { // Aes256-GCM

			std::cout << "\n" << "\e[1m" << yellow << "Aes256-GCM Cihper" << "\e[0m" << reset << std::endl;
			std::cout << "Generated Password >: " << password << std::endl;
			aesfilefolder(mode, filePath, password);
			std::cout << "\e[1m" << yellow << "Encrypted Successfully" << "\e[0m" << reset << "\n\n"; 
		}*/
	}
	
	// decryption
	else if(mode=="d" || mode=="decrypt") {
		std::cout << "\e[1mEnrolling Decryption Mode\e[0m" << std::endl;
		std::string filePath = getValidFilePath();
		
		std::string password;
		std::cout << "Enter your password >: ";
		setEcho(false); // disable mirroring input to the screen
		std::getline(std::cin, password); // password will not be outputed for security reason
		setEcho(true); //
		std::cout << std::endl;
		if(sm4filefolder(mode, filePath, password)) {
			std::cout << "\n\e[1m" << yellow << "SM4-GCM - Decrypted Successfully" << "\e[0m" << reset << "\n\n";
		}
		else if(xchacha20filefolder(mode, filePath, password)) {
			std::cout << "\n\e[1m" << yellow << "XChaCha20Poly1305 - Decrypted Successfully" << "\e[0m" << reset << "\n\n";
		}
		/*else if(aesfilefolder(mode, filePath, password)) {
			std::cout << "\e[1m" << yellow << "Aes256-GCM - Decrypted Successfully" << "\e[0m" << reset << "\n\n";
		}*/
		// default
		else {
			std::cout << "\n" << "\e[1m" << "Cannot decrypt. Program terminated." << "\e[0m"  << "\n\n";
			return 0;
		}
	}
	
	// default if not encrypt of decrypt mode
	else {
		std::cout << "\e[1m" << "Invalid choice. Program terminated." << "\e[0m" << "\n\n";
		return 0;
	}
	return 0;		
}
	
