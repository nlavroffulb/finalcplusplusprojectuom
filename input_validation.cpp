#include "pieces_class.h"
#include "board_game_class.h"
#include "stratego_game.h"
#include<algorithm>

//this cpp file contains function definitions for input validation functions from several classes.
//the main structure is that in the input validation function, all the valid input possibilities are determined and stored in a vector.
//this vector as well as the search parameter and a user message are then passed into a template function (as seen just below) which
//searches the possibilities vector using a lambda function and asks for new input if necessary.

//template used here because whether you are looking for a string in a vector of strings or an int in a vector of ints
//the code is the same except for the variable type. as it in happens, the only data type used in this case is strings.
template <class c_type> void check_input_for_match(std::vector<c_type> database, c_type& search_parameter, c_type& user_message) {
    bool no_match{ true };
    int counter{ 0 };

    while (no_match) {
        //we want the rules to be able to come up for the user no matter the situation so the following if statement goes through if 
        //user input  is '\rules'. this results in print_rules() function being called.
        if (search_parameter == "\\rules") {
            stratego_game::print_rules();
            std::cout << "Please enter the type of piece you would like to place (e.g Colonel)." << std::endl;
            std::cin >> search_parameter;
            check_input_for_match(database, search_parameter, user_message);

        }

        std::for_each(database.begin(), database.end(), [&search_parameter, &counter](std::string data_member) {
            if (search_parameter == data_member) { counter++; }
            });
        no_match = (counter == 1) ? false : true;
        if (no_match == true) {
            std::cout << "Invalid input." << user_message<< std::endl;
            std::cin >> search_parameter;
        }

    }

}

//short function to check that when a piece constructor is called with a certain team that it is valid.
//this function does not actually appear to the user since the choice of team is contained within the code
//however it is still a useful function in case the above situation were to change.
void pieces::team_check(std::string& team_input)const {

    while (team_input != "Red" && team_input != "Blue") {
        std::cout << "Invalid team. Please enter 'Red' or 'Blue'." << std::endl;
        std::cin >> team_input;
    }
}

//user inputs piece types when the board is being set up so this input needs to be checked. 
//could retrieve all the type names from the pieces class but in this case its faster to just
//make an array of them within the function and refer back to that.
//Although this particular function does not refer to any data members from the pieces class
//it has still been left in for possible future extension as if it were ever to use any
//class members they would be from the derived pieces classes.
void pieces::valid_piece_name(std::string& type_name) {

    std::vector<std::string> piece_types = { "Flag","Bomb","Spy", "Scout","Miner","Sergeant", "Lieutenant",
    "Captain", "Colonel","Major","General", "Marshal" };

    //customised warning message for this type of input validation.
    std::string user_warning{ "To see a list a list of valid piece names type \\rules." };
    check_input_for_match<std::string>(piece_types, type_name, user_warning);

}

//the following function will check any user input of the for A5 for example and make sure that its valid for the board dimensions
//and just in general.
//need access to width and height of board for correct validation so
//this function is a member function of the board_game class. 
void board_game::alphanumeric_input_check(std::string& alphanumeric)const {

    std::vector<char> possible_letters;
    std::vector<int> possible_numbers;
    std::vector<std::string> possible_positions(squares);

    //combined 2 for loops in a 3rd for loop to determine all the possible alphanumeric positions for this specific board game configuration.
    for (int i{ 0 }; i < height; i++) {
        possible_letters.push_back('A' + i);
    }
    for (int j{ 1 }; j <= width; j++) {
        possible_numbers.push_back(j);
    }
    for (int i{ 0 }; i < height; i++) {
        for (int j{ 0 }; j < width; j++) {
            possible_positions.push_back(possible_letters[i] + std::to_string(possible_numbers[j]));//this adds all possible positions e.g "I" + "9"
        }
    }

    //customised warning message for this type of input validation.
    std::string user_warning{ "Please enter a position of the form A7." };
    check_input_for_match<std::string>(possible_positions, alphanumeric, user_warning);

}

//need access to number of squares on board so this is a member function of the board game class. 
//useful for checking that a key to the board map is not out of bounds of the board.
void board_game::check_position_exists(int& some_position)const {
    while (some_position > squares || some_position <= 0) {
        std::cout << some_position << " is an invalid position as there are only "
            << squares << " positions on the board. Please choose another position" << std::endl;
        std::cin >> some_position;
    }
}

//the following are not input validation functions but these overloaded functions convert between 
//alphanumeric style for position (how position on the board is inputted) to a number
//lying between 1 and the total number of squares (which is used as the key to the board map object).

//they both use member functions or some member data of the board_game class so they
//both need to be member functions of the board_game class themselves.
int board_game::alphanumeric_integer_position_converter(std::string string_position) const{

    //assuming that the input is correct since it has been passed through the previous validation function, 
    //by using erase we can isolate both parts of the alphanumeric namely the letter and the number.
    std::vector<char> string_position_array{ string_position.begin(), string_position.end() };
    int numeric_part{ std::stoi(string_position.erase(0,1)) };

    return (1 + string_position_array[0] - 'A') + width*(numeric_part-1);

}

std::string board_game::alphanumeric_integer_position_converter(const int& int_position)const {

    //since we want a string we need to distinguish between numbers with two digits and those with only one
    //which is what the following if else statement does.
    if (int_position > (height-1)*width && int_position <= squares) {
        char letter = 'A' + horizontal_component(int_position);

        auto alphanumeric_position = std::string(1, letter) + "10";//compiler needs to deduce the type of the variable so it is declared 
        //as an auto variable.

        return alphanumeric_position;

    }
    else {
        char letter = 'A' - 1 + horizontal_component(int_position);
        char number = '0' + vertical_component(int_position);

        auto alphanumeric_position = std::string(1, letter) + number;

        return alphanumeric_position;
    }
}



//user is allowed to pick how many pieces are on each side. in this case the input is checked against classic Stratego
//game settings as the games themselves are played on a classic 10 by 10 Stratego board. As a consequence it is
int stratego_game::pieces_number_check(std::string& input, const int& min_num, const int& max_num) {

    //as per, just need to determine all the valid inputs and compare to the input using the function defined at the top of this file.
    std::vector<std::string> possible_piece_numbers;
    for (int i{ min_num }; i <= max_num; i++) {
        possible_piece_numbers.push_back(std::to_string(i));
    }
    bool no_match{ true };
    int counter{ 0 };
    std::string user_message{ "Please choose a number between " + std::to_string(min_num) + " and " + std::to_string(max_num) };//customized message for
    //this type of input.

    check_input_for_match(possible_piece_numbers, input, user_message);

    return std::stoi(input);//because the input we receive is a string but we want it to be an integer, we can't modify it and pass back by
    //reference so this function needs an integer return value.
    
}

