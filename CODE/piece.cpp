//piece.cpp

#include <iostream>
#include <array>
#include <string>
#include <memory>
#include "piece.h"
#include "piece.h"
#include "board.h"

int piece::get_position() const
{
	return position;
}

colour piece::get_colour() const
{
	return piece_colour;
}


int piece::get_file() const
{
	return file;
}

int piece::get_rank() const
{
	return rank;
}
