//title_screen.cpp
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include "title_screen.h"
#include "Windows.h"
#include "board.h"
#include "piece.h"

//function to print first title screen and wait for enter press
void title_screen::show_first_screen()
{
    //printing white rectangle which defines the area the chess game uses
    //the game cannot be resized after the chess game is started, therefore it is necessary to show the user how large their window must be before the game starts
    for (int i{ 0 }; i < 80; i++) {
        std::cout << "\xDB";
    }
    for (short i{ 0 }; i < 29; i++) {
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), COORD{0,i});
        std::cout << "\xDB";
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), COORD{ 79,i });
        std::cout << "\xDB";
    }
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), COORD{ 0,29 });
    for (int i{ 0 }; i < 80; i++) {
        std::cout << "\xDB";
    }

    //printing the word CHESS for title
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), COORD{ 12,10 });
    std::cout << " ______     __  __     ______     ______     ______    ";
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), COORD{ 12,11 });
    std::cout << "/\\  ___\\   /\\ \\_\\ \\   /\\  ___\\   /\\  ___\\   /\\  ___\\   ";
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), COORD{ 12,12 });
    std::cout << "\\ \\ \\____  \\ \\  __ \\  \\ \\  __\\   \\ \\___  \\  \\ \\___  \\  ";
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), COORD{ 12,13 });
    std::cout << " \\ \\_____\\  \\ \\_\\ \\_\\  \\ \\_____\\  \\/\\_____\\  \\/\\_____\\ ";
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), COORD{ 12,14 });
    std::cout << "  \\/_____/   \\/_/\\/_/   \\/_____/   \\/_____/   \\/_____/ ";
    
    //printing the instructions to the user
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), COORD{ 9,16 });
    std::cout << "Please resize window until the white rectangle is visible.";
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), COORD{ 25,17 });
    std::cout << "Press enter to continue.";

    std::string enter;
    std::getline(std::cin, enter);
}

//function to return either a loaded board or a new board
board title_screen::get_board()
{
    while (true) {

        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), COORD{ 9,16 });
        std::cout << "                                                               ";
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), COORD{ 1,17 });
        std::cout << "                                                                              ";
        
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), COORD{ 9,16 });
        std::string load_or_new_choice;
        std::cout << "Would you like to load a save file <L> or start a new game <N>?";
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), COORD{ 9,17 });
        std::getline(std::cin, load_or_new_choice);
        if (load_or_new_choice == "L" || load_or_new_choice == "l")
        {
            return load_board();
        }
        else if (load_or_new_choice == "N" || load_or_new_choice == "n") {
            board new_board;
            return new_board;
        }
        else {
            continue;
        }
    }
  
}

//function to load board from a save file
board title_screen::load_board()
{
    //clear the lines from the title screen
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), COORD{ 9,16 });
    std::cout << "                                                               ";
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), COORD{ 1,17 });
    std::cout << "                                                                              ";

    //get the users slot choice and 
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), COORD{ 9,16 });
    std::string slot_choice_string;
    std::istringstream slot_choice_ss{ -1 };
    int slot_choice;
    while (true) {
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), COORD{ 1,17 });
        std::cout << "                                                                              ";
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), COORD{ 9,16 });
        std::cout << "Which save slot would you like to use? (Enter 0-9)";
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), COORD{ 9,17 });
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

    //construct a new board to load the save into and open the save slot
    board load_board;
    std::ifstream save_slot;
    save_slot.open("save_slot_" + std::to_string(slot_choice) + ".dat");
    save_slot >> load_board;
    //if the save slot failbit is set then the save file is invalid
    if (save_slot.fail()) {
        save_slot.close();
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), COORD{ 9,16 });
        std::cout << "                                                               ";
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), COORD{ 1,17 });
        std::cout << "                                                                              ";
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), COORD{ 9,16 });
        std::cout << "Save slot failed to load. Press enter to continue.";
        std::string enter;
        std::getline(std::cin, enter);
        //runs the function get_board, which causes recursion until a valid save is loaded or a new board is created
        return get_board();
    }
    else {
        save_slot.close();
        return load_board;
    }
}