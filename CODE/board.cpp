//board.cpp
#include <array>
#include <vector>
#include <typeinfo>
#include <memory> 
#include <string>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <utility>
#include <map>
#include <iomanip>
#include "Windows.h"
#include "board.h"
#include "empty_piece.h"
#include "piece.h"
#include "bishop.h"
#include "king.h"
#include "knight.h"
#include "pawn.h"
#include "queen.h"
#include "rook.h"

//CONSTRUCTORS

//default constructor
board::board() : board_in_check{ colour::None }, black_king_position{ 4 }, white_king_position{ 60 }, checkmate{ colour::None }, pawn_to_promote{ 0 },
//initiallisation of board_array, creating unique pointers for all pieces
board_array{
	std::make_unique<rook>(0,colour::Black,false), std::make_unique<knight>(1,colour::Black), std::make_unique<bishop>(2,colour::Black) , std::make_unique<queen>(3,colour::Black) , std::make_unique<king>(4,colour::Black,false) , std::make_unique<bishop>(5,colour::Black) , std::make_unique<knight>(6,colour::Black) , std::make_unique<rook>(7,colour::Black,false) ,
	std::make_unique<pawn>(8,colour::Black,0), std::make_unique<pawn>(9,colour::Black,0), std::make_unique<pawn>(10,colour::Black,0), std::make_unique<pawn>(11,colour::Black,0), std::make_unique<pawn>(12,colour::Black,0), std::make_unique<pawn>(13,colour::Black,0), std::make_unique<pawn>(14,colour::Black,0) , std::make_unique<pawn>(15,colour::Black,0) ,
	std::make_unique<empty_piece>(16), std::make_unique<empty_piece>(17), std::make_unique<empty_piece>(18) , std::make_unique<empty_piece>(19) , std::make_unique<empty_piece>(20) , std::make_unique<empty_piece>(21) , std::make_unique<empty_piece>(22), std::make_unique<empty_piece>(23) ,
	std::make_unique<empty_piece>(24), std::make_unique<empty_piece>(25), std::make_unique<empty_piece>(26) , std::make_unique<empty_piece>(27) , std::make_unique<empty_piece>(28) , std::make_unique<empty_piece>(29) , std::make_unique<empty_piece>(30), std::make_unique<empty_piece>(31) ,
	std::make_unique<empty_piece>(32), std::make_unique<empty_piece>(33), std::make_unique<empty_piece>(34) , std::make_unique<empty_piece>(35) , std::make_unique<empty_piece>(36) , std::make_unique<empty_piece>(37) , std::make_unique<empty_piece>(38), std::make_unique<empty_piece>(39) ,
	std::make_unique<empty_piece>(40), std::make_unique<empty_piece>(41), std::make_unique<empty_piece>(42) , std::make_unique<empty_piece>(43) , std::make_unique<empty_piece>(44) , std::make_unique<empty_piece>(45) , std::make_unique<empty_piece>(46), std::make_unique<empty_piece>(47) ,
	std::make_unique<pawn>(48,colour::White,0), std::make_unique<pawn>(49,colour::White,0), std::make_unique<pawn>(50,colour::White,0), std::make_unique<pawn>(51,colour::White,0), std::make_unique<pawn>(52,colour::White,0), std::make_unique<pawn>(53,colour::White,0), std::make_unique<pawn>(54,colour::White,0) , std::make_unique<pawn>(55,colour::White,0) ,
	std::make_unique<rook>(56,colour::White,false), std::make_unique<knight>(57,colour::White), std::make_unique<bishop>(58,colour::White) , std::make_unique<queen>(59,colour::White) , std::make_unique<king>(60,colour::White,false) , std::make_unique<bishop>(61,colour::White) , std::make_unique<knight>(62,colour::White), std::make_unique<rook>(63,colour::White,false)
}
{
	//board updates when it is initialised to make all pieces have the correct pseudovalid_move arrays
	update();
}

