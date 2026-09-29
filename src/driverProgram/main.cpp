#include <iostream>
#include <string>
#include <string_view>
#include <filesystem>
#include <fstream>
#include <memory>

#include "../core/aegis.hpp"
#include "../aegisencryption/encryption.hpp"
#include "../aegisdecryption/decryption.hpp"

constexpr std::string_view RESET = "\033[0m";
constexpr std::string_view HIGHLIGHT = "\033[7m";

// main function

int main() {
    auto a = std::make_unique<Aegis>();
    auto e = std::make_unique<Encryption>();
    auto d = std::make_unique<Decryption>();

    while(true) {
		a->clearScreen();

		const std::vector<std::string> mode = {
			"Encrypt",
			"Decrypt"
		};

		size_t mode_selection = 0;

		char ch;

		std::cout << "\033[?25l"; // hide cursor

		// part of code to interact with the mode choice: encrypt or decrypt
		while (true) {
			std::cout << "Please select an option\n\n";
			for(size_t i = 0; i < mode.size(); ++i) {
				if(mode_selection == i) {
					std::cout << "    " << HIGHLIGHT << mode[i] << RESET << "\n";
				} else {
					std::cout << "    " << mode[i] << "\n";
				}
			}

			std::cout << "\n";
			if(a->action_selection==a->AppMode::Proceed) std::cout << "    " << HIGHLIGHT << "<Proceed>" << RESET << "  ";
			else std::cout << "    <Proceed>  ";

			if(a->action_selection==a->AppMode::Exit) std::cout << "  " << HIGHLIGHT << "<Exit>" << RESET << "\n";
			else std::cout << "  <Exit>" << "\n";

			// Dynamic description Line
			std::cout << "\n"; // 1. add an extra empty line
			std::cout << "\033[K"; // 2. clear the line to prevent "ghost text"
			
			(a->action_selection==a->AppMode::Exit) ? std::cout << "                Exit the program\n" : std::cout << "\n";

			ch = a->getch();

			if(ch==27) { // ascii value for escape
				a->getch(); // discard the intermediate '[' character
				switch (a->getch()) {
					case 'A':
						mode_selection = (mode_selection==0) ? mode.size()-1 : mode_selection-1;
						break;
					case 'B':
						mode_selection = (mode_selection==mode.size()-1) ? 0 : mode_selection+1;
						break;
					case 'D': // left arrow (wrap around logic)
					case 'C': // right arrow (wrap around logic)
						a->action_selection = (a->action_selection==a->AppMode::Proceed) ? a->AppMode::Exit : a->AppMode::Proceed;
						break;
				}
			} else if(ch==10) {
				if(a->action_selection==a->AppMode::Exit) {
					a->clearScreen();
					std::cout << "Program Terminated.\n\n";
					std::cout << "\033[?25h"; // restore cursor
					return 0;
				}
				break; // enter key
			}

			// redraw seamlessly
			std::cout << "\033[" << mode.size()+6 << "A"; 
		}

		std::cout << "\033[?25h"; // restore cursor

		if(mode_selection==0) {
			if(!e->encryptionMode()) return 0;
		} else if(mode_selection==1) {
			if(!d->decryptionMode()) return 0;
		}
	}
}
