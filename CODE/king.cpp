//king.cpp
#include <iostream>
#include <array>
#include <string>
#include <memory>
#include "piece.h"
#include "king.h"
#include "rook.h"
#include "empty_piece.h"
#include "board.h"

//copy constructor
king::king(const king& k) : piece(k.position, k.piece_colour), pseudovalid_moves{ k.pseudovalid_moves }, has_moved{ k.has_moved } {}

//update position also sets has_moved to true
void king::update_position(int new_position)
{
	position = new_position;
	file = position / 8;
	rank = position % 8;
	has_moved = true;
}

//function to find pseudovalid moves for the king, including those that may put it into check
void king::find_pseudovalid_moves(const board& b)
{
	
	std::array<move, 64> new_pseudovalid_moves;
	new_pseudovalid_moves.fill(move::Invalid);
	
	//standard king moves
	//loops over x and y, with x and y going from -1 to 1 effectively checking all positions in the 3x3 grid with the king at the centre (unless some this grid goes over the end of the board)
	for (int y{ -1 }; y < 2; y++) {
		for (int x{ -1 }; x < 2; x++) {
			//skip the iteration where x == 0 and y == 0, as this is where the king itself is
			if (x == 0 && y == 0) continue;
			if (file + y > -1 && file + y <8 && rank + x>-1 && rank + x < 8) {
				if (b.tile_is_type<empty_piece>(file + y, rank + x)) {
					new_pseudovalid_moves[8 * (file + y) + (rank + x)] = move::Passive;
				}
				else if (b.colour_of_piece_at_coordinate(file + y, rank + x) != piece_colour) {
					new_pseudovalid_moves[8 * (file + y) + (rank + x)] = move::Capture;
				}
			}
		}
	}

	//castling moves
	//these can only occur if the king and rook have not moved, if there are no pieces between the king and rook, and if the king is not in check and does not move through a tile that puts it into check.
	if (!has_moved && b.get_board_in_check() != piece_colour) {
		//king side castling
		if (b.tile_is_type<empty_piece>(file, rank + 1) && b.tile_is_type<empty_piece>(file, rank + 2) && b.tile_is_type<rook>(file, rank + 3) && b.colour_of_piece_at_coordinate(file, rank + 3) == piece_colour && !b.get_has_moved_at(file, rank + 3)) {
			//b.is_move_valid check if moving to the right would put the king into check, returning true if it does not.
			if (b.is_move_valid(position, 8 * file + rank + 1, piece_colour, move::Passive)) {
				
				new_pseudovalid_moves[8 * file + rank + 2] = move::Castling;
			}
		}
		//queen side castling
		if (b.tile_is_type<empty_piece>(file, rank - 1) && b.tile_is_type<empty_piece>(file, rank - 2) && b.tile_is_type<empty_piece>(file, rank - 3) && b.tile_is_type<rook>(file, rank - 4) && b.colour_of_piece_at_coordinate(file, rank - 4) == piece_colour && !b.get_has_moved_at(file, rank-4)) {
			//b.is_move_valid check if moving to the left would put the king into check, returning true if it does not.
			if (b.is_move_valid(position, 8 * file + rank - 1, piece_colour, move::Passive)) {
				new_pseudovalid_moves[8 * file + rank - 2] = move::Castling;
			}
		}
	}

	pseudovalid_moves = new_pseudovalid_moves;
}

std::array<move, 64> king::get_pseudovalid_moves() const
{
	return pseudovalid_moves;
}

std::unique_ptr<piece> king::clone()
{
	std::unique_ptr<piece> clone = std::make_unique<king>(*this);
	return clone;
}

std::string king::get_character()
{
	return "K";
}

bool king::get_has_moved() const
{
	return has_moved;
}