s#include "stratego_game.h"
#include "pieces_class.h"
#include "windows_header_class.h"
#include<thread>
#include<chrono>
#include<algorithm>

//this header file contains the bulk of functions in this project. 
//there are a variety of functions that generally cover just playing the game in the console and implementing its rules.

//disclaimer: this is not a member function of the stratego_game class but it is a piece of code that is used regularly
//in the following code.
//a find function for a vector database which uses lambda functions with the std::for_each from the algorithm header.
bool find_match(std::vector<int> database, int search_parameter) {
    bool no_match{ true };
    int counter{ 0 };

    while (no_match) {
        std::for_each(database.begin(), database.end(), [&search_parameter, &counter](int data_member) {
            if (search_parameter == data_member) { counter++; }
            });
        return (counter == 0) ? false : true;

    }


}

//this function makes a new shared pointer to a piece object based on the arguments that are called (and then passed
//onto the relevant constructors).
std::shared_ptr<pieces> stratego_game::create_piece(std::string type, int id, std::string team)
{
    //try catch block in case of memory allocation failure. 
    try {
        if (type == "Flag") {
            return std::make_shared<flag>("Flag" + id, team);
        }
        if (type == "Bomb") {
            return std::make_shared<bomb>("Bomb" + id, team);
        }
        if (type == "Scout") {
            return std::make_shared<scout>("Scout" + id, team);
        }
        if (type == "Spy") {
            return std::make_shared<spy>("Spy" + id, team);
        }
        if (type == "Miner") {
            return std::make_shared<miner>("Miner" + id, team);
        }
        if (type == "Sergeant") {
            return std::make_shared<sergeant>("Sergeant" + id, team);
        }
        if (type == "Lieutenant") {
            return std::make_shared<lieutenant>("Lieutenant" + id, team);
        }
        if (type == "Colonel") {
            return std::make_shared<colonel>("Colonel" + id, team);
        }
        if (type == "Major") {
            return std::make_shared<major>("Major" + id, team);
        }
        if (type == "General") {
            return std::make_shared<general>("General" + id, team);
        }
        if (type == "Marshal") {
            return std::make_shared<marshal>("Marshal" + id, team);
        }
        else {
            return nullptr;//generally the input is validated before this stage but in case it isnt, return a nullptr.
        }

    }
    catch(std::bad_alloc memFail){
        std::cerr << "Memory allocation failure" << std::endl;
        exit(1);

    }

}

//prints the game board to the console (with no pieces on it)
void stratego_game::print_game_board()
{

    for (int i{ 'A' }; i <= width + 64; i++) {
        windows_header::set_cursor_position(5 + 9 * (i - 65), 42);
        std::cout << char(i);

    }

    for (int j{ 1 }; j <= height; j++) {
        windows_header::set_cursor_position(92, 39 - 4 * (j-1));

        std::cout << j;

    }


    for (int k{ 0 }; k < width; k++) {
        for (int m{ 0 }; m < height; m++) {
            for (int i{ 1 }; i <= square_width; i++) {
                windows_header::set_cursor_position(k * 9, i + 4 * m);

                for (int j{ 1 }; j <= 2 * square_height; j++)
                {
                    if (i == 1 || i == square_width || j == 1 || j == 2 * square_height) {
                        std::cout << "*";
                    }
                    else {
                        std::cout << " ";
                    }
                }
                std::cout << "\n";
            }

        }
    }
    if (squares == 100) {//classic stratego
        int h{ square_width - 2 }, w{ 2 * square_width - 2 };

        int impassable_squares[] = { 43,44,47,48,53,54,57,58 };
        std::vector<int> v{ impassable_squares, impassable_squares + 8 };
        for (int m{ 0 }; m < 8; m++) {
            for (int i{ 0 }; i < h; i++) {
                for (int j{ 0 }; j < w; j++) {
                    windows_header::set_cursor_position(1 + 9 * (horizontal_component(v[m]) - 1) + j, 4 * (vertical_component(v[m])) - 2 + i);
                    windows_header::set_colour(244);//white background with red text.
                    std::cout << "X";
                }
                std::cout << "\n";
            }

        }

        windows_header::set_cursor_position(0, 45);
        windows_header::set_colour(7);

    }
    for (int i{ 1 }; i <= squares;i++) {
        the_board[i] = nullptr;
    }


}