// copy constructor
board::board(const board& b) : board_in_check{ b.board_in_check }, black_king_position{ b.black_king_position }, white_king_position{ b.white_king_position }, checkmate{ b.checkmate }, previous_moves{b.previous_moves}
{
	//fills board array with a clone of every piece in the copied board array
	for (int i{ 0 };i < 64;i++) {
		board_array[i] = b.board_array[i]->clone();
	}
	for (auto it = b.captured_pieces.begin(); it != b.captured_pieces.end();++it) {
		captured_pieces.push_back((*it)->clone());
	}

}


colour board::colour_of_piece_at_coordinate(int i, int j) const
{
	return board_array[8 * i + j]->get_colour();
}

colour board::colour_of_piece_at_index(int pos) const
{
	return board_array[pos]->get_colour();
}

bool board::get_has_moved_at(const int& i, const int& j) const
{
	return board_array[8 * i + j]->get_has_moved();
}

colour board::get_board_in_check() const
{
	return board_in_check;
}

int board::get_turn_number() const
{
	return previous_moves.size();
}

std::pair<int, int> board::get_previous_move() const
{
	std::pair<int, int> previous_move{ std::make_pair(std::get<0>(previous_moves.back()), std::get<1>(previous_moves.back())) };
	return previous_move;
}


//GAMEPLAY

//function to update the board, set king positions, see if any pawns need to be promoted, get all pieces pseudovalid moves, see if the board is in check
void board::update()
{
	//iterate over the board and get the positions of the two kings
	for (auto it = board_array.begin(); it != board_array.end(); ++it) {
		if (typeid(**it) == typeid(king)) {
			if ((*it)->get_colour() == colour::Black) {
				black_king_position = (*it)->get_position();
			}
			else {
				white_king_position = (*it)->get_position();
			}
		}
	}
	//set pawn to promote to -1, as this is the value if there is no pawn to promote
	pawn_to_promote = -1;
	//check for pawns in the 0th file
	for(int i{ 0 };i < 8;i++) {
		if (typeid(*board_array[i]) == typeid(pawn)) {
			pawn_to_promote = i;
		}
	}
	//check for pawns in the 7th file
	for (int i{ 56 };i < 64; i++) {
		if (typeid(*board_array[i]) == typeid(pawn)) {
			pawn_to_promote = i;
		}
	}

	//set board in check colour to None initially.
	board_in_check = colour::None;
	//iterate over all pieces in the board
	for (auto it = board_array.begin(); it != board_array.end(); ++it) {
		//update the pseudovalid move array for the piece.
		(*it)->find_pseudovalid_moves(*this);
		//get the pseudovalid move array
		std::array<move, 64> moves = (*it)->get_pseudovalid_moves();
		//check the position in the array which represents the position of the two kings, if this is !=move::Invalid then a piece can capture the king, so it is in check
		if (moves[black_king_position]!=move::Invalid) { 
			board_in_check = colour::Black; 
		}else if (moves[white_king_position]!=move::Invalid) {
			board_in_check = colour::White;
		}
	}
}


void board::pawn_promotion()
{
	//if pawn_to_promote ==-1 then there is no pawn to promote so this code does not need to run
	if (pawn_to_promote != -1) {
		while (true) {
			print_message("Your pawn has reached the eighth rank. Promote to Queen <Q>, Rook <R>, Bishop <B> or Knight <N>.");
			SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), COORD{ 0,27 });
			//"\33[2K" clears the current line
			std::cout << "\33[2K";
			std::string promotion_choice;
			std::getline(std::cin, promotion_choice);
			if (promotion_choice == "Q") {
				board_array[pawn_to_promote] = std::make_unique<queen>(pawn_to_promote, colour_of_piece_at_index(pawn_to_promote));
				update();
				print_pieces();
				break;
			}
			else if (promotion_choice == "R") {
				board_array[pawn_to_promote] = std::make_unique<rook>(pawn_to_promote, colour_of_piece_at_index(pawn_to_promote),true);
				update();
				print_pieces();
				break;
			}
			else if (promotion_choice == "B") {
				board_array[pawn_to_promote] = std::make_unique<bishop>(pawn_to_promote, colour_of_piece_at_index(pawn_to_promote));
				update();
				print_pieces();
				break;
			}
			else if (promotion_choice == "N") {
				board_array[pawn_to_promote] = std::make_unique<knight>(pawn_to_promote, colour_of_piece_at_index(pawn_to_promote));
				update();
				print_pieces();
				break;
			}
			else {
				continue;
			}
		}
	}
}

