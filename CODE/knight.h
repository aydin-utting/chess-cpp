//knight.h
//derived piece class representing a knight piece
#ifndef KNIGHT_H
#define KNIGHT_H
#include <vector>
#include <array>
#include <string>
#include "piece.h"

class knight : public piece
{
private:
	std::array<move, 64> pseudovalid_moves{ {move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid } };
public:
	knight(int set_position, colour set_colour) : piece(set_position, set_colour), pseudovalid_moves{ {move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid } } {}
	~knight() {}
	knight(const knight& kn);
	void find_pseudovalid_moves(const board& b);
	std::array<move, 64> get_pseudovalid_moves() const;
	void update_position(int new_position);
	std::unique_ptr<piece> clone();
	std::string get_character();
	bool get_has_moved() const;
};

#endif // !KNIGHT_H