//this works in tandem with create_piece to assign the piece to the map and display it in the console.
void stratego_game::add_piece(int starting_position, const std::shared_ptr<pieces>& some_piece) {
    check_position_exists(starting_position);//input validation

    //this statement checks that the user isnt trying to add a piece to a position that is already occupied
    //and also need to check that the selected piece has been created.
    if (the_board[starting_position] == nullptr && some_piece != nullptr) {
        the_board[starting_position] = some_piece;

        //next block of code chooses what to display on the board and in what colour (according to the team).
        int h{ square_height - 1 }, w{ 2 * square_width - 1 };
        int colour{ (the_board[starting_position]->get_team() == "Red" ? 4 : 1) };

        std::string piece_name{ (some_piece->get_type() != "Sergeant" && some_piece->get_type() != "Lieutenant") ? some_piece->get_type() :
        (some_piece->get_type() == "Sergeant") ? "S'geant" : "L'tenant" };//sergeant and lieutenant are too small to print on the current 
        //square configuration so they are shortened slightly.

        //here call the function that outputs to the correct position in the console.
        name_square_in_grid(1,colour, piece_name, horizontal_component(starting_position), vertical_component(starting_position));

        //for each function want to make sure we always leave the cursor of the console in the same position as where it started.
        windows_header::set_cursor_position(0, 45);
        windows_header::set_colour(7);//as well as colour.


    }
    else {
        std::cout << "Cannot add piece to that position." << std::endl;
    }
}

//this function is responsible for reassigning the piece objects between the map as they change position.
void stratego_game::move_piece_on_board(const int& colour_code, const std::string& piece_display_name, int start_position, int end_position) {

    //input validation
    check_position_exists(start_position);
    check_position_exists(end_position);

    //checking whether the end position is empty or not. if it is then move it if not then attack.
    if (get_piece(end_position) == nullptr) {
        the_board[end_position] = the_board[start_position];//reassignment. shared pointer count goes up by 1.
        the_board[start_position] = nullptr;//shared pointer count goes back down.
        std::cout << "Your piece was moved successfully." << std::endl;

    }
    else {
        attack_enemy_piece(start_position, end_position);//
    }


    //next block of code clears any text or colours in the old position and re-outputs to the new position.
    COORD current_pos1{ windows_header::get_current_cursor_coordinates() };
    windows_header::set_cursor_position(current_pos1.X, current_pos1.Y + 1);

    int width{ 8 }, height{ 3 };//dimensions of area per square to clear/colour in/name.

    int x1{ horizontal_component(start_position) }, y1{ vertical_component(start_position) };
    int x2{ horizontal_component(end_position) }, y2{ vertical_component(end_position) };

    for (int i{ 0 }; i < height; i++) {
        for (int j{ 0 }; j < width; j++) {
            windows_header::set_cursor_position(1 + 9 * (x1 - 1) + j, 40 - 4 * (y1 - 1) - 2 + i);
            std::cout << " ";
        }
    }

    for (int i{ 0 }; i < height; i++) {
        for (int j{ 0 }; j < width; j++) {
            windows_header::set_cursor_position(1 + 9 * (x2 - 1) + j, 40 - 4 * (y2 - 1) - 2 + i);
            std::cout << " ";
        }
    }

    windows_header::set_cursor_position(1 + 9 * (x2 - 1), 40 - 4 * (y2 - 1) - 1);
    windows_header::set_colour(colour_code);
    std::cout << piece_display_name;
    std::this_thread::sleep_for(std::chrono::seconds(1));//here we use sleep_for to delay the time until the next line.
    //this is so that the user has a bit of time to see where their pieces have been placed before the turn changes.
    windows_header::set_cursor_position(current_pos1.X, current_pos1.Y);


}

