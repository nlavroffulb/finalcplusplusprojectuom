#include "stratego_game.h"
#include"windows_header_class.h"
#include<thread>
#include<chrono>

//implementation of the parameterized constructor for a stratego_game object.
//sets the dimensions of the board and fills each position on the map with a nullptr.
stratego_game::stratego_game(const int& vertical_squares, const int& horizontal_squares) {
    height = vertical_squares;
    width = horizontal_squares;

    squares = height*width;
    //filling the map with nullptrs since at first the board is empty.
    for (int i{ 1 }; i < squares + 1; i++) {
        the_board[i] = nullptr;
    }

}

//function which takes user input on what mode they wish to 'play' or see and then
//calls other functions depending on that input. 
void stratego_game::start_game() {
    std::cout << "Please enter \n1 for the one player demonstration\n2 for the two player game"
        "\nD for a demonstration of display allowed moves \nor 0 to exit." << std::endl;

    char game_mode;
    std::cin >> game_mode;

    //input validation
    while (game_mode < '0' && game_mode>'2' && game_mode != 'D') {
        std::cout << "Invalid input. Please try again." << std::endl;
        std::cin >> game_mode;
    }
    stratego_game board_example(10, 10);

    //call functions depending on input or exit program.
    (game_mode == '1') ? board_example.one_player_demo() :
        ((game_mode == '2') ? board_example.two_player_game() :
        ((game_mode == 'D') ? board_example.display_allowed_moves_demo() : exit(1)));



}

//this function merely outputs the rules of stratego to the console.
void stratego_game::print_rules() {
    std::cout << "There are 40 pieces per player in classic Stratego. In ascending order of strength they are:" << std::endl;
    std::cout << "1 Flag,  1 Spy, 8 Scouts, 5 Miners, 4 Sergeants, 4 Lieutenants, 3 Majors, 2 Colonels, 1 General, 1 Marshal and 6 Bombs,.\n" << std::endl;
    std::cout << "You are required to place your pieces on your side of the board before the first turn.\n" << std::endl;
    std::cout << "Higher strength pieces always win against lower strength pieces EXCEPT FOR the cases where:"
        << "\na Spy attacks a Marshal (but not if a Marshal attacks a Spy).\na Miner attacks a Bomb." << std::endl;
    std::cout << "\nAdditionally, the Flag and all Bombs cannot move. All other pieces can move 1 square vertically or horizontally"
        << "\nEXCEPT FOR Scouts which can move in a straight line for as many pieces vertically or horizontally as there are before"
        << "\nthey encounter another piece or the end of the board." << std::endl;
    std::cout << "\nThe game ends when one player destroys the opposing player's flag or one player has no more moveable pieces." << std::endl;

    std::cout << "At the beginning of the game, each player must place all their pieces on their side of the board only"
        << "\n and always below the impassable terrain for the Red team above the impassable terrain for the Blue team." << std::endl;
    std::cout << "Playing in fullscreen is recommended." << std::endl;
    std::cout << "If at any point you wish to see the rules again, when asked for any kind of input, just type \\rules." << std::endl;
}

