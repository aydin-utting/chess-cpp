#include <iostream>
#include <array>
#include <string>
#include <memory>
#include "piece.h"
#include "empty_piece.h"

//copy constructor
empty_piece::empty_piece(const empty_piece& ep) : piece(ep.position, colour::None) {}

//find pseudovalid moves does nothing, as the empty_piece cannot move
void empty_piece::find_pseudovalid_moves(const board& b) {}


void empty_piece::update_position(int new_position)
{
	position = new_position;
	file = position / 8;
	rank = position % 8;

}

//as the empty_piece can never have moves, it creates a new array will all invalid moves and returns it
std::array<move, 64> empty_piece::get_pseudovalid_moves() const
{
	std::array<move, 64> empty;
	empty.fill(move::Invalid);
	return empty;
}

//clone creates a unique pointer to an exact copy of the empty_piece 
std::unique_ptr<piece> empty_piece::clone()
{
	std::unique_ptr<piece> clone = std::make_unique<empty_piece>(*this);
	return clone;
}

//the empty_piece's character is a blank space.
std::string empty_piece::get_character()
{
	return " ";
}

//the empty piece can never move, so has moved is always false
bool empty_piece::get_has_moved() const
{
	return false;
}