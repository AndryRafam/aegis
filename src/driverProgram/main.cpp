#include <iostream>
#include <string>
#include <string_view>
#include <filesystem>
#include <fstream>
#include <termios.h>
#include <unistd.h>

#include "../cipher/xchacha20.hpp"
#include "../cipher/sm4.hpp"
#include "../cipher/aes.hpp"
#include "../cipher/twofish.hpp"
#include "../password/password_generator.hpp"
#include "../password/set_echo.hpp"

constexpr std::string_view RESET = "\033[0m";
constexpr std::string_view HIGHLIGHT = "\033[7m";
constexpr std::string_view BOLD = "\e[1m";
constexpr std::string_view BOLD_RED = "\033[1;31m";

// forward declaration
void about();
std::string getValidFilePath();
char getch();
bool askToContinue();
void clearScreen();
bool encryptionMode();
bool decryptionMode();

/*=======================================================*/
// main function
/*=======================================================*/

int main() {
	while(true) {
		clearScreen();

		int select_mode = 0; // encrypt or decrypt
		char ch;

		std::cout << "\033[?25l"; // hide cursor

		// part of code to interact with the mode choice: encrypt, decrypt or quit.
		while (true) {
			std::cout << "Choose mode using left/right key arrows and press enter:\n";

			// line encrypt
			if(select_mode==0) {
				std::cout << "  " << HIGHLIGHT << "< Encrypt >" << RESET << " ";
			} else {
				std::cout << "  < Encrypt > ";
			}

			// line decrypt
			if(select_mode==1) {
				std::cout << "  " << HIGHLIGHT << "< Decrypt >" << RESET << " ";
			} else {
				std::cout << "  < Decrypt > ";
			}

			// line exit
			if(select_mode==2) {
				std::cout << "  " << HIGHLIGHT << "< Exit >" << RESET << "\n";
			} else {
				std::cout << "  < Exit >\n";
			}

			// Dynamic description Line
			std::cout << "\n"; // 1. add an extra empty line
			std::cout << "\033[K"; // 2. clear the line to prevent "ghost text"
			if(select_mode==0) {
				std::cout << "\n";
			} else if(select_mode==1) {
				std::cout << "\n";
			} else if(select_mode==2) {
				std::cout << "                              Exit the Program\n";
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

			// move 4 lines to redraw seamlessly
			std::cout << "\033[4A"; 
		}

		std::cout << "\033[?25h"; // restore cursor

		// if the user choice is "Exit", exit the program immediately
		if(select_mode==2) {
			clearScreen();
			std::cout << "Program Terminated.\n\n";
			return 0; // exit main function right here
		}

		if(select_mode==0) {
			if(!encryptionMode()) return 0;
		} else if(select_mode==1) {
			if(!decryptionMode()) return 0;
		}
	}
}

/*================================================================*/
// Helper Function
/*================================================================*/

void about() {
	const std::string aboutText = R"( NeptuneCrypt 1.6, Encryption Software, June 2026
 Andry RAFAM ANDRIANJAFY <andryrafam@protonmail.com>
 https://github.com/andryrafam

 NeptuneCrypt is free software, and
 comes with ABSOLUTELY NO WARRANTY.
)";
	std::cout << aboutText << std::endl;
}

std::string getValidFilePath() {
	std::string filePath;
	while(true) {
		std::cout << "File absolute path >: ";
		std::getline(std::cin, filePath);

		if(std::filesystem::is_regular_file(filePath)) {
			return filePath;
		}
		// if file doesn't exist repeat the process
		std::cout << BOLD << "File doesn't exist." << RESET << std::endl;
	}
}

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

void clearScreen() {
	std::cout << "\033[H\033[J"; // clear the screen
	about();
}

