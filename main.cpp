//this is the main function for a C++ program of the board game Stratego. 
//The game is itself playable and a lot of the code is meant to demonstrate the key features of object oriented programming and c++.
//Object Oriented Programming in C++ Final Project Nikolai Lavroff 11/5/2020
//University of Manchester


//inclusion of all the necessary header files.
#include "pieces_class.h"
#include "board_game_class.h"
#include "stratego_game.h"
#include "windows_header_class.h"

//the main function is only used to welcome the user to the program and call the start_game() function which links
//to every other functionality in this program.
int main()
{

	std::cout << "Welcome to Stratego: C++ Console Edition!!!\n"
		"This version supports a two player same display game or a one player demonstration."<< std::endl;
	
	stratego_game::start_game();


	return 0;
}

