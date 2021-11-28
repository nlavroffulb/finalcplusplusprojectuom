#include <iostream>
#include<string>
#include<utility>
#include<map>
#include "pieces_class.h"
#include<vector>
//here the board game abstract base class is defined. notable features are the map container for storing the pieces objects and their positions.

//where possible functions and their arguments are made const to protect member data and function arguments. additionally where possible,
//arguments are passed by reference either for void functions or to for compilation speed.

typedef std::map < int, std::shared_ptr<pieces> > grid;//map container definition
class board_game {
protected:

    grid the_board;
    int squares{ 0 };
    const int square_width{ 5 }, square_height{ 5 };//we set the dimensions for a square for any board that's created.
    int height{ 0 }, width{ 0 };//dimensions of the board e.g 10 squares by 10 squares.
public:

    //get piece at given position and functions to get the coordinates of a position on the board.
    std::shared_ptr<pieces> get_piece(int position);
    int vertical_component(const int &position) const;
    int horizontal_component(const int &position) const;


    //input validation functions for position as an integer (number between 1 and squares) and as an alphanumeric e.g A6.
    void check_position_exists(int& some_position) const;
    void alphanumeric_input_check(std::string& alphanumeric) const;

    //the board is linked to piece objects by a map container which takes an integer however users are asked for position in an alphanumeric
    //position form such as A6 and so it is necessary to convert between these two as part of the input/output interface.
    std::string alphanumeric_integer_position_converter(const int& position) const;
    int alphanumeric_integer_position_converter(std::string position) const;

    //virtual functions to format the display of the game on the console
    virtual void print_game_board()=0;
    virtual void name_square_in_grid(const int& sleep_time, const int& colour_code, const std::string& name, const int& x, const int& y)=0;
    virtual void colour_certain_squares(const std::string& identifier)=0;
    virtual void name_certain_squares(const std::string& some_team)=0;

    //various functions controlling creating, adding and moving pieces on the board as well as playing the actual game.
    virtual std::shared_ptr<pieces> create_piece(std::string type, int id, std::string team)=0;
    virtual void add_piece(int starting_position, const std::shared_ptr<pieces>& some_piece)=0;
    virtual void move_piece_on_board(const int& colour_code, const std::string& piece_display_name, int start_position, int end_position)=0;
    virtual bool move_allowed(int starting_position, int end_position)=0;
    virtual void display_allowed_moves(int position)=0;
    virtual void main_gameplay_loop(const bool& human_opponent) = 0;
    virtual bool game_over(const int& turn) = 0;

    virtual ~board_game() {};//since it is an abstract base class it cannot be instantiated by a constructor but
    //it still has a virtual destructor which will default to the destructor of whichever derived class object is being called.

};


#pragma once