//each time a move is made it must be check against the game rules to make sure it is valid. 
//that's what this function does.
bool stratego_game::move_allowed(int starting_position, int end_position)
{
    //code for attempt to move to the same position.
    check_position_exists(starting_position);
    check_position_exists(end_position);

    int vertical_distance{ std::abs(vertical_component(starting_position) - vertical_component(end_position)) };
    int horizontal_distance{ std::abs(horizontal_component(starting_position) - horizontal_component(end_position)) };

    std::vector<int> impassable_squares = { 43,44,47,48,53,53,57,58 };//in classic Stratego there 8 squares which you cannot move to. these are
    //encoded here.


    //cannot move an empty piece.
    if (get_piece(starting_position) == nullptr) {
        std::cout << "The starting position is empty. Move not allowed." << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        return false;
    }
    //cannot attack member of the same team
    else if (the_board[end_position] != nullptr && (the_board[starting_position]->get_team() == the_board[end_position]->get_team())) {
        std::cout << "Cannot attack piece on the same team" << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        return false;
    }
    //cannot move to the same position as you were before.
    else if (horizontal_distance == 0 && vertical_distance == 0) {
        std::cout << "Invalid move." << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        return false;
    }

    //special case for scouts. they can travel in as many squares in a straight vertical or horizontal line as possible until
    //encountering another piece, impassable terrain or the end of the board.
    else if (get_piece(starting_position)->rank() == 2) {

        //check that trajectory is a straight horizontal or vertical line then deal assign important parameters accordingly.
        if (vertical_distance == 0 || horizontal_distance == 0) {
            bool horizontal_movement{ (vertical_distance == 0) ? true : false };
            bool vertical_movement{ (horizontal_distance == 0) ? true : false };
            int distance, jump, direction;//distance is the number of squares travelled, jump is equal to 1 or the width ie if you're moving
            //horizontally, the way the board is coded, the position increases by 1 but if you move vertically the position increases by 
            //the width for every square you travel.
            if (horizontal_distance > 0) {
                distance = horizontal_distance;
                jump = 1;
                direction = (horizontal_component(starting_position) > horizontal_component(end_position)) ? -1 : 1;
            }
            else {
                distance = vertical_distance;
                jump = width;
                direction = (vertical_component(starting_position) > vertical_component(end_position)) ? -1 : 1 ;

            }
            //here we check for impassable terrain in the way, the end of the board or other pieces in the way. 
            int counter{ 0 };
            for (int i{ 1 }; i <= distance; i++) {
                if (get_piece(starting_position + i * direction*jump) == nullptr || get_piece(starting_position + i * direction * jump) == the_board[end_position]) {
                    bool impassable_terrain_met{ false };
                    
                    for (unsigned int k{ 0 }; k < impassable_squares.size(); k++) {
                        impassable_terrain_met = find_match(impassable_squares, starting_position + i * direction * jump);
                    }
                    if (impassable_terrain_met == false) {
                        counter++;
                    }
                }
            }
            if (counter != distance) { std::cout << "Illegal move. Way is blocked.\n"; }
            return (counter == distance) ? true : false;

        }//if one or other of the distances are not zero then either both are zero and its not a valid move because
        //it isn't a move or both are more than zero in which case its a diagonal travel which is not allowed.
        else {
            std::cout << "Illegal move." << std::endl;
            return false;

        }
    }
    //two special cases here: flags and bombs are immobile.
    else if (get_piece(starting_position)->rank() == 0 || get_piece(starting_position)->rank() == 11) {
        std::cout << "Illegal move, piece is immobile." << std::endl;
        return false;
    }
    //this last case deals with all other pieces (reminder they can only move 1 square horizontally or vertically each).
    else {
        if (vertical_distance == 0 || horizontal_distance == 0) {
            int sq{ squares };
            int counter{ 0 };

            //again checking that the end position is not impassable terrain.
            if (find_match(impassable_squares, end_position)) { counter++; }
            if (counter > 0) {
                std::cout << "That terrain is impassable. You cannot move your piece to that position." << std::endl;
                return false;

            }

            bool horizontal_movement{ (horizontal_distance == 1 && vertical_distance == 0) ? true : false };
            bool vertical_movement{ (vertical_distance == 1 && horizontal_distance == 0) ? true : false };
            if (horizontal_movement == true && vertical_movement == true) { std::cout << "Illegal move.\n"; }
            return (horizontal_movement == true || vertical_movement == true) ? true : false;
        }
        else {
            std::cout << "Illegal move." << std::endl;
            return false;
        }
    }

}