// encryption mode
bool encryptionMode() {
	clearScreen();
	std::cout << BOLD << "Enrolling Encryption Mode" << RESET << std::endl;
	std::string filePath = getValidFilePath();

	const std::vector<std::string> ciphers = {
		"SM4-GCM",
        "XChaCha20Poly1305",
        "Aes256-GCM",
        "Twofish-EAX"
	};

	size_t cipher_selection = 0;
	int action_selection = 0; // 0 = proceed, 1 = go back
	char ch;

	std::cout << "\033[?25l"; // hide cursor

	// select cipher interactive loop
	while(true) {
		std::cout << "\nSelect cipher and choose < Proceed > using key arrows:\n";

		for(size_t i = 0; i < ciphers.size(); ++i) {
			if (cipher_selection == i) {
                std::cout << "  > " << HIGHLIGHT << ciphers[i] << RESET << "\n";
            } else {
                std::cout << "    " << ciphers[i] << "\n";
            }
		}

		std::cout << "\n";
		if(action_selection==0) std::cout << "    " << HIGHLIGHT << "< Proceed >" << RESET << "  ";
		else std::cout << "    < Proceed >  ";

		if(action_selection==1) std::cout << "  " << HIGHLIGHT << "< Go Back >" << RESET << "\n";
		else std::cout << "  < Go Back >\n";

		std::cout << "\n\033[K";
		if(action_selection==1) std::cout << "                 Back to Main Menu\n";
		else std::cout << "\n";

		ch = getch();

		if(ch==27) {
			getch();
			switch(getch()) {
				case 'A': // Up arrow
                    cipher_selection = (cipher_selection == 0) ? ciphers.size() - 1 : cipher_selection - 1;
                    break;
                case 'B': // Down arrow
                    cipher_selection = (cipher_selection == ciphers.size() - 1) ? 0 : cipher_selection + 1; 
                    break;
                case 'D': // Left arrow
                    action_selection = (action_selection == 0) ? 1 : action_selection - 1;
                    break;
                case 'C': // Right arrow
                    action_selection = (action_selection == 1) ? 0 : action_selection + 1;
                    break;
			}
		} else if(ch==10) {
			if(action_selection==1) {
				std::cout << "\033[?25h";
				return true; // go back to main menu loop
			}
			break;
		}
		std::cout << "\033[" << ciphers.size() + 6 << "A"; // Redraw menu dynamically
	}

	std::cout << "\033[?25h"; // restore cursor

	// initialize random number for password legnth [16,32]
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<int> distrib(16, 32);
    std::string password = generatePassword(distrib(gen));

    clearScreen();
    std::string cipherName = ciphers[cipher_selection];
    std::cout << BOLD << cipherName << " Cipher Selected" << RESET << "\n\n";
    std::cout << "Generated Password >: " << password << "\n";

	// run selected cipher
	if(cipher_selection==0) sm4_cipher("encrypt", filePath, password);
	else if(cipher_selection==1) xchacha20_cipher("encrypt", filePath, password);
	else if(cipher_selection==2) aes_cipher("encrypt", filePath, password);
	else if(cipher_selection==3) twofish_cipher("encrypt", filePath, password);

	std::cout << "\n" << BOLD << "Encrypted Successfully" << RESET << "\n";
	std::cout << BOLD_RED << "CRITICAL: Do not lose your password or you will not recover your data." << RESET << "\n";

	if(askToContinue()) return true;

	clearScreen();
	std::cout << "Program Terminated.\n\n";
	return false;
}

// decryption mode
bool decryptionMode() {
	clearScreen();
	std::cout << BOLD << "Enrolling Decryption Mode" << RESET << std::endl;
	std::string filePath = getValidFilePath();

	std::ifstream file(filePath, std::ios::binary);
	if(!file) {
		std::cout << "\nError opening file for reading.\n";
		if(askToContinue()) return true;
		return false;
	}

	char header[2];
	file.read(header, 2);
	file.close();

	std::string cipherID(header, 2);
	std::string password;

	std::cout << "Enter Password >: ";
	setEcho(false);
	std::getline(std::cin, password);
	setEcho(true);
	std::cout << std::endl;

	bool success = false;

	if(cipherID=="01") success = sm4_cipher("decrypt", filePath, password);
	else if(cipherID=="02") success = xchacha20_cipher("decrypt", filePath, password);
	else if(cipherID=="03") success = aes_cipher("decrypt", filePath, password);
	else if(cipherID=="04") success = twofish_cipher("decrypt", filePath, password);
	else {
		std::cout << "\nCannot decrypt. Encryption algorithm not recognized." << "\n\n";
		std::cout << "Program Terminated.\n\n";
        return false;
	}

	if(success) std::cout << "\n" << BOLD << "Decrypted Successfully" << RESET << "\n";
	if(askToContinue()) return true;

	clearScreen();
	std::cout << "Program Terminated.\n\n";
	return false;
}
