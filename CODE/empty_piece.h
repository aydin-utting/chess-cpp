//empty_piece.h
//derived piece class representing an empty tile
#ifndef EMPTY_PIECE_H
#define EMPTY_PIECE_H
#include <vector>
#include <array>
#include <string>
#include "piece.h"

class empty_piece : public piece
{
public:
	//empty_piece is the only piece which has no colour
	empty_piece(int set_position) : piece{ set_position , colour::None } {}
	~empty_piece() {}
	empty_piece(const empty_piece& ep);

	void find_pseudovalid_moves(const board& b);

	std::array<move, 64> get_pseudovalid_moves() const;

	std::unique_ptr<piece> clone();

	std::string get_character();

	bool get_has_moved() const;

	void update_position(int new_position);
};


#endif // !EMPTY_PIECE_H
