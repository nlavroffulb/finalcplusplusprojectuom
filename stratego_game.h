#pragma once

#include<map>
#include "pieces_class.h"
#include "board_game_class.h"

//this header file contains the stratego_game class which is derived from the board_game class. it is a class to describe 
//the main functions involved in playing stratego which includes virtual as declared in the base class and plain and simple
//member functions.

//where possible functions and their arguments are made const to protect member data and function arguments. additionally where possible,
//arguments are passed by reference either for void functions or to for compilation speed.

class stratego_game : public board_game {
public:
    
    stratego_game(const int& height, const int& width);

    //definition of board_game class virtual functions.
    std::shared_ptr<pieces> create_piece(std::string type, int id, std::string team);
    void print_game_board();
    void add_piece(int starting_position, const std::shared_ptr<pieces>& some_piece);
    void move_piece_on_board(const int& colour_code, const std::string& piece_display_name, int start_position, int end_position);
    bool move_allowed(int starting_position, int end_position);
    void display_allowed_moves(int position);
    void main_gameplay_loop(const bool& human_opponent);
    bool game_over(const int& turn);
    void name_square_in_grid(const int& sleep_for, const int& colour_code, const std::string& name, const int& x, const int& y);
    void colour_certain_squares(const std::string& team);
    void name_certain_squares(const std::string& some_team);

    //the rest of the functions are not generally specific to board games but they are to Stratego hence they are not
    //virtual functions of board_game class but they are member functions of stratego_game class.

    static void start_game();//static because we need to be able to call it without reference to a stratego_game object.

    static void print_rules();//this function is static because although it is specific to Stratego and thus belongs in the
    //board game class, it is standalone in that it shouldn't need to be called by specifying a stratego_game object.

    void preparation_phase(const std::string& player_team, const int& pieces_num);
    void attack_enemy_piece(int starting_position, int end_position);
    void one_player_demo();
    void two_player_game();
    void display_allowed_moves_demo();

    //input validation
    int pieces_number_check(std::string& input, const int& min_num, const int& max_num);
    
    ~stratego_game() { std::cout << "GAME OVER"; }


};
