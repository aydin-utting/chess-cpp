//board.h
//Class representing the chess board, contains array of base class pointer to pieces representing the board and store other information including previous moves and captured pieces
#ifndef BOARD_H
#define BOARD_H

#include <iostream>
#include <array>
#include <typeinfo>
#include <memory>
#include <vector>
#include <utility>
#include "piece.h"

class board
{
private:
	std::array<std::unique_ptr<piece>,64> board_array;
	colour board_in_check{};
	int black_king_position{ 0 };
	int white_king_position{ 0 };
	colour checkmate{};
	int pawn_to_promote{ -1 };
	std::vector<std::tuple<int, int, move>> previous_moves;
	std::vector<std::unique_ptr<piece>> captured_pieces;
public:

	//Constructors

	board();

	board(const board& b);


	//Get functions

	template <class T> bool tile_is_type(int pos) const
	{
		return typeid(*board_array[pos]) == typeid(T);
	}

	template <class T> bool tile_is_type(int i, int j) const
	{
		return typeid(*board_array[8 * i + j]) == typeid(T);
	}

	bool get_has_moved_at(const int & i,const int & j) const;

	colour colour_of_piece_at_coordinate(int i, int j) const;

	colour colour_of_piece_at_index(int pos) const;

	colour get_board_in_check() const;

	int get_turn_number() const;

	std::pair<int, int> get_previous_move() const;


	//Gameplay functions

	void update();

	void pawn_promotion();

	void move_piece(const int& initial_position, const int& final_position, move move_type);

	int turn(colour player_colour);

	bool is_move_valid(const int& initial_position, const int& final_position, colour c, move move_type) const;

	bool is_checkmate();

	bool is_stalemate(colour player_colour);


	//Input/Output functions

	void print_board();

	void print_pieces();

	void print_message(std::string message);

	std::tuple<bool, int, int> get_input(colour player_colour);


	//Friend functions for save and load

	friend std::ostream& operator<<(std::ostream& os, const board& b);

	friend std::istream& operator>>(std::istream& is, board& b);


};

#endif // !BOARD_H
