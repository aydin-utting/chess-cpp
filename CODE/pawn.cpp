#include <iostream>
#include <array>
#include <string>
#include <memory>
#include "piece.h"
#include "pawn.h"
#include "empty_piece.h"
#include "board.h"

pawn::pawn(const pawn& p) : piece(p.position, p.piece_colour), has_moved{ p.has_moved }, pseudovalid_moves{ p.pseudovalid_moves } {}

//function to update position of the pawn, setting has_moved to true
void pawn::update_position(int new_position)
{
	position = new_position;
	file = position / 8;
	rank = position % 8;
	has_moved = true;
}

void pawn::find_pseudovalid_moves(const board& b)
{
	std::array<move, 64> new_pseudovalid_moves;
	new_pseudovalid_moves.fill(move::Invalid);

	//pawns are the only piece for which their movement depends on their colour. (White moves up the board, and black moves down. Although in the code, white pawns start on file == 6 and move to file == 0, and black start on file == 1 and move to file == 7)
	if (piece_colour == colour::White) {
		if (file > 0) {
			//movement in straight line
			if (b.tile_is_type<empty_piece>(file - 1, rank)) {
				new_pseudovalid_moves[(file - 1) * 8 + rank] = move::Passive;
				//for a pawns first move it can move two spaces
				if (file > 1 && !has_moved) {
					if (b.tile_is_type<empty_piece>(file - 2, rank)) {
						new_pseudovalid_moves[(file - 2) * 8 + rank] = move::Passive;
					}
				}
			}
			//capturing moves, diagonally
			//decreasing rank
			if (rank > 0) {
				//normal capture, if the piece diagonally is Black then it can capture it
				if (!b.tile_is_type<empty_piece>(file - 1, rank - 1) && b.colour_of_piece_at_coordinate(file - 1, rank - 1) == colour::Black) {
					new_pseudovalid_moves[(file - 1) * 8 + (rank - 1)] = move::Capture;
				}
				//En Passant move, if the piece directly next to the pawn itself is a Black pawn, and the previous move was moving that piece two spaces then it can be captured by En Passant
				else if (b.tile_is_type<pawn>(8 * file + rank - 1) && b.colour_of_piece_at_coordinate(file, rank - 1) == colour::Black) {
					if (b.get_previous_move() == std::make_pair(8 * (file - 2) + (rank - 1), 8 * file + rank - 1)) {
						new_pseudovalid_moves[(file - 1) * 8 + (rank - 1)] = move::EnPassant;
					}
				}
			}
			//increasing rank
			if (rank < 7) {
				//normal capture
				if (!b.tile_is_type<empty_piece>(file - 1, rank + 1) && b.colour_of_piece_at_coordinate(file - 1, rank + 1) == colour::Black) {
					new_pseudovalid_moves[(file - 1) * 8 + (rank + 1)] = move::Capture;
				}
				//En passant
				else if (b.tile_is_type<pawn>(8 * file + rank + 1) && b.colour_of_piece_at_coordinate(file, rank + 1) == colour::Black) {
					if (b.get_previous_move() == std::make_pair(8 * (file - 2) + (rank + 1), 8 * file + rank + 1)) {
						new_pseudovalid_moves[(file - 1) * 8 + (rank + 1)] = move::EnPassant;
					}
				}
			}

		}
	}
	else { //for Black pawns
		if (file < 7) {
			//movement in a straight line
			if (b.tile_is_type<empty_piece>(file + 1, rank)) {
				new_pseudovalid_moves[(file + 1) * 8 + rank] = move::Passive;
				if (file < 6 && !has_moved) {
					if (b.tile_is_type<empty_piece>(file + 2, rank)) {
						new_pseudovalid_moves[(file + 2) * 8 + rank] = move::Passive;
					}
				}
			}
			//capturing moves, diagonally
			//decreasing rank
			if (rank > 0) {
				//normal capture
				if (!b.tile_is_type<empty_piece>(file + 1, rank - 1) && b.colour_of_piece_at_coordinate(file + 1, rank - 1) == colour::White) {
					new_pseudovalid_moves[(file + 1) * 8 + (rank - 1)] = move::Capture;
				}
				//En Passant
				else if (b.tile_is_type<pawn>(8 * file + rank - 1) && b.colour_of_piece_at_coordinate(file, rank - 1) == colour::White) {
					if (b.get_previous_move() == std::make_pair(8 * (file + 2) + (rank - 1), 8 * file + rank - 1)) {
						new_pseudovalid_moves[(file + 1) * 8 + (rank - 1)] = move::EnPassant;
					}
				}
			}
			//increasing rank
			if (rank < 7) {
				//normal capture
				if (!b.tile_is_type<empty_piece>(file + 1, rank + 1) && b.colour_of_piece_at_coordinate(file + 1, rank + 1) == colour::White) {
					new_pseudovalid_moves[(file + 1) * 8 + (rank + 1)] = move::Capture;
				}
				//En passant
				else if (b.tile_is_type<pawn>(file, rank + 1) && b.colour_of_piece_at_coordinate(file, rank + 1) == colour::White) {
					if (b.get_previous_move() == std::make_pair(8 * (file + 2) + (rank + 1), 8 * file + rank + 1)) {
						new_pseudovalid_moves[(file + 1) * 8 + (rank + 1)] = move::EnPassant;
					}
				}
			}
		}
	}

	pseudovalid_moves = new_pseudovalid_moves;
}

std::array<move, 64> pawn::get_pseudovalid_moves() const
{
	return pseudovalid_moves;
}


std::unique_ptr<piece> pawn::clone()
{
	std::unique_ptr<piece> clone = std::make_unique<pawn>(*this);
	return clone;
}

std::string pawn::get_character()
{
	return "P";
}

bool pawn::get_has_moved() const
{
	return has_moved;
}