//bishop.h
//derived class for bishop piece
#ifndef BISHOP_H
#define BISHOP_H
#include <vector>
#include <array>
#include <string>
#include "piece.h"



class bishop : public piece
{
private:
	//initialise the pseudovalid_moves array with all tiles being Invalid moves.
	std::array<move, 64> pseudovalid_moves{ {move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid} };
public:
	//constructors and destructors
	bishop(int set_position, colour set_colour) : piece(set_position, set_colour), pseudovalid_moves{ {move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid,move::Invalid } } {}
	~bishop() {}
	bishop(const bishop& b);
	
	//function to calculate the possible moves the bishop can make, including those that may put the player's own king into check (hence pseudovalid)
	void find_pseudovalid_moves(const board& b);
	
	//function to update the position of the piece when it is moved
	void update_position(int new_position);
	
	//clone() creates a unique_pointer to an exact copy of the bishop
	std::unique_ptr<piece> clone();
	
	//get functions
	std::array<move, 64> get_pseudovalid_moves() const;
	std::string get_character();
	bool get_has_moved() const;

};

#endif // !BISHOP_H