//function to move piece at intial_postion to final_postion
void board::move_piece(const int& initial_position, const int& final_position, move move_type)
{
	//the formula for the position calculates whether the captured piece was on the right or the left of the moving piece
	int position_of_captured_pawn{ initial_position + ((final_position % 8) - (initial_position % 8)) };

	//the action depends on the move type
	switch (move_type)
	{
	case move::Passive:
		//the final postion will be empty for passive moves
		//create a clone of the piece at the inital position and store it in the final position of the array
		board_array[final_position] = board_array[initial_position]->clone();
		//update the position of the newly created piece, as it is currently an exact clone of the piece at initial_position,
		board_array[final_position]->update_position(final_position);
		//create a new empty piece and set the inital position to point to this empty_piece
		board_array[initial_position] = std::make_unique<empty_piece>(initial_position);
		//add the move to the previous moves vector
		previous_moves.push_back({ initial_position, final_position,move::Passive });
		break; 
	case move::Capture:
		//capture moves are identical to passive moves, except that the piece at the final position is added to the captured pieces vector
		captured_pieces.push_back(board_array[final_position]->clone());
		board_array[final_position] = board_array[initial_position]->clone();
		board_array[final_position]->update_position(final_position);
		board_array[initial_position] = std::make_unique<empty_piece>(initial_position);
		previous_moves.push_back({ initial_position, final_position, move::Capture });
		break;
	case move::EnPassant:
		//En Passant moves the pawn first, and clears the tile it was on
		board_array[final_position] = board_array[initial_position]->clone();
		board_array[final_position]->update_position(final_position);
		board_array[initial_position] = std::make_unique<empty_piece>(initial_position);
		previous_moves.push_back({ initial_position, final_position, move::EnPassant });
	
		//Add the piece captured by the move to the captured pieces array
		captured_pieces.push_back(board_array[position_of_captured_pawn]->clone());
		//make a new empty_piece unique pointer and make the tile where the captured pawn is point to it
		board_array[position_of_captured_pawn] = std::make_unique<empty_piece>(position_of_captured_pawn);
		break;
	case move::Castling:
		//Castling moves the king first, and adds this to the previous moves vector
		board_array[final_position] = board_array[initial_position]->clone();
		board_array[final_position]->update_position(final_position);
		board_array[initial_position] = std::make_unique<empty_piece>(initial_position);
		previous_moves.push_back({ initial_position, final_position, move::Castling });
		//if the rank of the kings final position ==2 then it is queen side castling and therefore the rook moves from 2 to the left of the king to 1 to the right
		if (final_position % 8 == 2) {
			board_array[final_position+1] = board_array[final_position-2]->clone();
			board_array[final_position+1]->update_position(final_position+1);
			board_array[final_position-2] = std::make_unique<empty_piece>(final_position-2);
		}
		//otherwise the rank will == 6 and so it is king side castling and therefore the rook moves from one to the right to one to the left of the king
		else {
			board_array[final_position - 1] = board_array[final_position + 1]->clone();
			board_array[final_position - 1]->update_position(final_position - 1);
			board_array[final_position + 1] = std::make_unique<empty_piece>(final_position+1);
		}
		break;
	}
}

