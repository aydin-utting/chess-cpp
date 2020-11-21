/*
Chess game written in C++ by Aydin Utting
Last edited on 09/05/2020

Game for two players to compete against each other.
Calculates allowed moves for all pieces, stopping players from checking themselves
Includes En Passant and Castling
Determines if the game has reached checkmate or stalemate by simulating possible moves
Allows for saving and loading of game into 10 slots
*/
#include <iostream>
#include <typeinfo>
#include <memory>
#include <array>
#include <string>
#include <sstream>
#include <fstream>
#include "Windows.h"
#include "piece.h"
#include "board.h"
#include "time.h"
#include "title_screen.h"


int main()
{
	//attempting to open all files in ios::in, and if this fails creating the file
	//this creates all 10 save slots and makes sure the files exist
	for (char i{ '0' }; i <= '9'; i++) {
		std::string name = "save_slot_" + std::string(1, i) + ".dat";
		std::ifstream save_slot(name.c_str());
		if (!save_slot.good()) {
			save_slot.close();
			std::ofstream new_save_slot(name);
			new_save_slot.close();
		}
	}

	//initialising the title screen object and displaying the title screen.
	title_screen t;
	t.show_first_screen();

	//initialising the game board object b1 with the title screen function get board, allows for loading save or new board
	board b1{ t.get_board() };

	//start by printing the board and the pieces in their initial positions.
	b1.print_board();
	b1.print_pieces();
	std::array<colour, 2> player_colour{ {colour::White,colour::Black} };
	int turn_code{ 0 };
	//turn number is returned to start game on correct players turn
	int turn_number{ b1.get_turn_number() };
	while (true) {
		//board::turn returns a code, the colour is determined by number of turns modulo 2, as white always starts
		int turn_code = b1.turn(player_colour[turn_number % 2]);
		if (turn_code == 0) {
			//if turn_code == 0, then the turn did not complete the game, and the user did not choose to save or quit
			//so the number of turns increases, so the next turn will be the other colour
			turn_number++;
		}
		else if (turn_code == 1) {
			//code if checkmate reached.
			//changes cursor position to the bottom of the board, so that the board message is printed below the board.
			SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), COORD{ 0,26 });
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
			std::cout << "   ____ _   _ _____ ____ _  ____  __    _  _____ _____ _ " << std::endl;
			std::cout << "  / ___| | | | ____/ ___| |/ /  \\/  |  / \\|_   _| ____| |" << std::endl;
			std::cout << " | |   | |_| |  _|| |   | ' /| |\\/| | / _ \\ | | |  _| | |" << std::endl;
			std::cout << " | |___|  _  | |__| |___| . \\| |  | |/ ___ \\| | | |___|_|" << std::endl;
			std::cout << "  \\____|_| |_|_____\\____|_|\\_\\_|  |_/_/   \\_\\_| |_____(_)" << std::endl;
			if (b1.get_board_in_check() == colour::Black) {
				std::cout << "White Wins!" << std::endl;
			}
			else {
				std::cout << "Black Wins!" << std::endl;
			}
			//break while loop, ending game.
			break;
		}
		else if (turn_code == 2) {
			//stalemate code
			SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), COORD{ 0,26 });
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
			std::cout << " ____  _        _                      _       " << std::endl;
			std::cout << "/ ___|| |_ __ _| | ___ _ __ ___   __ _| |_ ___ " << std::endl;
			std::cout << "\\___ \\| __/ _` | |/ _ \\ '_ ` _ \\ / _` | __/ _ \\" << std::endl;
			std::cout << " ___) | || (_| | |  __/ | | | | | (_| | ||  __/" << std::endl;
			std::cout << "|____/ \\__\\__,_|_|\\___|_| |_| |_|\\__,_|\\__\\___|" << std::endl;
			std::cout << "Its a Draw!" << std::endl;
			break;
		}
		else if (turn_code == -1) {
			//code runs on user input of "exit"
			SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), COORD{ 0,26 });
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
			std::cout << "\33[2K";
			std::cout << "Thanks for playing!";
			break;
		}
		else if (turn_code == -2) {
			//code block runs on user input of save
			//loop runs until the user inputs a valid integer 0-9
			std::string slot_choice_string;
			std::istringstream slot_choice_ss{ -1 };
			int slot_choice;
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
			while (true) {
				SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), COORD{ 0,26 });
				std::cout << "\33[2K";
				std::cout << "Which save slot would you like to use? (Enter 0-9) (Slot will be overwritten)" << std::endl;
				std::cout << "\33[2K";
				std::getline(std::cin, slot_choice_string);
				slot_choice_ss.str(slot_choice_string);
				slot_choice_ss >> slot_choice;
				if (slot_choice_ss.fail() || slot_choice < 0 || slot_choice>9) {
					slot_choice_ss.clear();
					continue;
				}
				else {
					break;
				}
			}
			
			//opens chosen save slot in ios::trunc state, to overwrite anything stored there.
			std::ofstream save_slot;
			save_slot.open("save_slot_" + std::to_string(slot_choice) + ".dat", std::ios::out | std::ios::trunc);
			if (save_slot.good()) {
				save_slot << b1;
				save_slot.close();
			}
			else {
				SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), COORD{ 0,26 });
				std::cout << "\33[2K";
				std::cout << "Failed to save game. Press enter to continue." << std::endl;
				std::cout << "\33[2K";
				std::string enter;
				std::getline(std::cin, enter);
				save_slot.close();
				continue;
			}

			//loops until user chooses one of the two options. Then continues or exits game
			std::string exit_choice{ "" };
			while (exit_choice != "e" && exit_choice != "E"&& exit_choice != "c" && exit_choice != "C") {
				SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), COORD{ 0,26 });
				std::cout << "\33[2K";
				std::cout << "Game saved to save slot " + std::to_string(slot_choice) + ". Would you like to exit <E> or continue <C>?" << std::endl;
				std::cout << "\33[2K";
				std::getline(std::cin, exit_choice);
			}
			if (exit_choice == "c" || exit_choice == "C") {
				//erase message line before continuing to next loop, which will be the same player's go, as turn_number is not increased.
				SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), COORD{ 0,26 });
				std::cout << "\33[2K";
				continue;
			}
			else {
				SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), COORD{ 0,26 });
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
				std::cout << "\33[2K";
				std::cout << "Thanks for playing!";
				break;
			}
			
		}
	}
	return 0;

}
