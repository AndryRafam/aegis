#include <vector>
#include <iostream>
#include <fstream>

#include <ftxui/component/captured_mouse.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/color.hpp>

#include "encryption.hpp"

constexpr std::string_view RESET = "\033[0m";
constexpr std::string_view HIGHLIGHT = "\033[7m";
constexpr std::string_view BOLD_RED = "\033[1;31m";

using namespace ftxui;

bool Encryption::encryptionMode() {
    
    this->clearScreen(); // clear the screen

    std::cout << "\n";
    std::cout << "ENCRYPTION MODE" << "\n\n";
    std::string path = this->getValidPath();
    std::cout << "\n";
    
    while(true) {
        int cipher_selection = 0;
        const std::vector<std::string> ciphers = {
            "Aes256-GCM",
		    "SM4-GCM",
            "XChaCha20Poly1305"
        };

        auto menu = Menu(&ciphers, &cipher_selection);
        auto screen = ScreenInteractive::TerminalOutput();

        ButtonOption option;
        option.transform = [](const EntryState& state) {
            auto button_box = text(state.label) | hcenter;
            if(state.focused) {
                return button_box | border | bold | inverted;
            }
            return button_box | border | dim;
        };

        auto btn_proceed = Button("Proceed", [&] {
            action_selection = Aegis::AppMode::Proceed;
            screen.ExitLoopClosure()();
        }, option);

        auto btn_back = Button("Back", [&] {
            action_selection = Aegis::AppMode::Go_Back;
            screen.ExitLoopClosure()();
        }, option);

        auto action_column = Container::Vertical({btn_proceed,btn_back});
        auto main_container = Container::Horizontal({menu,action_column});

        // left / right arrow
        auto container_with_events = CatchEvent(main_container, [&](Event event) {
            // pressing right arrow from the menu highlights proceed
            if(menu->Focused() && (event==Event::ArrowRight)) {
                btn_proceed->TakeFocus();
                return true;
            }
            // pressing left arrow on proceed returns focus back to the menu
            if(btn_proceed->Focused() && (event==Event::ArrowLeft)) {
                menu->TakeFocus();
                return true;
            }
            return false;
        });

        auto renderer = Renderer(container_with_events, [&] {
            std::string description;

            if(btn_back->Focused()) {
                description = "Back to the Main Menu";
            } else if(btn_proceed->Focused()) {
                switch(cipher_selection) {
                    case 0:
                        description = "Encrypt with AES.";
                        break;
                    case 1:
                        description = "Encrypt with SM4.";
                        break;
                    case 2:
                        description = "Encrypt with XChaCha20.";
                        break;
                }
            } else {
                switch(cipher_selection) {
                    case 0:
                        description = "Original name Rijndael. Winner of the AES contest.";
                        break;
                    case 1:
                        description = "ShāngMì 4 - Standardised for commercial cryptography in China.";
                        break;
                    case 2:
                       description = "Extended version of ChaCha20.";
                       break; 
                }
            }

            // combine the about header
            auto content_box = window(
                text(""),
                vbox({
                    text(""),
                    hbox({
                        filler(),
                        // -- LEFT COLUMN
                        vbox({
                            text("Please select a cipher") | hcenter | bold | color(Color::White),
                            text(""),
                            menu->Render(),
                        }) | center,

                        filler(),
                        separator(),
                        filler(),

                        // -- RIGHT COLUMN
                        vbox({
                            btn_proceed->Render(),
                            text("  "),
                            btn_back->Render(),
                        }) | center,

                        filler(),
                    }),

                    text(""),
                    text(description) | dim | hcenter,
                })
            ) | size(WIDTH, EQUAL, 64);

            return vbox({
                filler(),
                hbox({
                    content_box,
                    filler(),
                }),
                filler(),
            });
        });

        screen.Loop(renderer);
    
    //std::cout << "\033[?25h"; // restore cursor
    std::string password;
    
    switch(action_selection) {
        case AppMode::Go_Back:
            std::cout << "\033[?25h";
            return true;
        
        case AppMode::Proceed: {
            this->clearScreen();

            // generate password randomly
            int random_length = this->getRandomInt(16,32);
	        password = this->generatePassword(random_length);
            std::cout << "\n\n";
            std::cout << HIGHLIGHT << ciphers[cipher_selection] << " Cipher Selected" << RESET << "\n\n";
	        std::cout << "Generated Password >: " << password << "\n";
            
            // run selected cipher
            if(cipher_selection==0) this->aes_cipher("encrypt", path, password);
            else if(cipher_selection==1) this->sm4_cipher("encrypt", path, password);
            else if(cipher_selection==2) this->xchacha20_cipher("encrypt", path, password);
            break;
        }

        case AppMode::Exit:
        default:
            break;
    }

	    // wipe password contents
	    this->secure_clear(password);

	    std::cout << "\n" << "Encrypted Successfully" << "\n";
	    std::cout << BOLD_RED << "Warning: " << RESET << "Do not lose your password or you will not recover your data." << "\n\n";

	    if(this->askYN("Continue ?")) return true;

	    std::cout << "\033[H\033[J"; // clear the screen
	
        std::cout << "Program Terminated.\n\n";
	    return false;
    }
}
