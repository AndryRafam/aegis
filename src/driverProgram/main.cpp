#include <iostream>
#include <string>
#include <filesystem>
#include <fstream>
#include <termios.h>
#include <unistd.h>

#include "../simpleCipher/xchacha20.hpp"
#include "../simpleCipher/sm4.hpp"
#include "../simpleCipher/aes.hpp"
#include "../simpleCipher/twofish.hpp"
#include "../password/password_generator.hpp"
#include "../password/set_echo.hpp"

#define reset "\033[0m"
#define highlight "\033[7m"

// helper function
void about();
std::string getValidFilePath();
char getch();
bool askToContinue();
/*=======================================================*/

// main function
int main(/*int argc, char **argv*/) {

	// Main menu
	main_menu:
		std::cout << "\033[H\033[J"; // clear the screen
		about();

	int select_mode = 0; // encryption, decryption, quit
	char ch;

	std::cout << "\033[?25l"; // hide cursor

	// part of code to interact with the mode choice: encrypt, decrypt or quit.
	while (true) {
		std::cout << "Select mode, use left/right key arrows, and then press enter:\n";

		// line encrypt
		if(select_mode==0) {
			std::cout << "  " << highlight << "< Encrypt >" << reset << "  ";
		} else {
			std::cout << "   < Encrypt >  ";
		}

		// line decrypt
		if(select_mode==1) {
			std::cout << "  " << highlight << "< Decrypt >" << reset << "  ";
		} else {
			std::cout << "   < Decrypt >  ";
		}

		// line exit
		if(select_mode==2) {
			std::cout << "  " << highlight << "< Exit >" << reset << "\n";
		} else {
			std::cout << "   < Exit >\n";
		}

		// Dynamic description Line
		std::cout << "\n"; // 1. add an extra empty line
		std::cout << "\033[K"; // 2. clear the line to prevent "ghost text"
		if(select_mode==0) {
			std::cout << "\n";
		} else if(select_mode==1) {
			std::cout << "\n";
		} else if(select_mode==2) {
			std::cout << "                                   Exit the Program\n";
		}

		ch = getch();

		if(ch==27) { // ascii value for escape
			getch(); // discard the intermediate '[' character
			switch (getch()) { // read the final arrow character ('A' or 'B') directly
				case 'D': // left arrow 
					// if at leftmost item, wrap around to the rightmost, otherwise decrement
					select_mode = (select_mode==0) ? 2 : select_mode - 1;
					break;
				case 'C': // right arrow
					// if at rightmost item, wrap around to the leftmost, otherwise increment
					select_mode = (select_mode==2) ? 0 : select_mode + 1; 
					break;
			}
		} else if(ch==10) {
			break; // enter key
		}

		std::cout << "\033[4A"; 
	}

	std::cout << "\033[?25h"; // restore cursor

	// if the user choice is "Exit", exit the program immediately
	if(select_mode==2) {
		std::cout << "\033[H\033[J"; // clear the screen
		about();
		std::cout << "Program Terminated.\n\n";
		return 0; // exit main function right here
	}

	std::string mode = (select_mode==0) ? "encrypt" : "decrypt";
	
	// encryption
	if (mode=="encrypt") {

		std::cout << "\033[H\033[J"; // clear the screen
		about();
		std::cout << "\e[1mEnrolling Encryption Mode\e[0m" << std::endl;
		std::string filePath = getValidFilePath();

		const std::vector<std::string> ciphers = {
			"SM4-GCM",
			"XChaCha20Poly1305",
			"Aes256-GCM",
			"Twofish-EAX"
		};

		size_t selection = 0; // initialize selection

		std::cout << "\033[?25l"; // hide cursor

		// select cipher interactive
		while (true) {
			std::cout << "\nSelect cipher, use up/down key arrows, and then press enter:\n";

			for(size_t i = 0; i < ciphers.size(); ++i) {
				if(selection==i) {
					std::cout << "  > " << highlight << ciphers[i] << reset << "\n";
				} else {
					std::cout << "    " << ciphers[i] << "\n";
				}
			}
			
			ch = getch();

			if(ch==27) { // ascii value for escape
				getch(); // discard the intermediate '[' character
				switch (getch()) { // read the final arrow character ('A' or 'B') directly
					case 'A': // up arrow
						// if at top, wrap around to bottom, otherwise decrement
						selection = (selection==0) ? ciphers.size() : selection-1;
						break;
					case 'B': // down arrow
						selection = (selection==ciphers.size()) ? 0 : selection+1; 
						break;
				}
			} else if(ch==10) {
				break; // enter key
			}

			// move up 5 lines to redraw seamlessly
			std::cout << "\033[6A";
		}

		std::cout << "\033[?25h"; // restore cursor

		/*Initialize random number [16,64] length
		using mersene twister*/
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution<int> distrib(16,64);
		int passLen = distrib(gen);
		std::string password = generatePassword(passLen);

		// cipher name 
		std::string cipher_name;
		if(selection==0) {
			cipher_name = "SM4-GCM";
		} else if(selection==1) {
			cipher_name = "XChaCha20Poly1305";
		} else if(selection==2) {
			cipher_name = "Aes256-GCM";
		} else if(selection==3) {
			cipher_name = "Twofish-EAX";
		}

		// SM4-GCM
		if(selection==0) { 
			
			std::cout << "\033[H\033[J"; // clear the screen
			about();
			std::cout << "\e[1m" << "SM4-GCM Cipher Selected" << "\e[0m" << "\n";
			std::cout << "Generated Password >: " << password << std::endl;
			sm4_cipher(mode, filePath, password);
			std::cout << "\n\e[1m" << "Encrypted Successfully" << "\e[0m" << "\n\n";
			
			if(askToContinue()) goto main_menu;
			else {
				std::cout << "\033[H\033[J"; // clear the screen
				about();
				std::cout << "Program Terminated.\n\n";
				return 0;
			}
		}

		// XChaCha20Poly1305
		else if(selection==1) { 
			
			std::cout << "\033[H\033[J"; // clear the screen
			about();
			std::cout << "\e[1m" << "XChaCha20Poly1305 Cipher Selected" << "\e[0m" << "\n";
			std::cout << "Generated Password >: " << password << std::endl;
			xchacha20_cipher(mode, filePath, password);
			std::cout << "\n\e[1m" << "Encrypted Successfully" << "\e[0m" << "\n\n";

			if(askToContinue()) goto main_menu;
			else {
				std::cout << "\033[H\033[J"; // clear the screen
				about();
				std::cout << "Program Terminated.\n\n";
				return 0;
			}
		}

		// Aes256-GCM
		else if(selection==2) { 

			std::cout << "\033[H\033[J"; // clear the screen
			about();
			std::cout << "\e[1m" << "Aes256-GCM Cihper Selected" << "\e[0m" << "\n";
			std::cout << "Generated Password >: " << password << std::endl;
			aes_cipher(mode, filePath, password);
			std::cout << "\n\e[1m" << "Encrypted Successfully" << "\e[0m" << "\n\n";
			
			if(askToContinue()) goto main_menu;
			else {
				std::cout << "\033[H\033[J"; // clear the screen
				about();
				std::cout << "Program Terminated.\n\n";
				return 0;
			}
		}

		// Twofish-EAX
		else if(selection==3) {
			
			std::cout << "\033[H\033[J"; // clear the screen
			about();
			std::cout << "\e[1m" << "Twofish-EAX Cipher Selected" << "\e[0m" << "\n";
			std::cout << "Generated Password >: " << password << std::endl;
			twofish_cipher(mode, filePath, password);
			std::cout << "\n\e[1m" << "Encrypted Successfully" << "\e[0m" << "\n\n";

			if(askToContinue()) goto main_menu;
			else {
				std::cout << "\033[H\033[J"; // clear the screen
				about();
				std::cout << "Program Terminated.\n\n";
				return 0;
			}
		}
	}
	
	// decryption
	else if(mode=="decrypt") {
		
		std::cout << "\033[H\033[J"; // clear the screen
		about();
		std::cout << "\e[1mEnrolling Decryption Mode\e[0m" << std::endl;
		std::string filePath = getValidFilePath();

		std::ifstream file(filePath);
		
		// read the 2 first characters of the file
		char header[2];
		file.read(header, 2);
		file.close();

		std::string cipherID(header, 2);
		
		std::string password;

		std::cout << "Enter Password >: ";
		setEcho(false); // disable mirroring input to the screen
		std::getline(std::cin, password); // password will not be displayed for security reason
		setEcho(true); //
		std::cout << std::endl;

		// auto select decrypt function based on ID detected
		bool success = false;
		// SM4-GCM
		if(cipherID=="01") success = sm4_cipher(mode, filePath, password);
		// XChaCha20Poly1305
		else if(cipherID=="02") success = xchacha20_cipher(mode, filePath, password);
		// Aes256-GCM
		else if(cipherID=="03") success = aes_cipher(mode, filePath, password);
		// Twofish-EAX
		else if(cipherID=="04") success = twofish_cipher(mode, filePath, password);
		// default
		else {
			std::cout << "\nCannot decrypt. Encryption algorithm not recongnized." << "\n\n";
			std::cout << "Program Terminated.\n\n";
			return 0;
		}

		// decryption successful
		if(success) {
			std::cout << "\n\e[1m" << "Decrypted Successfully" << "\e[0m" << "\n\n";
			if(askToContinue()) goto main_menu;
			else {
				std::cout << "\033[H\033[J"; // clear the screen
				about();
				std::cout << "Program Terminated.\n\n";
				return 0;
			}
		}
		// decryption failed
		else {
			if(askToContinue()) goto main_menu;
			else {
				std::cout << "\033[H\033[J"; // clear the screen
				about();
				std::cout << "Program Terminated.\n\n";
				return 0;
			}
		return 0;
		}
	}
	return 0;
}