void stratego_game::display_allowed_moves(int position) {
    check_position_exists(position);

    //first check that selected position is not empty.
    if (the_board[position] == nullptr) {
        std::cout << "Cannot show allowed moves as selected piece is empty." << std::endl;
    }
    //special case for immobile pieces as per
    else if (the_board[position]->rank() == 11 || the_board[position]->rank() == 0) {
        std::cout << "Cannot show allowed moves as selected piece cannot be moved once placed." << std::endl;
    }
    //for the rest of the bunch...
    else {
        std::cout << "Allowed moves: " << std::endl;

        //special case again, scouts.
        if (the_board[position]->rank() == 2) {
            //next two loops loop in a straight line vertically and horizontally with respect to the position given.
            for (int n{ 0 }; n < height; n++) {
                if (n != vertical_component(position) - 1 && move_allowed(position, horizontal_component(position) + n * width)) {
                    std::cout << alphanumeric_integer_position_converter(horizontal_component(position) + n * width) << std::endl;

                }
            }
            for (int n{ 0 }; n < width; n++) {
                if (n != horizontal_component(position) - 1 && move_allowed(position, (vertical_component(position) - 1) * width + n + 1)) {
                    std::cout << alphanumeric_integer_position_converter((vertical_component(position) - 1) * width + n + 1) << std::endl;
                }
            }
        }
        //the rest of the rest.
        else {
            int neighbours[]{ position - 1, position + 1, position + 10, position - 10 };
            for (int i{ 0 }; i < 4; i++) {
                if ((neighbours[i] > 0 && neighbours[i] < squares) && move_allowed(position, neighbours[i])) {
                    std::cout << alphanumeric_integer_position_converter(neighbours[i]) << std::endl;
                }
            }
        }
    }

}

//this is the main block of code that controls the game after pieces have been placed.
//mainly, it switches turns between players, allows them to move their pieces and exits when the game is over.
void stratego_game::main_gameplay_loop(const bool& human_opponent){

    //pieces have been placed at this stage. By default the Red team starts. 
    std::string selected_position;
    std::string end_position_input;

    //for clarity in the console, we clear 50 lines below where the board has been outputted and start 
    //each demand for input at the same place every time.
    windows_header::clear_50_lines(0, 45);
    int turn{ 1 };

    //the main loop
    while (game_over(turn) == false) {

        //red and blue colours are used depending on which team is playing.
        std::string current_team{ (turn % 2 != 0) ? "Red" : "Blue" };
        int current_team_colour_code{ (turn % 2 != 0) ? 4 : 1 };
        
        //every turn the team that is not playing has their occupied squares filled with their colour so that the playing team cannot see their pieces.
        //this switches every turn so need to 'name'/display information on each piece at each position at the start of each players turn.
        name_certain_squares(current_team);
        windows_header::set_colour(current_team_colour_code);
        std::cout << current_team;
        windows_header::set_colour(7);

        //ask the user which piece to move and where to. also validate the input so far as checking that the position exists on the board.
        std::cout << " turn to play." << std::endl;
        std::cout << "Please select the position of the piece you want to move." << std::endl;
        std::cin >> selected_position;
        alphanumeric_input_check(selected_position);
        std::cout << "Please enter the end position for your move." << std::endl;
        std::cin >> end_position_input;
        alphanumeric_input_check(end_position_input);

        //convert our input into number form for the position.
        int start_pos{ alphanumeric_integer_position_converter(selected_position) }, end_pos{ alphanumeric_integer_position_converter(end_position_input) };

        //more input validation. checking that selected position is not empty.
        while (move_allowed(start_pos,end_pos)==false || the_board[start_pos]->get_team() != current_team) {
            std::cout << "That move is not allowed. Please select the position of the piece you want to move." << std::endl;
            std::cin >> selected_position;
            alphanumeric_input_check(selected_position);
            std::cout << "Please enter the end position for your move." << std::endl;
            std::cin >> end_position_input;
            alphanumeric_input_check(end_position_input);
            start_pos = alphanumeric_integer_position_converter(selected_position);
            end_pos = alphanumeric_integer_position_converter(end_position_input);


        }
        windows_header::clear_50_lines(0, 45);

        //we dont want to display anything on one of the start or end squares if one or both of the pieces got destroyed.
        std::string display_on_square{ (the_board[end_pos] != nullptr) ? the_board[end_pos]->get_type() : "" };
        move_piece_on_board(current_team_colour_code, display_on_square, start_pos, end_pos); 

        colour_certain_squares(current_team);

        //part of the main gameplay loop that differentiates the one player mode to the two player mode.
        //switch to other player's turn for two player mode, skip two turns for single human player for one player mode.
        (human_opponent == true) ? turn++ : turn += 2;

        windows_header::clear_50_lines(0, 45);


    }
    std::cout << "Play again?" << std::endl;//game only exits when user says they want to stop.
    start_game();

}