//stratego requires a preparation phase before any moves can be made where each player chooses where to place their pieces.
//this stage is controlled by this function.
void stratego_game::preparation_phase(const std::string& player_team, const int& pieces_num) {

    //clear any text that might have already been on the console. this makes things clearer.
    windows_header::clear_50_lines(0, 45);

    //pieces_num here is the number of pieces per player. so for each player it loops through all their pieces
    //and the players chooses where to place them via user input.
    for (int i{ 1 }; i <= pieces_num; i++) {
        std::string selected_piece;
        std::string selected_position;
        int max_position{ (player_team == "Red") ? 40 : 100 };
        int min_position{ (player_team == "Red") ? 0 : 60 };
        int position;

        //formatted info to user.
        windows_header::set_colour((player_team == "Red") ? 4 : 1);
        std::cout << player_team;
        windows_header::set_colour(7);
        std::cout << " turn to play." << std::endl;


        //error message to user if they pick a position that is outside the board. it is outputted later on in the program.
        std::string user_message{ "Invalid position. Please try again. " +
        ((i == 1) ? "You cannot place your piece above " + std::to_string(max_position) + " or below" + std::to_string(min_position)
            : "You cannot place your pieces below " + std::to_string(min_position)) + " or above " + std::to_string(max_position) };

        //each player can only have one flag but they must have at least one. that is what is coded for in the if block below.
        //as well as input validation.
        if (i == pieces_num && flag::get_flag(player_team) != 1) {
            std::cout << "You have not yet picked a flag but this is required. Please enter your flag's position." << std::endl;
            std::cin >> selected_position;
            alphanumeric_input_check(selected_position);

            position = alphanumeric_integer_position_converter(selected_position);
            while (position > max_position || position < min_position || the_board[position] != nullptr) {
                std::cout << user_message << std::endl;
                std::cin >> selected_position;
                alphanumeric_input_check(selected_position);

            }
            selected_piece = "Flag";

        }
        //aside from the Flag players are free to choose whatever pieces they like so they can deviate from classical Stratego if they please.
        else {
            //pick which position you want to place your piece on
            std::cout << "Please enter the type of piece you would like to place (e.g Colonel)." << std::endl;
            std::cin >> selected_piece;
            pieces::valid_piece_name(selected_piece);

            //you cannot have more than one flag
            while (selected_piece == "Flag" && flag::get_flag(player_team) > 0) {
                std::cout << "Invalid choice of piece. There can only be one flag per team. Please pick another piece." << std::endl;
                std::cin >> selected_piece;
                pieces::valid_piece_name(selected_piece);
            }
            //ask for user input
            std::cout << "Where would you like to position this piece? Please enter in the form A6 for example" << std::endl;
            std::cin >> selected_position;
            alphanumeric_input_check(selected_position);
            position = alphanumeric_integer_position_converter(selected_position);

            //more input validation
            while (position > max_position || position < min_position || the_board[position] != nullptr) {
                std::cout << user_message << std::endl;
                std::cin >> selected_position;
                alphanumeric_input_check(selected_position);
                position = alphanumeric_integer_position_converter(selected_position);

            }

        }

        COORD current_pos{ windows_header::get_current_cursor_coordinates() };

        //here we call a function to add the piece to the board after checking that the input is valid and converting it from alphanumeric to integer.
        add_piece(alphanumeric_integer_position_converter(selected_position), create_piece(selected_piece, i + pieces_num, player_team));
        
        windows_header::set_cursor_position(current_pos.X, current_pos.Y + 2);//move cursor down a bit.

    }

}

//this function determines the outcome of every possible matchup between pieces.
void stratego_game::attack_enemy_piece(int starting_position, int end_position) {
    std::shared_ptr<pieces> winner{ nullptr };
    check_position_exists(starting_position);
    check_position_exists(end_position);

    std::string unknown_piece_type{ the_board[end_position]->get_type() };

    //if the attacking and defending pieces do not have a special interaction with another or are not bombs then its business
    //as usual. The winner is decided by whoever has the higher rank.
    if (move_allowed(starting_position, end_position) == false) { return; }
    else if (the_board[starting_position]->special_interaction() != the_board[end_position]->special_interaction()
                && the_board[starting_position]->rank() != 11 && the_board[end_position]->rank() != 11) {

                winner = (the_board[starting_position]->rank() > the_board[end_position]->rank())
                ? the_board[starting_position] : the_board[end_position];
        //destroyed_piece_type = (winner == the_board[starting_position]) ? the_board[end_position]->get_type() : "";
         }
    //special interactions
    else if (the_board[starting_position]->rank() != the_board[end_position]->rank()) {

        if (the_board[starting_position]->special_interaction() == "bomb-miner") {
            winner = (the_board[starting_position]->rank() == 3) ? the_board[starting_position] : the_board[end_position];
        }
        if (the_board[starting_position]->special_interaction() == "spy-marshal") {
            winner = (the_board[starting_position]->rank() == 10) ? the_board[starting_position] :
                (the_board[starting_position]->rank() == 1) ? the_board[starting_position] : the_board[end_position];

        }
        //attacking a bomb. it is itself destroyed in the process.
        else if (the_board[end_position]->rank() == 11 && the_board[starting_position]->rank() != 3) {
            winner = the_board[end_position];
            the_board[end_position] = nullptr;

        }

    }
    //attacking piece of the same rank
    else if (the_board[starting_position]->rank() == the_board[end_position]->rank()) {
        std::cout << "The enemy had the same rank as you ("<<unknown_piece_type<< ") and thus you have both been defeated." << std::endl;
        winner = the_board[end_position];
        the_board[end_position] = nullptr;
    }

    //preparing user messages depending on outcome.
    std::string user_message{ (winner == the_board[starting_position])
    ? "Your piece destroyed the enemy piece (" + unknown_piece_type + ") and moved to their position." 
        : "Your piece was destroyed by an enemy "+unknown_piece_type };
    std::cout << user_message << std::endl;

    //need to reshuffle the map depending on the outcome of the attack.
    if (winner == the_board[starting_position]) {
        the_board[end_position] = winner;
    }
    else if (winner == the_board[end_position]) {
    }
    the_board[starting_position] = nullptr;

}