//function to complete a turn
int board::turn(colour player_colour)
{
	//if the board is not in check, then the game may be in stalemate. 
	if (board_in_check == colour::None) {
		if (is_stalemate(player_colour)) return 2;
	}

	//loop until valid move is entered
	while (true) {
		//create a tuple used to store the output of get_input
		std::tuple<bool, int, int> input_tuple;
		input_tuple = get_input(player_colour);
		//if the first element of the tuple is false, then input was invalid
		while (std::get<0>(input_tuple) == false) {
			print_message("Invalid Input!");
			input_tuple = get_input(player_colour);
		}

		//if the player inputs "exit" then this tuple is returned so return -1, the code for exit
		if (input_tuple == std::make_tuple(true, -1, -1)) {
			return -1;
		}
		//if the player inputs "save" then this tuple is returned so return -2, the code for save
		else if (input_tuple == std::make_tuple(true, -2, -2)) {
			return -2;
		}
		//clear the message line
		print_message("");
		//get the initial and final positions from the tuple
		int init_pos = std::get<1>(input_tuple);
		int fin_pos = std::get<2>(input_tuple);

		//get the possible moves that the pieces can make
		std::array<move, 64> possible_moves = board_array[init_pos]->get_pseudovalid_moves();
		//check if the piece at the inital position entered is the colour of the player who's turn it is, and check if that piece can make the move
		if (board_array[init_pos]->get_colour() == player_colour && possible_moves[fin_pos]!=move::Invalid && is_move_valid(init_pos, fin_pos, player_colour, possible_moves[fin_pos])) {
			//move the piece, possible_moves[fin_pos] gives the move type
			move_piece(init_pos, fin_pos, possible_moves[fin_pos]);
			//update the board following this move
			update();
			//reprint the pieces
			print_pieces();
			//if there are pawns to promote, promote them
			pawn_promotion();
			//check for a checkmate, and return 1 if there is, which tells the main to end the game
			if (is_checkmate()) return 1;
			//clear the message line
			print_message("");
			//returning 0 tells the main that this move ended without the game ending or the player saving or exiting
			return 0;
		}
		else {
			print_message("Invalid Move!");
		}
	}
	
}


bool board::is_move_valid(const int& initial_position, const int& final_position, colour c, move move_type) const
{
	//create copy of board
	board b{ *this };
	//complete the move being tested on the copy board, and update
	b.move_piece(initial_position, final_position, move_type);
	b.update();
	//if the board is now in check, with the colour of the player that made the move, then it is not a valid move (as you cannot put youself into check)
	if (b.board_in_check == c) {
		return false;
	}
	else {
		return true;
	}
}

bool board::is_checkmate()
{
	if (board_in_check != colour::None) {
		bool is_checkmate{ true };
		//iterater over all pieces on the board
		for (auto it = board_array.begin(); it != board_array.end(); ++it) {
			//only checks moves of the colour in check, as only these can get the player out of check
			if ((*it)->get_colour() == board_in_check) {
				auto moves = (*it)->get_pseudovalid_moves();
				//checking all possible moves for the piece
				for (int i{ 0 }; i < 64; i++) {
					if (moves[i] != move::Invalid) {
						//if is_move_valid returns true, then the move has taken the board out of check, and therefore it is not checkmate
						if (is_move_valid((*it)->get_position(), i, board_in_check, moves[i])) {
							is_checkmate = false;
							break; //once a valid move is found, there is no need to check more so the loop breaks.
						}
					}
				}
			}
			if (!is_checkmate) {
				//by returning false here, it stops the iterations over the rest of the pieces once a valid move is found
				return false;
			}
		}

		checkmate = board_in_check;
		return true;
	}
	else {
		return false;
	}
}

bool board::is_stalemate(colour player_colour)
{
	bool is_stalemate{ true };
	//iterater over all pieces on the board
	for (auto it = board_array.begin(); it != board_array.end(); ++it) {
		//only checks moves of non-empty tiles, as only these will have moves
		if ((*it)->get_colour() == player_colour) {
			auto moves = (*it)->get_pseudovalid_moves();
			//checking all possible moves for the piece
			for (int i{ 0 }; i < 64; i++) {
				if (moves[i] != move::Invalid) {
					//if is_move_valid returns true, then the move has taken the board out of check, and therefore it is not stalemate
					if (is_move_valid((*it)->get_position(), i, player_colour, moves[i])) {
						is_stalemate = false;
						break; //once a valid move is found, there is no need to check more so the loop breaks.
					}
				}
			}
		}
		if (!is_stalemate) {
			//by returning false here, it stops the iterations over the rest of the pieces once a valid move is found
			return false;
		}
	}
	return true;
}

//INPUT/OUTPUT

