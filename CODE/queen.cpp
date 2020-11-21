//queen.cpp

#include <iostream>
#include <array>
#include <string>
#include <memory>
#include "piece.h"
#include "queen.h"
#include "empty_piece.h"
#include "board.h"


queen::queen(const queen& q) : piece(q.position, q.piece_colour), pseudovalid_moves{ q.pseudovalid_moves } {}


void queen::update_position(int new_position)
{
	position = new_position;
	file = position / 8;
	rank = position % 8;
}


void queen::find_pseudovalid_moves(const board& b)
{
	std::array<move, 64> new_pseudovalid_moves;
	new_pseudovalid_moves.fill(move::Invalid);
	//the queen moves as a combination of a rook and bishop.
	//multipliers y and x modify the the file and rank respectively, setting whether they decrease, increase or remain constant during iterations of the inner for loop
	for (int y{ -1 }; y < 2; y++) {
		for (int x{ -1 }; x < 2; x++) {
			//the formula for i and j ensure that the search never starts on the same square as the queen
			//i and j must be >-1 and <8 such that the queen can only move inside the board
			//i and j increase by y and x respectively, so that the search continues in the same direction until a piece or the end of the board is reached.
			for (int i = file + y, j = rank + x; i > -1 && i < 8 && j > -1 && j < 8; i += y, j += x) {
				//empty tiles can be moved into passively
				if (b.tile_is_type<empty_piece>(i, j)) {
					new_pseudovalid_moves[8 * i + j] = move::Passive;
				}
				//a tile with an opponents piece can be moved into, capturing the piece
				else if (b.colour_of_piece_at_coordinate(i, j) != piece_colour) {
					new_pseudovalid_moves[8 * i + j] = move::Capture;
					//once an opponents piece is reached, the search must stop in this direction
					break;
				}
				else {
					//if a piece of the same colour is reached, then the search is this direction is stopped
					break;
				}
			}
		}
	}
	pseudovalid_moves = new_pseudovalid_moves;
}


std::array<move, 64> queen::get_pseudovalid_moves() const
{
	return pseudovalid_moves;
}

std::unique_ptr<piece> queen::clone()
{
	std::unique_ptr<piece> clone = std::make_unique<queen>(*this);
	return clone;
}


std::string queen::get_character()
{
	return "Q";
}

bool queen::get_has_moved() const
{
	return false;
}