//this function runs the one player demonstration mode including introductory output
//and creating pieces for the AI.
void stratego_game::one_player_demo() {

    std::string piece_types[] = { "Flag","Bomb","Spy", "Scout","Miner","Sergeant", "Lieutenant",
        "Captain", "Colonel","Major","General", "Marshal" };

    print_game_board();//start off by printing the board
    std::cout << "Welcome to this one player demonstration of classic Stratego (10 by 10 board).\n";
    print_rules();
    std::cout << "To start off with you must pick the number of pieces per team. In classic Stratego there are";
    std::cout << " 40 pieces per player.\nThe more pieces the longer the demonstration takes.\n";

    std::cout << "When you have read the instructions and are ready to proceed enter 1\n" << std::endl;
    std::string input;
    std::cin >> input;
    while (input != "1") {
        std::cout << "When you have read the instructions and are ready to proceed enter 1" << std::endl;
        std::cin >> input;

    }

    std::cout << "Please enter a number between 2 and 40 for the number of pieces per player.\n";

    //getting and checking user input.
    int num_of_pieces{ 0 };
    std::cin >> input;
    int created_objects{ 0 };
    num_of_pieces = pieces_number_check(input, 2, 40);

    //ready for preparation phase. the human player is the "red" team by default.
    preparation_phase("Red", num_of_pieces);
    COORD current_cursor_pos{ windows_header::get_current_cursor_coordinates() };
    add_piece(61, create_piece("Flag", 2, "Blue"));//add a flag for the AI. 



    //creating pieces for the 'AI'
    while (pieces::get_number_of_piece_objects() < 2 * num_of_pieces) {
        add_piece(62 + created_objects, create_piece(piece_types[6], created_objects, "Blue"));//the rest of the AI pieces are all Lieutenants.
        created_objects++;
    }

    //its always the red players turn so blue player's squares should always be filled
    colour_certain_squares("Blue");
    windows_header::set_cursor_position(current_cursor_pos.X, current_cursor_pos.Y);
    main_gameplay_loop(false);//run the main gameplay loop with an argument that tells it that there is only one human and 
    //so to skip the turn of the other player.


}

