#include "board_game_class.h"



//this header file contains the function definitions for the board game class (specifically
//those that are not declared as virtual).

//this is a 'get' function to find the piece situated at a certain point. it uses a map iterator with find() from the algorithm header 
//and the map function -> second to look through the map, find a match and retrieve it.
std::shared_ptr<pieces> board_game::get_piece(int position) {
    check_position_exists(position);
    
    grid::iterator board_iterator{ the_board.find(position) };
    return board_iterator->second;

}

//in the map object pieces are stored in as numbers e.g A1 is 1 and J10 is 100 for a 10 by 10 board.
//the next two functions take a numerical version of the position and essentially return their
//x and y values. these functions are particularly useful in the determination of allowed moves as
//there are no pieces which move diagonally.
int board_game::vertical_component(const int &position) const {
    double integer_part, fraction_part;
    double position_to_width{ (0.0 + position) / width };

    fraction_part = std::modf(position_to_width, &integer_part);
    std::string integer{ std::to_string((fraction_part == 0.0) ? integer_part : integer_part + 1) };
    integer = integer.substr(0, integer.find('.', 0));

    return std::stoi(integer);
}

int board_game::horizontal_component(const int &position) const {

    return position - (vertical_component(position) - 1) * width;
}

