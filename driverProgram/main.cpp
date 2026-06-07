/* THIS PART OF THE PROGRAM WILL BE MODIFIED WITH V2 VERSION */

#include <iostream>
#include <string>
#include <filesystem>
#include <fstream>
#include <termios.h>
#include <unistd.h>

#include "../xchacha20file/xchacha20fileCipher.hpp"
#include "../password/password_generator.hpp"

#define yellow "\x1B[33m"
#define reset "\x1B[0m"

// function used to not output
// the password when typing
void setEcho(bool enable);

// main function
int main(/*int argc, char **argv*/) {
		
	std::ios::sync_with_stdio(false);
	
	system("clear"); // clear the screen
	
	std::ifstream infile;
	std::string line;
	infile.open("about.txt");
	while(std::getline(infile, line)) {
		std::cout << "\e[1m" << line << "\e[0m" << std::endl;
	}
	infile.close();
	
	// encryption or decryption
	std::cout << std::endl;
	std::cout << "encrypt/decrypt (e/d) ? >: ";
	std::string mode;
	std::cin >> mode;
	std::cin.ignore();
	
	// this part of the code
	// will be available on V2 version
	/*std::cout << "Target type ? (file/folder) >: ";
	std::string target_type;
	std::cin >> target_type;
	std::cin.ignore();*/
	
	// encryption
	if (mode=="e" || mode=="encrypt") {
	
		std::cout << "\e[1m" << "Encryption mode" << "\e[0m" << std::endl;
		
		label_1_file_path:
			std::cout << "File absolute path >: ";
			std::string filePath;
			std::getline(std::cin, filePath);
			
			// if file exist
			// proceed for encryption
			if(std::filesystem::exists(filePath)) {
				
				int passLen; // define password len
				do {
					std::cout << "Password length must be >= 16 and <= 256" << std::endl;
					std::cout << "Password length ? >: ";
					std::cin >> passLen;
					std::cin.ignore();
				} while(passLen < 16 || passLen > 256); // for security reason, password should be at least 16 characters long
				
				std::string password_xchacha20 = generatePassword(passLen);
				std::cout << "Genrated Password >: " << password_xchacha20 << std::endl;
				xchacha20_file(mode, filePath, password_xchacha20);
			}
			// if file doesn't exist repeat the process
			else {
				std::cout << "\e[1m" << yellow << "File doesn't exist" << "\e[0m" << reset << std::endl;
				goto label_1_file_path; // repeat the process until a valid is input
			}
	}
	// decryption
	else if(mode=="d" || mode=="decrypt") {
		std::cout << "\e[1m" << "Decryption mode" << "\e[0m" << std::endl;
		
		label_2_file_path:
			std::cout << "File absolute path >: ";
			std::string filePath;
			std::getline(std::cin, filePath);
			
			// if file exist,
			// proceed for decryption
			if(std::filesystem::exists(filePath)) {
				std::string password_xchacha20;
				std::cout << "Enter your password >: ";
				setEcho(false); // disable mirroring input to the screen
				std::getline(std::cin, password_xchacha20); // password will not be outputed for security reason
				setEcho(true); //
				std::cout << std::endl;
				xchacha20_file(mode, filePath, password_xchacha20);
			}
				// if file doesn't exist repeat the process
			else {
				std::cout << "\e[1m" << yellow << "File doesn't exist" << "\e[0m" << reset << std::endl;
				goto label_2_file_path; // repeat the process until a valid is input
			}
	}
	// default
	else {
		std::cout << "\e[1m" << "Invalid choice. Program terminated." << "\e[0m" << std::endl;
		return 0;
	}
	return 0;		
}

// function setEcho
void setEcho(bool enable) {
	struct termios tty;
	// get the current terminal attributes
	tcgetattr(STDIN_FILENO, &tty);
	
	if(!enable) {
		// clear the ECHO flag
		tty.c_lflag &= ~ECHO;
	}
	else {
		tty.c_lflag |= ECHO;
	}
	
	// apply the modified attributes
	tcsetattr(STDIN_FILENO, TCSANOW, &tty);
}
