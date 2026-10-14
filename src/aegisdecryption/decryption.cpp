#include <vector>
#include <iostream>
#include <fstream>

#include <ftxui/component/captured_mouse.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/component/component_options.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/component/event.hpp>
#include <ftxui/component/screen_interactive.hpp>

#include "decryption.hpp"

bool Decryption::decryptionMode() {
	using namespace ftxui;

    this->clearScreen();
	auto screen = ScreenInteractive::TerminalOutput();
	std::cout << "\n";
	std::cout << "DECRYPTION MODE" << "\n\n";
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

	InputOption password_option;
	password_option.password = true;
	password_option.transform = [](InputState state) {
		if(state.is_placeholder) {
			state.element |= dim;
		}
		return state.element;
	};

	// password input zone
	Component input_password = Input(&password, "", password_option);
	Component secure_input = CatchEvent(input_password, [&](Event event) {
		if(event==Event::Return) {
			screen.ExitLoopClosure()();
			return true;
		}
		return false;
	});

	auto component = Container::Vertical({
		secure_input,
	});

	auto renderer = Renderer(component, [&] {
		return vbox({
			text(""),
			window(text(" Password "), secure_input->Render()),
		}) | size(WIDTH, EQUAL, 48);
	});

	secure_input->TakeFocus();
	screen.Loop(renderer);

	std::cout << "\nFile to Process: " << path << "\n";

	bool success = false;

	if(cipherID=="01") success = this->aes_cipher("decrypt", path, password);
	else if(cipherID=="02") success = this->sm4_cipher("decrypt", path, password);
	else if(cipherID=="03") success = this->xchacha20_cipher("decrypt", path, password);
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
