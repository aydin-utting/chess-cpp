#ifndef PAWN_H
#define PAWN_H
#include <vector>
#include <array>
#include <string>
#include "piece.h"

class pawn : public piece
{
//pawn.h
//derived piece class representing the pawn piece
private:
	bool has_moved{ false };
	std::array<move, 64> pseudovalid_moves{ {move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid } };
public:
	pawn() : piece(0, colour::Black), has_moved{ false }, pseudovalid_moves{ {move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid } } {}
	pawn(int set_position, colour set_colour, bool set_has_moved) : piece(set_position, set_colour), has_moved{ set_has_moved }, pseudovalid_moves{ {move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid } } {}
	pawn(const pawn& p);
	~pawn() {}
	void find_pseudovalid_moves(const board& b);
	std::array<move, 64> get_pseudovalid_moves() const;
	void update_position(int new_position);
	std::unique_ptr<piece> clone();
	std::string get_character();
	bool get_has_moved() const;
};

#endif // !PAWN_H
