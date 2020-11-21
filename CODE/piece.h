//piece.h
//Abstract base class for the pieces.
#ifndef PIECE_H
#define PIECE_H

#include <vector>
#include <array>
#include <string>

//initialised the board class in this file, so that board can be used in the functions for this class
class board;

//enum class for the colour of pieces, also used to determine the colour in check
enum class colour{Black, White, None};

//enum class for move type, stored by pieces and used by board to determine how to execute the move
enum class move{Invalid, Passive, Capture, EnPassant, Castling};

class piece
{
protected:
	//pieces store their position in the 1 dimensional array, their rank and file calculated from this position, and their colour
	int position{ 0 };
	int file{ position / 8 };
	int rank{ position % 8 };
	colour piece_colour{ colour::None };
	
public:
	
	piece(int set_position, colour set_colour) : position{ set_position }, piece_colour{ set_colour }  {}
	
	virtual ~piece() {}

	//Get functions

	int get_position() const;

	int get_file() const;

	int get_rank() const;

	colour get_colour() const;

	//Virtual functions to be overloaded

	virtual void update_position(int new_position) = 0;

	virtual void find_pseudovalid_moves(const board& b) = 0;

	virtual std::array<move, 64> get_pseudovalid_moves() const = 0;

	virtual std::unique_ptr<piece> clone() = 0;

	virtual std::string get_character() = 0;

	virtual bool get_has_moved() const = 0;

};



#endif // !PIECE_H