void board::print_board()
{
	//print board starts by clearing the console
	std::system("cls");
	
	//array storing the two colour codes for the two grey shades which the board is colour with
	std::array<int, 2> board_colour{ {128, 112} };
	
	//printing the board

	//print the letters along the top of the board
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
	std::cout << "   a    b    c    d    e    f    g    h   " << std::endl;
	for (int row{ 0 }; row < 8;row++) {
		for (int line{ 0 }; line < 3; line++) {
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
			if (line == 1) {
				//the file is labelled with numbers increasing from bottom to top, from 1 to 8. So the label is 8-row
				std::cout <<8-row;
			}
			else {
				std::cout << " ";
			}
			for (int column{ 0 };column < 8;column++) {
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), board_colour[(row + column) % 2]);
				std::cout << "     ";
			}
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
			if (line == 1) {
				std::cout << 8-row<< std::endl;
			}
			else {
				std::cout << " "<<std::endl;
			}
		}
	}
	std::cout << "   a    b    c    d    e    f    g    h   " << std::endl;

	//printing the captured pieces zone
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), COORD{ 44,1 });
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 143);
	std::cout << "Captured Pieces:                ";
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 112);
	for (SHORT i{ 0 }; i < 6; i++)
	{
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), COORD{ 44,2+i });
		std::cout << "                                ";
	}

	//printing the instructions
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), COORD{ 44, 9 });
	std::cout << "Instructions:";
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), COORD{ 44, 10 });
	std::cout << "Enter coordinate of piece to move";
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), COORD{ 44, 11 });
	std::cout << "Followed by coordinate of destination";
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), COORD{ 44, 12 });
	std::cout << "i.e. \"a2 a4\" or \"g7f6\"";
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), COORD{ 44, 13 });
	std::cout << "For castling, move king and rook will";
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), COORD{ 44, 14 });
	std::cout << "move automatically.";
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), COORD{ 44, 16 });
	std::cout << "To save game enter \"save\".";
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), COORD{ 44, 17 });
	std::cout << "To exit game enter \"exit\".";

}

//function to print the pieces onto the board and into the captured pieces zone
void board::print_pieces()
{
	//using a map to link the colour of the piece to the colour printed on the screen
	std::map<colour, int> colour_codes;
	colour_codes[colour::Black] = 15;
	colour_codes[colour::White] = 240;

	std::array<int, 2> board_colour{ {128,112} };
	
	//printing the pieces onto the board
	for (short i{ 0 }; i < 64; i++) {
		//COORD position calculates the coordinate of the character at the centre of the tile on the board representing the board array position i
		COORD position{ ((i % 8) * 5) + 3,((i / 8) * 3) + 2 };
		//move cursor to this position
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), position);
		if (!tile_is_type<empty_piece>(i)) {
			//set the colour to the appropriate colour for the piece
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), colour_codes[colour_of_piece_at_index(i)]);
		}
		else {
			//empty tiles are coloured based on the colour of the board at that position
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), board_colour[((i/8)+(i%8))%2]);
		}
		//output the character associated with the piece at the position i
		std::cout << board_array[i]->get_character();
	}

	//print the the captured white pieces
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), COORD{ 44,3 });
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 240);
	for (auto it = captured_pieces.begin(); it != captured_pieces.end();++it) {
		if ((*it)->get_colour() == colour::White) {
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 112);
			std::cout << " ";
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 240);
			std::cout << (*it)->get_character();
		}
	}
	//print the captured black pieces
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), COORD{ 44,5 });
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
	for (auto it = captured_pieces.begin(); it != captured_pieces.end();++it) {
		if ((*it)->get_colour() == colour::Black) {
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 112);
			std::cout << " ";
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
			std::cout<< (*it)->get_character();
		}
	}
	
}

//function to print messages to the messages line and which colour is in check
void board::print_message(std::string message)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
	COORD end_position{ 0,26 };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), end_position);
	std::cout << "\33[2K";
	if (board_in_check == colour::Black) {
		std::cout << "Black is in Check!! "<< message << std::endl;
	}
	else if (board_in_check == colour::White) {
		std::cout << "White is in Check!! "<< message << std::endl;
	}
	else {
		std::cout << message << std::endl;
	}
}

