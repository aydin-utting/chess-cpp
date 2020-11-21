#include <iostream>
#include <array>
#include <string>
#include <memory>
#include "piece.h"
#include "knight.h"
#include "empty_piece.h"
#include "board.h"


knight::knight(const knight& kn) : piece(kn.position, kn.piece_colour), pseudovalid_moves{ kn.pseudovalid_moves } {}

void knight::update_position(int new_position)
{
	position = new_position;
	file = position / 8;
	rank = position % 8;
}


void knight::find_pseudovalid_moves(const board& b)
{

	std::array<move, 64> new_pseudovalid_moves;
	new_pseudovalid_moves.fill(move::Invalid);
	//the knight can move to 8 positions all of which lie on the perimeter of a 5x5 grid on the board. As it can jump over pieces, it is only necessary to check if these 8 are empty or contain an opponents piece
	for (int i{ -2 }; i < 3; i++) {
		//skip iterations which check above or below the board, or on the same file as the knight itself
		if (file + i < 0 || file + i>7 || i == 0) continue;
		for (int j{ -2 }; j < 3; j++) {
			//skip iterations which check outside the board to the left or right, on the same file as the knight itself, or where i and j have the same absolute value (the corners of the 5x5 grid)
			if (j == 0 || j + rank < 0 || j + rank>7 || abs(i) == abs(j)) continue;
			if (b.tile_is_type<empty_piece>(file + i, rank + j)) {
				new_pseudovalid_moves[8 * (file + i) + rank + j] = move::Passive;
			}
			else if (b.colour_of_piece_at_coordinate(file + i, rank + j) != piece_colour) {
				new_pseudovalid_moves[8 * (file + i) + rank + j] = move::Capture;
			}
		}
	}
	pseudovalid_moves = new_pseudovalid_moves;
}


std::array<move, 64> knight::get_pseudovalid_moves() const
{
	return pseudovalid_moves;
}

std::unique_ptr<piece> knight::clone()
{
	std::unique_ptr<piece> clone = std::make_unique<knight>(*this);
	return clone;
}

//knights character is N, as in standard algebraic notation for chess
std::string knight::get_character()
{
	return "N";
}

//no rules depend on whether or not the knight has moved, so this function is never called and will always return false.
bool knight::get_has_moved() const
{
	return false;
}
