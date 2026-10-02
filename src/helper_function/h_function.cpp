// helper function

#include <iostream>
#include <algorithm>
#include <termios.h>
#include <unistd.h>
#include <filesystem>

#include "../core/aegis.hpp"

namespace fs = std::filesystem;

// Thread-safe instance initialization
CryptoPP::AutoSeededRandomPool& Aegis::get_rng() {
    thread_local CryptoPP::AutoSeededRandomPool rng;
    return rng;
}

// Generates a random integer in range [min, max] inclusive
int Aegis::getRandomInt(int min, int max) {
    return static_cast<int>(
        get_rng().GenerateWord32(
            static_cast<CryptoPP::word32>(min), 
            static_cast<CryptoPP::word32>(max)
        )
    );
}

void Aegis::about() {
    const std::string aboutText = R"(       | Aegis TUI | 
 
 Andry RAFAM ANDRIANJAFY - June 2026
 E-mail: andryrafam@protonmail.com
 Website: https://github.com/andryrafam
 Version - 1.6.9.1                           
                                                            
 Aegis is free software, and comes with ABSOLUTELY NO WARRANTY.  
)";
	std::cout << aboutText << "\n";
}

std::string Aegis::getValidPath() {
    std::string path;
	while(true) {
		std::cout << "File absolute path >: ";
		std::getline(std::cin, path);

		if(fs::is_regular_file(path)) return path;
		//if(fs::is_directory(path)) return path;
		// if path doesn't exist repeat the process
		std::cout << "Path doesn't exist." << std::endl;
	}
}

char Aegis::getch() {
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

bool Aegis::askYN(std::string_view prompt) {
    char yn; // [y/n]
	while(true) {
		std::cout << prompt << " [Y/n] >: ";
		if(!(std::cin >> yn)) return false; // safely check for EOF/Ctrl+D
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

		if(yn=='y' || yn=='Y') return true;
		if(yn=='n' || yn=='N') return false;
		std::cout << "Invalid input. Only [y/n].\n"; // invalid input try again
	}
}

void Aegis::clearScreen() {
    std::cout << "\033[H\033[J"; // clear the screen
	about();
}

void Aegis::secure_clear(std::string& s) {
    std::fill(s.begin(), s.end(), '\0');
}