//function to get input from the user
std::tuple<bool,int,int> board::get_input(colour player_colour)
{	
	std::tuple<bool, int, int> output{ true,0,0 };
	
	//mapping the inputted character representing a rank to a rank used in the code.
	std::map<char, int> letter_to_rank;
	letter_to_rank['a'] = 0;
	letter_to_rank['b'] = 1;
	letter_to_rank['c'] = 2;
	letter_to_rank['d'] = 3;
	letter_to_rank['e'] = 4;
	letter_to_rank['f'] = 5;
	letter_to_rank['g'] = 6;
	letter_to_rank['h'] = 7;

	//mapping the colour enum to strings representing the colours
	std::map<colour, std::string> colour_strings;
	colour_strings[colour::Black] = "Black > ";
	colour_strings[colour::White] = "White > ";

	std::string raw_input;
	COORD input_position{ 0,27 };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), input_position);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
	std::cout << "\33[2K" << colour_strings[player_colour];
	std::getline(std::cin, raw_input);
	
	//if the user inputs exit or save, return relevent tuples
	if (raw_input == "exit") {
		return { true,-1,-1 };
	}
	else if (raw_input == "save") {
		return { true, -2, -2 };
	}
	
	std::istringstream input_stream;
	input_stream.str(raw_input);
	char first_letter;
	int first_number{ 0 };
	char second_letter;
	int second_number{ 0 };

	int initial_file{ 0 };
	int initial_rank{ 0 };
	int final_file{ 0 };
	int final_rank{ 0 };

	input_stream >> first_letter;
	if (first_letter >= 'a' && first_letter < 'i') {
		//mapping letter to rank;
		initial_rank = letter_to_rank[first_letter];
	}
	else {
		//if any input is incorrect, then the function immediately sets the first element of the tuple to false, and outputs it
		std::get<0>(output) = false;
		return output;
	}
	input_stream >> first_number;
	if (!input_stream.fail() && first_number > 0 && first_number < 9) {
		std::get<0>(output) = true;
		//mapping input number to file and setting initial position
		initial_file = 8 - first_number;
		std::get<1>(output) = 8 * initial_file + initial_rank;
	}
	else {
		std::get<0>(output) = false;
		return output;
	}

	input_stream >> second_letter;
	if (second_letter >= 'a' && second_letter < 'i') {
		std::get<0>(output) = true;
		//mapping letter to rank;
		final_rank = letter_to_rank[second_letter];
	}
	else {
		std::get<0>(output) = false;
		return output;
	}

	input_stream >> second_number;
	if (!input_stream.fail() && second_number > 0 && second_number < 9) {
		//mapping input number to file and setting initial position
		std::get<0>(output) = true;
		final_file = 8 - second_number;
		std::get<2>(output) = 8 * final_file + final_rank;
	}
	else {
		std::get<0>(output) = false;
		return output;
	}

	//if the input is not at the end when this is reached, then they have entered something else after what is a otherwise valid input
	//and therefore the input is invalid
	if (input_stream.eof()) {
		return output;
	}
	else {
		std::get<0>(output) = false;
		return output;
	}

}

//SAVING AND LOADING FUNCTIONS

//output function to output to a file
std::ostream& operator<<(std::ostream& os, const board& b)
{
	//outputting the board_array, one piece per line
	//outputs the characters for the pieces, and their colour as the letters B or W
	for (int i{ 0 };i < 64;i++) {

		switch (b.board_array[i]->get_colour()) {
		case(colour::Black):
			os <<b.board_array[i]->get_character()<< " B";
			break;

		case(colour::White):
			os << b.board_array[i]->get_character() << " W";
			break;
		case(colour::None):
			os << "E";
			break;
		}
		//whether or not the these pieces have moved matters for the game, so this information is outputted
		if (b.tile_is_type<pawn>(i)){
			os << " " << b.board_array[i]->get_has_moved();
		}
		else if (b.tile_is_type<king>(i)) {
			os << " " << b.board_array[i]->get_has_moved();
		}
		else if (b.tile_is_type<rook>(i)) {
			os << " " << b.board_array[i]->get_has_moved();
		}

		os << "\n";
	}

	//outputting the captured piece vector
	//first output the size of the vector, for when this save is loaded
	os << b.captured_pieces.size() << "\n";
	//output the data in a similar way to for the board_array, although has_moved is unnecessary
	for (auto it = b.captured_pieces.begin(); it != b.captured_pieces.end();++it) {
		switch ((*it)->get_colour()) {
		case(colour::Black):
			os << (*it)->get_character() << " B";
			break;

		case(colour::White):
			os << (*it)->get_character() << " W";
			break;
		}

		os << "\n";
	}

	//outputting the previous moves vector
	//first output the size of the vector, for when the save is loaded
	os << b.previous_moves.size() << "\n";
	for (auto it = b.previous_moves.begin(); it != b.previous_moves.end(); ++it) {
		//ouput the initial and final positions
		os << std::get<0>(*it) << " " << std::get<1>(*it) << " ";
		//output a letter representing the type of move
		switch (std::get<2>(*it))
		{
		case(move::Invalid):
			os << "I";
			break;
		case(move::Passive):
			os << "P";
			break;
		case(move::Capture):
			os << "C";
			break;
		case(move::Castling):
			os << "K";
			break;
		case(move::EnPassant):
			os << "E";
			break;
		}
		os << "\n";
	}

	return os;
}

