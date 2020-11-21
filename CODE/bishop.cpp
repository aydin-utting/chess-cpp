#include <iostream>
#include <array>
#include <string>
#include <memory>
#include "piece.h"
#include "bishop.h"
#include "empty_piece.h"
#include "board.h"

//copy constructor
bishop::bishop(const bishop& b) : piece(b.position, b.piece_colour), pseudovalid_moves{ b.pseudovalid_moves } {}

void bishop::update_position(int new_position)
{
	position = new_position;
	file = position / 8;
	rank = position % 8;

}


void bishop::find_pseudovalid_moves(const board& b)
{
	//create new array of moves and set them all to Invalid
	std::array<move, 64> new_pseudovalid_moves;
	new_pseudovalid_moves.fill(move::Invalid);

	//x and y are multipliers which set the "direction" in which the loop moves along the board.
	for (int y{ -1 }; y < 2; y = y + 2) {
		for (int x{ -1 }; x < 2; x = x + 2) {
			//i and j start one below the current file and rank respectively, and on each iteration they move further below the file and rank until the loop breaks, and then the direction changes
			for (int i = file + y, j = rank + x; i > -1 && i < 8 && j > -1 && j < 8; i += y, j += x) {
				if (b.tile_is_type<empty_piece>(i, j)) {
					//if the piece is empty it sets the relavent position in the pseudovalid moves array to a Passive move
					new_pseudovalid_moves[8 * i + j] = move::Passive;
				}
				else if (b.colour_of_piece_at_coordinate(i, j) != piece_colour) {
					//if the piece is not empty, but it is the opposite colour, then the bishop can move their to capture it
					new_pseudovalid_moves[8 * i + j] = move::Capture;
					//the loop must break as the bishop cannot travel past the piece
					break;
				}
				else {
					//if the bishop hits a piece of its own colour, it cannot move there and cannot travel further in this direction
					break;
				}
			}
		}
	}
	pseudovalid_moves = new_pseudovalid_moves;
}

std::array<move, 64> bishop::get_pseudovalid_moves() const
{
	return pseudovalid_moves;
}

//function to create unique pointer of exact copy of the bishop
std::unique_ptr<piece> bishop::clone()
{
	std::unique_ptr<piece> clone = std::make_unique<bishop>(*this);
	return clone;
}

//bishop has the character B
std::string bishop::get_character()
{
	return "B";
}

//no rules in chess depend on the bishop having moved, so this is never called and will return false.
bool bishop::get_has_moved() const
{
	return false;
}