//title_screen.h
//class which contains the print function for the title screen and can load the board from a save file, or create a new board
#ifndef TITLE_SCREEN_H
#define TITLE_SCREEN_H

#include "board.h"

class title_screen
{
public:
	void show_first_screen();
	board get_board();
	board load_board();
};


#endif