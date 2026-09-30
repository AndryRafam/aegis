#include <vector>
#include <iostream>
#include <fstream>

#include "encryption.hpp"

constexpr std::string_view RESET = "\033[0m";
constexpr std::string_view HIGHLIGHT = "\033[7m";
constexpr std::string_view BOLD_RED = "\033[1;31m";

bool Encryption::encryptionMode() {
    this->clearScreen();
    std::cout << "Enrolling Encryption Mode" << "\n";
    std::cout << "=========================" << "\n\n";
    std::string path = this->getValidPath();

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
    action_selection = AppMode::Proceed; // start with proceed
    char ch;

    if(this->askYN("Select cipher randomly ?")) {
        cipher_selection = this->getRandomInt(0, ciphers.size() - 1);
    }
    else {
        std::cout << "\033[?25l"; // hide cursor

        // select cipher interactive loop
        while(true) {
            std::cout << "\nPlease select a cipher\n\n";

            for(size_t i = 0; i < ciphers.size(); ++i) {
                if(cipher_selection==i) {
                    std::cout << "    " << HIGHLIGHT << ciphers[i] << RESET << "\n";
                } else {
                    std::cout << "    " << ciphers[i] << "\n";
                }
            }

            std::cout << "\n";
            if(action_selection==AppMode::Proceed) std::cout << "    " << HIGHLIGHT << "<Proceed>" << RESET << "  ";
            else std::cout << "    <Proceed>  ";

            if(action_selection==AppMode::Go_Back) std::cout << "  " << HIGHLIGHT << "<Back>" << RESET << "\n";
            else std::cout << "  <Back>" << "\n";

            std::cout << "\n\033[K";
            if(action_selection==AppMode::Go_Back) std::cout << "                 Back to Main Menu\n";
            else std::cout << " " << about_ciphers[cipher_selection] << "\n";

            ch = this->getch();

            if(ch==27) {
                this->getch();
                switch(this->getch()) {
                    case 'A': // Up arrow
                    	cipher_selection = (cipher_selection == 0) ? ciphers.size() - 1 : cipher_selection - 1;
                    	break;
                	case 'B': // Down arrow
                    	cipher_selection = (cipher_selection == ciphers.size() - 1) ? 0 : cipher_selection + 1; 
                    	break;
                	case 'D': // Left arrow (wrap around logic)
                	case 'C': // Right arrow (wrap around logic)
                   		action_selection = (action_selection==AppMode::Proceed) ? AppMode::Go_Back : AppMode::Proceed;
                    	break;
                }
            } else if(ch==10) {
                if(action_selection==AppMode::Go_Back) {
					std::cout << "\033[?25h";
					return true; // go back to main menu loop
				}
				break;
            }
            std::cout << "\033[" << ciphers.size() + 7 << "A"; // Redraw menu dynamically
        }
    }

    std::cout << "\033[?25h"; // restore cursor

    std::string password;
    this->clearScreen();
    std::cout << ciphers[cipher_selection] << " Cipher Selected" << "\n\n";

    // generate password randomly
    int random_length = this->getRandomInt(16, 32);
	password = this->generatePassword(random_length);
	std::cout << "Generated Password >: " << password << "\n";
    
    // run selected cipher
    if(cipher_selection==0) this->aes_cipher("encrypt", path, password);
	else if(cipher_selection==1) this->sm4_cipher("encrypt", path, password);
	else if(cipher_selection==2) this->twofish_cipher("encrypt", path, password);
	else if(cipher_selection==3) this->xchacha20_cipher("encrypt", path, password);

	// wipe password contents
	this->secure_clear(password);

	std::cout << "\n" << "Encrypted Successfully" << "\n";
	std::cout << BOLD_RED << "Warning: " << RESET << "Do not lose your password or you will not recover your data." << "\n\n";

	if(this->askYN("Continue ?")) return true;

	this->clearScreen();
	std::cout << "Program Terminated.\n\n";
	return false;
}