//operator for loading board from file
std::istream & operator>>(std::istream& is, board & b)
{
	//try catch stops any corrupted or empty files from being loaded and aborting the program
	try {
		std::string line;
		colour loaded_piece_colour;
		bool has_moved;
		//inputting the board array, which is always the first 64 lines of the file
		for (int i{ 0 }; i < 64;i++) {
			std::getline(is, line);
			switch (line[0]) {
			case('E'):
				b.board_array[i] = std::make_unique<empty_piece>(i);
				break;
			case('P'):
				if (line.size() < 5) throw("Invalid Save");
				if (line[2] == 'B') {
					loaded_piece_colour = colour::Black;
				}
				else {
					loaded_piece_colour = colour::White;
				}
				if (line[4] == '0') {
					has_moved = false;
				}
				else {
					has_moved = true;
				}
				b.board_array[i] = std::make_unique<pawn>(i, loaded_piece_colour, has_moved);
				break;
			case('R'):
				if (line.size() < 3) throw("Invalid Save");
				if (line[2] == 'B') {
					loaded_piece_colour = colour::Black;
				}
				else {
					loaded_piece_colour = colour::White;
				}
				if (line[4] == '0') {
					has_moved = false;
				}
				else {
					has_moved = true;
				}
				b.board_array[i] = std::make_unique<rook>(i, loaded_piece_colour, has_moved);
				break;
			case('B'):
				if (line.size() < 3) throw("Invalid Save");
				if (line[2] == 'B') {
					loaded_piece_colour = colour::Black;
				}
				else {
					loaded_piece_colour = colour::White;
				}
				b.board_array[i] = std::make_unique<bishop>(i, loaded_piece_colour);
				break;
			case('N'):
				if (line.size() < 3) throw("Invalid Save");
				if (line[2] == 'B') {
					loaded_piece_colour = colour::Black;
				}
				else {
					loaded_piece_colour = colour::White;
				}
				b.board_array[i] = std::make_unique<knight>(i, loaded_piece_colour);
				break;
			case('Q'):
				if (line.size() < 2) throw("Invalid Save");
				if (line[2] == 'B') {
					loaded_piece_colour = colour::Black;
				}
				else {
					loaded_piece_colour = colour::White;
				}
				b.board_array[i] = std::make_unique<queen>(i, loaded_piece_colour);
				break;
			case('K'):
				if (line.size() < 5) throw("Invalid Save");
				if (line[2] == 'B') {
					loaded_piece_colour = colour::Black;
				}
				else {
					loaded_piece_colour = colour::White;
				}
				if (line[4] == '0') {
					has_moved = false;
				}
				else {
					has_moved = true;
				}
				b.board_array[i] = std::make_unique<king>(i, loaded_piece_colour, has_moved);
				break;
			default:
				//if the first character of these lines is anything other than those above, then it must be an invalid save
				throw("Invalid save");
				break;
			}
		}
		
		//loading captured pieces

		//first get the number of captured peices from the next line
		std::string number_of_captured_pieces_string;
		std::istringstream number_of_captured_pieces_ss;
		int number_of_captured_pieces{ 0 };
		std::getline(is, number_of_captured_pieces_string);
		number_of_captured_pieces_ss.str(number_of_captured_pieces_string);
		number_of_captured_pieces_ss >> number_of_captured_pieces;
		if (number_of_captured_pieces_ss.fail()) {
			throw("Invalid Save");
		}
		//loops over as many lines as there are captured pieces
		for (int i{ 0 };i < number_of_captured_pieces;i++) {
			std::getline(is, line);
			switch (line[0]) {
			case('P'):
				if (line.size() < 3) throw("Invalid Save");
				if (line[2] == 'B') {
					loaded_piece_colour = colour::Black;
				}
				else {
					loaded_piece_colour = colour::White;
				}
				b.captured_pieces.push_back(std::make_unique<pawn>(i, loaded_piece_colour, true));
				break;
			case('R'):
				if (line.size() < 3) throw("Invalid Save");
				if (line[2] == 'B') {
					loaded_piece_colour = colour::Black;
				}
				else {
					loaded_piece_colour = colour::White;
				}
				b.captured_pieces.push_back(std::make_unique<rook>(i, loaded_piece_colour,true));
				break;
			case('B'):
				if (line.size() < 3) throw("Invalid Save");
				if (line[2] == 'B') {
					loaded_piece_colour = colour::Black;
				}
				else {
					loaded_piece_colour = colour::White;
				}
				b.captured_pieces.push_back(std::make_unique<bishop>(i, loaded_piece_colour));
				break;
			case('N'):
				if (line.size() < 3) throw("Invalid Save");
				if (line[2] == 'B') {
					loaded_piece_colour = colour::Black;
				}
				else {
					loaded_piece_colour = colour::White;
				}
				b.captured_pieces.push_back(std::make_unique<knight>(i, loaded_piece_colour));
				break;
			case('Q'):
				if (line.size() < 3) throw("Invalid Save");

				if (line[2] == 'B') {
					loaded_piece_colour = colour::Black;
				}
				else {
					loaded_piece_colour = colour::White;
				}
				b.captured_pieces.push_back(std::make_unique<queen>(i, loaded_piece_colour));
				break;
			default:
				throw("Invalid save");
				break;
			}
		}

		//loading the previous moves
		//get the number of previous moves
		std::string number_of_previous_moves_string;
		std::istringstream number_of_previous_moves_ss;
		int number_of_previous_moves{ 0 };
		std::getline(is, number_of_previous_moves_string);
		number_of_previous_moves_ss.str(number_of_previous_moves_string);
		number_of_previous_moves_ss >> number_of_previous_moves;
		if (number_of_previous_moves_ss.fail()) {
			throw("Invalid Save");
		}

		int init_pos{ 0 };
		int fin_pos{ 0 };
		std::istringstream line_ss;
		char move_char;
		move move_type;
		//loop over the number of previous moves
		for (int i{ 0 }; i < number_of_previous_moves; i++) {
			std::getline(is, line);
			line_ss.str(line);
			line_ss >> init_pos;
			//if these inputs fail, then there is something wrong with the integer so the save file is invalid
			if (line_ss.fail()) {
				throw("Invalid Save");
			}
			line_ss >> fin_pos;
			if (line_ss.fail()) {
				throw("Invalid Save");
			}
			line_ss >> move_char;
			switch (move_char)
			{
			case('I'):
				move_type = move::Invalid;
				break;
			case('P'):
				move_type = move::Passive;
				break;
			case('C'):
				move_type = move::Capture;
				break;
			case('K'):
				move_type = move::Castling;
				break;
			case('E'):
				move_type = move::EnPassant;
				break;
			default:
				throw("Invalid save");
				break;
			}

			b.previous_moves.push_back(std::make_tuple(init_pos, fin_pos, move_type));
		}
		//now that the number board_array, captured_pieces and previous_moves has been inputted into the board it must be updated
		//to get the pseudovalid moves for each piece
		b.update();
		return is;
	}
	catch (...) {
		//if any operation fails, then the failbit is set
		is.setstate(std::ios::failbit);
		return is;
	}
}