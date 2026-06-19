#include <iostream>
#include <string>
#include <filesystem>
#include <fstream>
#include <termios.h>
#include <unistd.h>

#include "../simpleCipher/xchacha20Cipher.hpp"
#include "../simpleCipher/sm4.hpp"
#include "../simpleCipher/aes.hpp"
#include "../password/password_generator.hpp"
#include "../password/set_echo.hpp"

#define yellow "\x1B[33m"
#define reset "\x1B[0m"
#define highlight "\x1B[7m"

// helper function to show about the program
void about() {
	const std::string aboutText = R"(NeptuneCrypt-v1.0.3, Encryption Software, June 2026
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
		std::cout << "\e[1m" << "File doesn't exist" << "\e[0m" << std::endl;
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
		std::cout << "Invalid input. Only [y/n]\n"; // invalid input try again
	}
}
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
			std::cout << " > " << highlight << "[ Encrypt ]" << reset << "  ";
		} else {
			std::cout << "   [ Encrypt ]  ";
		}

		// line decrypt
		if(select_mode==1) {
			std::cout << " > " << highlight << "[ Decrypt ]" << reset << "  ";
		} else {
			std::cout << "   [ Decrypt ]  ";
		}

		// line exit
		if(select_mode==2) {
			std::cout << " > " << highlight << "[ Exit ]" << reset << "\n";
		} else {
			std::cout << "   [ Exit ]\n";
		}

		// Dynamic description Line
		std::cout << "\n"; // 1. add an extra empty line
		std::cout << "\033[K"; // 2. clear the line to prevent "ghost text"
		if(select_mode==0) {
			std::cout << "\n";
		} else if(select_mode==1) {
			std::cout << "\n";
		} else if(select_mode==2) {
			std::cout << "                               Exit the Program\n";
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

		int selection = 0; // initialize selection

		std::cout << "\033[?25l"; // hide cursor

		// select cipher interactive
		while (true) {
			std::cout << "\nSelect cipher, use up/down key arrows, and then press enter:\n";

			// SM4-GCM cipher mode
			if(selection==0) {
				std::cout << " > " << highlight << "SM4-GCM" << reset << "\n";
			} else {
				std::cout << "   SM4-GCM\n";
			}

			// XChaCha20Poly1305 cipher mode
			if(selection==1) {
				std::cout << " > " << highlight << "XChaCha20Poly1305" << reset << "\n";
			} else {
				std::cout << "   XChaCha20Poly1305\n";
			}

			// Aes256-GCM cipher mode
			if(selection==2) {
				std::cout << " > " << highlight << "Aes256-GCM" << reset << "\n";
			} else {
				std::cout << "   Aes256-GCM\n";
			}

			ch = getch();

			if(ch==27) { // ascii value for escape
				getch(); // discard the intermediate '[' character
				switch (getch()) { // read the final arrow character ('A' or 'B') directly
					case 'A': // up arrow
						// if at top, wrap around to bottom, otherwise decrement
						selection = (selection==0) ? 2 : selection-1;
						break;
					case 'B': // down arrow
						selection = (selection==2) ? 0 : selection + 1; 
						break;
				}
			} else if(ch==10) {
				break; // enter key
			}

			// move up 5 lines to redraw seamlessly
			std::cout << "\033[5A";
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
		}

		if(selection==0) { // SM4-GCM
			
			std::cout << "\033[H\033[J"; // clear the screen
			about();
			std::cout << "\e[1m" << "SM4-GCM Cipher Selected" << "\e[0m" << "\n\n";
			std::cout << "Generated Password >: " << password << std::endl;
			sm4filefolder(mode, filePath, password);
			std::cout << "\e[1m" << "Encrypted Successfully" << "\e[0m" << "\n\n";
			
			if(askToContinue()) goto main_menu;
			else {
				std::cout << "\033[H\033[J"; // clear the screen
				about();
				std::cout << "Program Terminated.\n\n";
				return 0;
			}
		}
		else if(selection==1) { // XChaCha20Poly1305
			
			std::cout << "\033[H\033[J"; // clear the screen
			about();
			std::cout << "\e[1m" << "XChaCha20Poly1305 Cipher Selected" << "\e[0m" << "\n\n";
			std::cout << "Generated Password >: " << password << std::endl;
			xchacha20filefolder(mode, filePath, password);
			std::cout << "\e[1m" << "Encrypted Successfully" << "\e[0m" << "\n\n";

			if(askToContinue()) goto main_menu;
			else {
				std::cout << "\033[H\033[J"; // clear the screen
				about();
				std::cout << "Program Terminated.\n\n";
				return 0;
			}
		}
		else if(selection==2) { // Aes256-GCM

			std::cout << "\033[H\033[J"; // clear the screen
			about();
			std::cout << "\e[1m" << "Aes256-GCM Cihper Selected" << "\e[0m" << "\n\n";
			std::cout << "Generated Password >: " << password << std::endl;
			aesfilefolder(mode, filePath, password);
			std::cout << "\e[1m" << "Encrypted Successfully" << "\e[0m" << "\n\n";
			
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
		
		std::string password;
		std::cout << "Enter Password >: ";
		setEcho(false); // disable mirroring input to the screen
		std::getline(std::cin, password); // password will not be displayed for security reason
		setEcho(true); //
		std::cout << std::endl;
		if(sm4filefolder(mode, filePath, password)) {
			std::cout << "\n\e[1m" << "SM4-GCM Decrypted Successfully" << "\e[0m" << "\n\n";

			if(askToContinue()) goto main_menu;
			else {
				std::cout << "\033[H\033[J"; // clear the screen
				about();
				std::cout << "Program Terminated.\n\n";
				return 0;
			}
		}
		else if(xchacha20filefolder(mode, filePath, password)) {
			std::cout << "\n\e[1m" << "XChaCha20Poly1305 Decrypted Successfully" << "\e[0m" << "\n\n";

			if(askToContinue()) goto main_menu;
			else {
				std::cout << "\033[H\033[J"; // clear the screen
				about();
				std::cout << "Program Terminated.\n\n";
				return 0;
			}
		}
		else if(aesfilefolder(mode, filePath, password)) {
			std::cout << "\n\e[1m" << "Aes256-GCM Decrypted Successfully" << "\e[0m" << "\n\n";

			if(askToContinue()) goto main_menu;
			else {
				std::cout << "\033[H\033[J"; // clear the screen
				about();
				std::cout << "Program Terminated.\n\n";
				return 0;
			}
		}
		// default
		else {
			std::cout << "\nCannot decrypt." << "\n\n";

			if(askToContinue()) goto main_menu;
			else {
				std::cout << "\033[H\033[J"; // clear the screen
				about();
				std::cout << "Program Terminated.\n\n";
				return 0;
			};
		}
	}
	return 0;		
}
	
