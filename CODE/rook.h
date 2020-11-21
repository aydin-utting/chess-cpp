//rook.h
//derived piece class representing a rook piece
#ifndef ROOK_H
#define ROOK_H
#include <vector>
#include <array>
#include <string>
#include "piece.h"

class rook : public piece
{
private:
	std::array<move, 64> pseudovalid_moves{ {move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid } };
	bool has_moved{ false };
public:
	rook(int set_position, colour set_colour, bool set_has_moved) : piece(set_position, set_colour), has_moved{ set_has_moved }, pseudovalid_moves{ {move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid } } {}
	rook(const rook& r);
	~rook() {}
	void find_pseudovalid_moves(const board& b);
	std::array<move, 64> get_pseudovalid_moves() const;
	void update_position(int new_position);
	std::unique_ptr<piece> clone();
	std::string get_character();
	bool get_has_moved() const;
};

#endif // !ROOK_H

