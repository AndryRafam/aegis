#include <iostream>
#include <string>
#include <string_view>
#include <filesystem>
#include <fstream>
#include <termios.h>
#include <unistd.h>
#include <algorithm> // required for std::fill

#include "../cipher/xchacha20.hpp"
#include "../cipher/sm4.hpp"
#include "../cipher/aes.hpp"
#include "../cipher/twofish.hpp"
#include "../password/password_generator.hpp"
#include "../password/set_echo.hpp"

constexpr std::string_view RESET = "\033[0m";
// constexpr std::string_view HIGHLIGHT = "\033[7m";
constexpr std::string_view HIGHLIGHT = "\033[30;44m";
constexpr std::string_view BOLD = "\033[1m";
constexpr std::string_view BOLD_RED = "\033[1;31m";

namespace fs = std::filesystem;

enum class AppMode {
	Encrypt,
	Decrypt,
	Exit,
	Proceed,
	Go_Back
};

// forward declaration
void about();
std::string getValidPath();
char getch();
bool askToContinue();
bool askManually(); // ask to input password manually
void clearScreen();
void secure_clear(std::string& s); // wipe string contents
bool encryptionMode();
bool decryptionMode();

/*=======================================================*/
// main function
/*=======================================================*/

int main() {
	while(true) {
		clearScreen();

		AppMode select_mode = AppMode::Encrypt; // start with encrypt mode highligthed by default
		char ch;

		std::cout << "\033[?25l"; // hide cursor

		// part of code to interact with the mode choice: encrypt, decrypt or quit.
		while (true) {
			std::cout << "Choose mode using left/right key arrows and press enter:\n";

			// line encrypt
			if(select_mode==AppMode::Encrypt) {
				std::cout << "  " << HIGHLIGHT << BOLD << "< ENCRYPT >" << RESET << " ";
			} else {
				std::cout << BOLD << "  < ENCRYPT > " << RESET;
			}

			// line decrypt
			if(select_mode==AppMode::Decrypt) {
				std::cout << "  " << HIGHLIGHT << BOLD << "< DECRYPT >" << RESET << " ";
			} else {
				std::cout << BOLD << "  < DECRYPT > " << RESET;
			}

			// line exit
			if(select_mode==AppMode::Exit) {
				std::cout << "  " << HIGHLIGHT << BOLD << "< EXIT >" << RESET << "\n";
			} else {
				std::cout << BOLD << "  < EXIT >" << RESET << "\n";
			}

			// Dynamic description Line
			std::cout << "\n"; // 1. add an extra empty line
			std::cout << "\033[K"; // 2. clear the line to prevent "ghost text"
			if(select_mode==AppMode::Exit) {
				std::cout << "                             Exit the Program\n";
			} else {
				std::cout << "\n";
			}

			ch = getch();

			if(ch==27) { // ascii value for escape
				getch(); // discard the intermediate '[' character
				switch (getch()) { // read the final arrow character ('A' or 'B') directly
					case 'D': // left arrow (wrap around logic)
						if(select_mode==AppMode::Encrypt) select_mode = AppMode::Exit;
						else if(select_mode==AppMode::Decrypt) select_mode = AppMode::Encrypt;
						else if(select_mode==AppMode::Exit) select_mode = AppMode::Decrypt;
						break;
					case 'C': // right arrow (wrap around logic)
						if(select_mode==AppMode::Encrypt) select_mode = AppMode::Decrypt;
						else if(select_mode==AppMode::Decrypt) select_mode = AppMode::Exit;
						else if(select_mode==AppMode::Exit) select_mode = AppMode::Encrypt; 
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
		if(select_mode==AppMode::Exit) {
			clearScreen();
			std::cout << "Program Terminated.\n\n";
			return 0; // exit main function right here
		}

		if(select_mode==AppMode::Encrypt) {
			if(!encryptionMode()) return 0;
		} else if(select_mode==AppMode::Decrypt) {
			if(!decryptionMode()) return 0;
		}
	}
}

/*================================================================*/
// Helper Function
/*================================================================*/

void about() {
	const std::string aboutText = R"( NeptuneCrypt 1.6.2, Encryption Software, June 2026
 Andry RAFAM ANDRIANJAFY <andryrafam@protonmail.com>
 https://github.com/andryrafam

 NeptuneCrypt is free software, and
 comes with ABSOLUTELY NO WARRANTY.
)";
	std::cout << aboutText << std::endl;
}

std::string getValidPath() {
	std::string filePath;
	while(true) {
		std::cout << "File absolute path >: ";
		std::getline(std::cin, filePath);

		if(fs::is_regular_file(filePath)) return filePath;
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
		std::cout << "Continue ? [Y/n] >: ";
		if(!(std::cin >> yn)) return false; // safely check for EOF/Ctrl+D
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

		if(yn=='y' || yn=='Y') return true;
		if(yn=='n' || yn=='N') return false;
		std::cout << "Invalid input. Only [y/n].\n"; // invalid input try again
	}
}

// ask to input password manually
bool askManually() {
	char yn; // [y/n]
	while(true) {
		std::cout << "Input password manually ? [Y/n] >: ";
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

// wipe password content
void secure_clear(std::string& s) {
	std::fill(s.begin(), s.end(), '\0');
}

// encryption mode
bool encryptionMode() {
	clearScreen();
	std::cout << BOLD << "Enrolling Encryption Mode" << RESET << std::endl;
	std::cout << BOLD << "=========================" << RESET << "\n\n";
	std::string filePath = getValidPath();

	const std::vector<std::string> ciphers = {
		"Aes256-GCM",
		"SM4-GCM",
		"Twofish-EAX",
        "XChaCha20Poly1305"
	};

	const std::vector<std::string> about_ciphers = {
		"Original name Rijndael. Winner of the AES contest.",
		"ShāngMì 4 - Standardised for commercial cryptography in China.",
		"AES contest finalist developed by Bruce Schneier.",
		"Extended version of ChaCha20."
	};

	size_t cipher_selection = 0;
	AppMode action_selection = AppMode::Proceed; // start with proceed
	char ch;

	std::cout << "\033[?25l"; // hide cursor

	// select cipher interactive loop
	while(true) {
		std::cout << "\nSelect cipher and choose < Proceed > using key arrows:\n";

		for(size_t i = 0; i < ciphers.size(); ++i) {
			if (cipher_selection == i) {
                std::cout << "  > " << HIGHLIGHT << BOLD << ciphers[i] << RESET << "\n";
            } else {
                std::cout << "    " << BOLD << ciphers[i] << RESET << "\n";
            }
		}

		std::cout << "\n";
		if(action_selection==AppMode::Proceed) std::cout << "    " << HIGHLIGHT << BOLD << "< PROCEED >" << RESET << "  ";
		else std::cout << BOLD << "    < PROCEED >  " << RESET;

		if(action_selection==AppMode::Go_Back) std::cout << "  " << HIGHLIGHT << BOLD << "< GO BACK >" << RESET << "\n";
		else std::cout << BOLD << "  < GO BACK >" << RESET << "\n";

		std::cout << "\n\033[K";
		if(action_selection==AppMode::Go_Back) std::cout << "                 Back to Main Menu\n";
		else std::cout << " " << about_ciphers[cipher_selection] << "\n";

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
                case 'D': // Left arrow (wrap around logic)
                    if(action_selection==AppMode::Proceed) action_selection = AppMode::Go_Back;
					else if(action_selection==AppMode::Go_Back) action_selection = AppMode::Proceed;
                    break;
                case 'C': // Right arrow (wrap around logic)
                    if(action_selection==AppMode::Go_Back) action_selection = AppMode::Proceed;
					else if(action_selection==AppMode::Proceed) action_selection = AppMode::Go_Back;
                    break;
			}
		} else if(ch==10) {
			if(action_selection==AppMode::Go_Back) {
				std::cout << "\033[?25h";
				return true; // go back to main menu loop
			}
			break;
		}
		std::cout << "\033[" << ciphers.size() + 6 << "A"; // Redraw menu dynamically
	}

	std::cout << "\033[?25h"; // restore cursor

	// initialize random number for password legnth [16,32]
	// Initialize random number for password length [16,32]
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<int> distrib(16, 32);
    std::string password, confirm_password;

    clearScreen();
    std::string cipherName = ciphers[cipher_selection];
    std::cout << BOLD << cipherName << " Cipher Selected" << RESET << "\n\n";

	if(askManually()) {
		while(true) {
			std::cout << "Password >: ";
			setEcho(false);
			std::getline(std::cin, password);
			setEcho(true);
			std::cout << "\n";
			std::cout << "Confirm Password >: ";
			setEcho(false);
			std::getline(std::cin, confirm_password);
			setEcho(true);
			std::cout << "\n";

			if(password==confirm_password) {
				break;
			}
			std::cout << BOLD_RED << "Password does not match. Try again." << RESET << std::endl;
		}
	}
	else {
		password = generatePassword(distrib(gen));
		std::cout << "Generated Password >: " << password << "\n";
	}

	// run selected cipher
	if(cipher_selection==0) aes_cipher("encrypt", filePath, password);
	else if(cipher_selection==1) sm4_cipher("encrypt", filePath, password);
	else if(cipher_selection==2) twofish_cipher("encrypt", filePath, password);
	else if(cipher_selection==3) xchacha20_cipher("encrypt", filePath, password);

	// wipe password contents
	secure_clear(password);
	secure_clear(confirm_password);

	std::cout << "\n" << BOLD << "Encrypted Successfully" << RESET << "\n";
	std::cout << BOLD_RED << "CRITICAL: Do not lose your password or you will not recover your data." << RESET << "\n\n";

	if(askToContinue()) return true;

	clearScreen();
	std::cout << "Program Terminated.\n\n";
	return false;
}

// decryption mode
bool decryptionMode() {
	clearScreen();
	std::cout << BOLD << "Enrolling Decryption Mode" << RESET << std::endl;
	std::cout << BOLD << "=========================" << RESET << "\n\n";
	std::string filePath = getValidPath();

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
	std::cout << "\n";
	std::cout << "\nFile to Process: " << filePath << "\n";

	bool success = false;

	if(cipherID=="01") success = aes_cipher("decrypt", filePath, password);
	else if(cipherID=="02") success = sm4_cipher("decrypt", filePath, password);
	else if(cipherID=="03") success = twofish_cipher("decrypt", filePath, password);
	else if(cipherID=="04") success = xchacha20_cipher("decrypt", filePath, password);
	else {
		secure_clear(password); // wipe password contents
		std::cout << "\nCannot decrypt. Encryption algorithm not recognized." << "\n\n";
		std::cout << "Program Terminated.\n\n";
        return false;
	}

	// wipe password contents
	secure_clear(password);

	if(success) std::cout << "\n" << BOLD << "Decrypted Successfully" << RESET << "\n";
	if(askToContinue()) return true;

	clearScreen();
	std::cout << "Program Terminated.\n\n";
	return false;
}