//returns true when the game is detected to be over otherwise it is false.
bool stratego_game::game_over(const int& turn) 
{
    //there are 2 victory conditions: destroy the enemy flag or destroy all their moving pieces.
    //both of these conditions are accounted for below.

    int red_mobile_pieces{ 0 }, blue_mobile_pieces{ 0 };

    //loop through all the squares and check if each team has mobile pieces left.
    for (int i{ 1 }; i <= squares; i++) {
        if (the_board[i] != nullptr) {
            if (the_board[i]->get_team() == "Blue" && the_board[i]->get_type() != "Flag" && the_board[i]->get_type() != "Bomb") {
                blue_mobile_pieces++;
            }
            if (the_board[i]->get_team() == "Red" && the_board[i]->get_type() != "Flag" && the_board[i]->get_type() != "Bomb"){
                red_mobile_pieces++;
            }

        }
    }

    //determine the winning team and so which colour to output in.
    std::string winning_team{ (turn % 2 == 0 && red_mobile_pieces>0)? "Blue":"Red" };
    int winning_colour_code{ (turn % 2 == 0 && red_mobile_pieces>0) ?1:4 };
    std::string user_message{ (red_mobile_pieces == 0 || blue_mobile_pieces == 0) ? "The enemy has no more mobile pieces."
    : "You destroyed the enemy team's flag.\n" };

    //here we use a static function from the pieces class to determine if a flag has been destroyed and check the mobile pieces of each team.
    while (flag::get_flag_num() == 2 && blue_mobile_pieces > 0 && red_mobile_pieces>0) {
        return false;
    }

    //this block executes once the game is over. it outputs information to the user about the outcome of the game.
    std::cout << user_message;
    windows_header::set_colour(winning_colour_code);
    std::cout << winning_team;
    windows_header::set_colour(7);
    std::cout << " wins the game." << std::endl;
    return true;

}

//this function is used to fill all the squares of a team with their colour
void stratego_game::colour_certain_squares(const std::string& some_team)
{
    //COORD type variables come from windows header file. 
    //first line below gets the current cursor coordinates so we can come back at the end.
    COORD current_pos{ windows_header::get_current_cursor_coordinates() };
    int width{ 8 }, height{ 3 };

    int unicode_colour{ (some_team == "Red") ? 64 : 16 };//colour of fill

    //loop through each point in a square and fill it with the required background colour.
    for (int i{ 1 }; i <= squares; i++) {
        if (the_board[i]!= nullptr && the_board[i]->get_team() == some_team) {

            for (int k{ 0 }; k < height; k++) {
                for (int j{ 0 }; j < width; j++) {
                    windows_header::set_cursor_position(1 + 9 * (horizontal_component(i) - 1) + j, 40 - 4 * (vertical_component(i) - 1) - 2 + k);
                    windows_header::set_colour(unicode_colour);
                    std::cout << " ";
                }
            }
            windows_header::set_colour(7);//set colour back to normal. 7 is the normal unicode colour code.
        }
    }
    windows_header::set_cursor_position(current_pos.X, current_pos.Y);

}

//as previously discussed, this clears all the squares of one team then displays their name and strenght on those cleared squares.
void stratego_game::name_certain_squares(const std::string& some_team) {
    COORD current_pos{ windows_header::get_current_cursor_coordinates() };
    int colour_code{ (some_team == "Red") ? 4 : 1 };

    //loop through all the squares and print info on them if they have a piece that is of the desired team.
    for (int i{ 1 }; i <= squares; i++) {
        if (the_board[i] != nullptr && the_board[i]->get_team() == some_team) {
            name_square_in_grid(0, colour_code, the_board[i]->get_type(), horizontal_component(i), vertical_component(i));
        }
    }
    windows_header::set_cursor_position(current_pos.X, current_pos.Y);

}

//print information to a single square. takes arguments for how long you would like to see the printed info before
//moving on, the colour of the information, the content and finally the position in coordinate form.
void stratego_game::name_square_in_grid(const int& sleep_for, const int& colour_code, const std::string& name, const int& x, const int& y){
    int h{ square_height - 1 }, w{ 2 * square_width - 1 };//dimensions of a square not including their borders.

    for (int i{ 1 }; i < h; i++) {
        for (int j{ 1 }; j < w; j++) {
            windows_header::set_cursor_position(w * (x-1) + j, width*h - h * (y-1) - 3 + i);
            windows_header::set_colour(7);
            std::cout << " ";
        }


    }
    windows_header::set_cursor_position(w * (x - 1) + 1, width*h-h*y+3);
    windows_header::set_colour(colour_code);

    std::cout << name;
    windows_header::set_cursor_position(w * (x - 1) + 2, width * h - h * y + 2);
    std::cout << the_board[x + (y-1) * width]->rank();
    std::this_thread::sleep_for(std::chrono::seconds(sleep_for));//this allows each player to see where theyve placed a piece for a bit. (note they can
    //always scroll up a bit if they need to see it again).



}