//function which controls the running of the two player game mode.
void stratego_game::two_player_game() {

    print_game_board();
    int pieces_num;
    std::cout << "Welcome to 2-player classic Stratego.\n" << std::endl;
    print_rules();
    std::cout << "To start off please enter the number of pieces per team you would like. This must be a number "
        << "between 2 and 40.\n" << std::endl;

    std::string input;
    std::cin >> input;
    pieces_num = pieces_number_check(input, 2, 40);

    //get confirmation that users are ready. 
    std::cout << "When you have read the instructions and are ready to proceed enter 1" << std::endl;
    std::cin >> input;

    //input validation
    while (input != "1") {
        std::cout << "When you have read the instructions and are ready to proceed enter 1" << std::endl;
        std::cin >> input;

    }

    //loop between the two players. display information for each and run the preparation phase for each player.
    for (int player_num{ 1 }; player_num <= 2; player_num++) {
        std::string player_team{ (player_num == 1) ? "Red" : "Blue" };
        windows_header::set_colour((player_team == "Red") ? 4 : 1);
        std::cout << "Player " << player_num << "'s turn to place their pieces." << std::endl;
        windows_header::set_colour(7);
        preparation_phase(player_team, pieces_num);
        colour_certain_squares(player_team);

    }

    //more information about how the game proceeds after preparation phase
    std::cout << "The pieces for both players have now been placed.\nThe red team goes first." << std::endl;
    std::cout << "Each turn you must pick a piece to move and a valid destination. \nFlags and bombs are immobile"
        << " while scouts can move as many squares as possible in a straight (vertical or horizontal) line "
        << "until they reach another piece (friendly or enemy) or the end of the board. \nAll other pieces can "
        "move 1 square horizontally or vertically.\n" << std::endl;

    main_gameplay_loop(true);//ready to run the main gameplay loop.


}

//this function handles the running of the mode which gives a demonstration of the allowed moves function
//defined in the stratego game virtual functions file. it mainly prints information to the user then gets
//input to set up the demo and finally calls the allowed move function.
void stratego_game::display_allowed_moves_demo() {

    //set up and introduction to the user.
    print_game_board();
    windows_header::clear_50_lines(0, 45);
    std::cout << "Welcome to this demonstration of a display allowed moves function for a given piece." << std::endl;
    std::cout << "To start off with you'll need to decide how many pieces you want for this demonstration."
        << "\nNote all the exceptions of the function can be demonstrated using only 4 pieces.";
    std::cout << "\nThese rules are:\nYou cannot move to a position that is occupied by a friendly piece."
        << "\nScouts can move as many squares in a straight horizontal or vertical line as possible"
        << "\nbefore being blocked by another piece or reaching the end of the board.\n"
        << "\nAll other pieces can only move vertically or horizontally 1 square at a time.\n" << std::endl;

    std::cout << "How many pieces in total would you like to add to the board? \nMust be an even number between 4 and 80."
        << "\nOdd number inputs above 4 will be rounded up." << std::endl;

    //input and validation
    std::string input;
    int num_of_pieces{ 0 };
    std::cin >> input;
    num_of_pieces = pieces_number_check(input, 4, 80);
    if (num_of_pieces % 2 != 0) { num_of_pieces++; }//if the user says theyd like 5 pieces in total this is rounded up so that each team
    //has an equal number of pieces.

    //run the preparation phase for both teams.
    preparation_phase("Red", num_of_pieces / 2);
    preparation_phase("Blue", num_of_pieces / 2);

    //get more input about which position to demonstrate the function on and of course validate that input.
    std::string selected_position;
    std::cout << "Please select the position of the piece for which you would like to see the allowed moves." << std::endl;
    std::cin >> selected_position;
    alphanumeric_input_check(selected_position);
    int position{ alphanumeric_integer_position_converter(selected_position) };

    //validation
    while (the_board[position] == nullptr) {
        std::cout << "Invalid choice for position. Please enter a position that is occupied." << std::endl;
        std::cin >> selected_position;
        alphanumeric_input_check(selected_position);
        position = alphanumeric_integer_position_converter(selected_position);
    }

    //input is ok. run the demonstration.
    display_allowed_moves(position);


    //user is given the option to restart the demonstration. otherwise start game is called so that user is called
    //back to the start screen and can there decide what to do.
    std::cout << "Would you like to re-run this demonstration? Yes/No" << std::endl;
    std::string response;
    std::cin >> response;

    while (response != "Yes" && response != "No") {
        std::cout << "Please enter Yes or No." << std::endl;
        std::cin >> response;
    }

    (response == "Yes") ? display_allowed_moves_demo() : start_game();

}


