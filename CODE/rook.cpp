#include <iostream>
#include <array>
#include <string>
#include <memory>
#include "piece.h"
#include "rook.h"
#include "empty_piece.h"
#include "board.h"

rook::rook(const rook& r) : piece(r.position, r.piece_colour), has_moved{ r.has_moved }, pseudovalid_moves{ r.pseudovalid_moves } {}

//update position for the rook, must change has_moved to true for castling
void rook::update_position(int new_position)
{
	position = new_position;
	file = position / 8;
	rank = position % 8;
	has_moved = true;
}


void rook::find_pseudovalid_moves(const board& b)
{
	std::array<move, 64> new_pseudovalid_moves;
	new_pseudovalid_moves.fill(move::Invalid);
	//searches for pseudovalid moves in the 4 directions which the rook can travel
	
	//increasing file
	for (int i = file + 1; i < 8; i++) {
		if (b.tile_is_type<empty_piece>(i, rank)) {
			new_pseudovalid_moves[8 * i + rank] = move::Passive;
		}
		else if (b.colour_of_piece_at_coordinate(i, rank) != piece_colour) {
			new_pseudovalid_moves[8 * i + rank] = move::Capture;
			break;
		}
		else {
			break;
		}
	}
	//decreasing file
	for (int i = file - 1; i > -1; i--) {
		if (b.tile_is_type<empty_piece>(i, rank)) {
			new_pseudovalid_moves[8 * i + rank] = move::Passive;
		}
		else if (b.colour_of_piece_at_coordinate(i, rank) != piece_colour) {
			new_pseudovalid_moves[8 * i + rank] = move::Capture;
			break;
		}
		else {
			break;
		}
	}
	//increasing rank
	for (int j = rank + 1; j < 8; j++) {
		if (b.tile_is_type<empty_piece>(file, j)) {
			new_pseudovalid_moves[8 * file + j] = move::Passive;
		}
		else if (b.colour_of_piece_at_coordinate(file, j) != piece_colour) {
			new_pseudovalid_moves[8 * file + j] = move::Capture;
			break;
		}
		else {
			break;
		}
	}
	//decreasing rank
	for (int j = rank - 1; j > -1; j--) {
		if (b.tile_is_type<empty_piece>(file, j)) {
			new_pseudovalid_moves[8 * file + j] = move::Passive;
		}
		else if (b.colour_of_piece_at_coordinate(file, j) != piece_colour) {
			new_pseudovalid_moves[8 * file + j] = move::Capture;
			break;
		}
		else {
			break;
		}
	}

	pseudovalid_moves = new_pseudovalid_moves;

}

std::array<move, 64> rook::get_pseudovalid_moves() const
{
	return pseudovalid_moves;
}

std::unique_ptr<piece> rook::clone()
{
	std::unique_ptr<piece> clone = std::make_unique<rook>(*this);
	return clone;
}

std::string rook::get_character()
{
	return "R";
}

bool rook::get_has_moved() const
{
	return has_moved;
}