/*================================================================*/

void about() {
	const std::string aboutText = R"(NeptuneCrypt 1.6, Encryption Software, June 2026
Andry RAFAM ANDRIANJAFY <andryrafam@protonmail.com>
https://github.com/andryrafam

 NeptuneCrypt is free software, and
 comes with ABSOLUTELY NO WARRANTY.
)";
	std::cout << aboutText << std::endl;
}

// helper function to safely get an existing path via user input
std::string getValidFilePath() {
	std::string filePath;
	while(true) {
		std::cout << "File absolute path >: ";
		std::getline(std::cin, filePath);

		if(std::filesystem::is_regular_file(filePath)) {
			return filePath;
		}
		// if file doesn't exist repeat the process
		std::cout << "\e[1m" << "File doesn't exist." << "\e[0m" << std::endl;
	}
}

// helper function for interactive mode
char getch() {
    struct termios oldt, newt; // old terminal, new terminal
    char ch;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return ch;
}

// helper function to handle user continuation safely
bool askToContinue() {
	char yn; // [y/n]
	while(true) {
		std::cout << "Continue ? [y/n] >: ";
		if(!(std::cin >> yn)) return false; // safely check for EOF/Ctrl+D
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

		if(yn=='y' || yn=='Y') return true;
		if(yn=='n' || yn=='N') return false;
		std::cout << "Invalid input. Only [y/n].\n"; // invalid input try again
	}
}
