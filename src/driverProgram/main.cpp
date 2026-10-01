#include <iostream>
#include <string>
#include <vector>
#include <memory>

#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/color.hpp>

#include "../core/aegis.hpp"
#include "../aegisencryption/encryption.hpp"
#include "../aegisdecryption/decryption.hpp"

using namespace ftxui;

// helper function to render about() header inside FTXUI
Element RenderAboutHeader() {
	return vbox({
		text(""),
		text("Andry RAFAM ANDRIANJAFY - June 2026") | dim | hcenter | color(Color::White),
		text("E-mail: andryrafam@protonmail.com") | dim | hcenter | color(Color::White),
		text("Website: https://github.com/andryrafam") | dim | hcenter | color(Color::White),
		text("Version - 1.6.9") | dim | hcenter | color(Color::White),
		text(""),
		text("Aegis is free software, and comes with ABSOLUTELY NO WARRANTY.") | dim | hcenter | color(Color::White),
	});
}

// main function

int main() {
    auto a = std::make_unique<Aegis>();
    auto e = std::make_unique<Encryption>();
    auto d = std::make_unique<Decryption>();

    while(true) {
		int mode_selection = 0;
		const std::vector<std::string> entries = {"Encrypt","Decrypt"};

		auto menu = Radiobox(&entries, &mode_selection);

		auto menu_with_auto_select = CatchEvent(menu, [&](Event event) {
			if((event==Event::ArrowDown || event==Event::Character('j')) && mode_selection < (int)entries.size() - 1) {
				mode_selection++;
			} else if((event==Event::ArrowUp || event==Event::Character('k')) && mode_selection > 0) {
				mode_selection--;
			}
			return false;
		});

		auto screen = ScreenInteractive::Fullscreen();

		auto btn_proceed = Button("Proceed", [&] {
			a->action_selection = Aegis::AppMode::Proceed;
			screen.ExitLoopClosure()();
		});

		auto btn_exit = Button("Exit", [&] {
			a->action_selection = Aegis::AppMode::Exit;
			screen.ExitLoopClosure()();
		});

		auto action_column = Container::Vertical({btn_proceed,btn_exit});
		auto main_container = Container::Horizontal({menu_with_auto_select,action_column});

		// left / right arrow
		auto container_with_events = CatchEvent(main_container, [&](Event event) {
			// pressing right arrow from the menu highlights proceed
			if(menu_with_auto_select->Focused() && (event==Event::ArrowRight || event==Event::Character('l'))) {
				//btn_proceed->TakeFocus();
				action_column->TakeFocus();
				return true;
			}
			// pressing left arrow on proceed returns focus back to the menu
			if(btn_proceed->Focused() && (event==Event::ArrowLeft || event == Event::Character('h'))) {
				menu_with_auto_select->TakeFocus();
				return true;
			}
			return false;
		});

		auto renderer = Renderer(container_with_events, [&] {
			std::string description;
			
			if(btn_exit->Focused()) {
				description = "Exit the program";
			} else if(btn_proceed->Focused()) {
				description = (mode_selection==0) ? "Proceed to Encrypion" : "Proceed to Decryption";
			} else {
				description = (mode_selection==0) ? "Encrypt mode" : "Decrypt mode";
			}

			// combine the about header banner and the menu into one centered window
			auto content_box = window(
				text(" | Aegis TUI | ") | hcenter | bold | color(Color::White),
				vbox({
					RenderAboutHeader(), // about the program
					separator(),
					text(""),

					// Side by side layout
					hbox({
						filler(),

						// -- LEFT COLUMN: text, Encrypt and Decrypt
						vbox({
							text("Please select an option") | hcenter | bold | color(Color::White),
							text(""),
							menu->Render(),
						}) | center,

						filler(),
						separator(), // vertical divider line
						filler(),

						// -- RIGHT COLUMN: Proceed and Exit buttons
						vbox({
							btn_proceed->Render(),
							text("   "), // space between proceed and exit buttons
							btn_exit->Render(),
						}) | center,

						filler(),
					}),
					
					text(""),
					separator(),
					text(description) | dim | hcenter,
				})
			) | size(WIDTH, EQUAL, 68);

			return vbox({
				filler(),
				hbox({
					filler(),
					content_box,
					filler(),
				}),
				filler(),
			});
		});

		screen.Loop(renderer);

		switch(a->action_selection) {
			case Aegis::AppMode::Exit:
				std::cout << "\033[H\033[J"; // clear the screen
				std::cout << "Program Terminated.\n\n";
				return 0;

			case Aegis::AppMode::Proceed:
				a->clearScreen();
				if(mode_selection==0) {
					if(!e->encryptionMode()) return 0;
				} else if (mode_selection==1) {
					if(!d->decryptionMode()) return 0;
				}
				break;
			case Aegis::AppMode::Go_Back:
    		default:
        		break; // Deliberately do nothing for Go_Back or unexpected modes
		}
	}
}
