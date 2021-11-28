#include "pieces_class.h"
//this is the cpp file for the pieces base class and its derived classes. the bulk of this file
//is implementations of the parameterized constructor for each derived class object.

//these are two static variables that keep track of each team's flag.
int flag::red_flag{0};
int flag::blue_flag{ 0 };
int pieces::existing_piece_objects{ 0 };
//nearly all the constructor implementations have the same structure. 

//for example for the flag constructor the id and team data members are assigned and team is checked.
//otherwise the flag constructor is special because it is also the place that keeps track of the existence of the flag
//for each team.
flag::flag(std::string identification, std::string red_or_blue_team)
{
    id = identification;
    team_check(red_or_blue_team);
    team = red_or_blue_team;
    if (red_or_blue_team == "Red") {
        red_flag++;
    }
    else { blue_flag++; }
    
    existing_piece_objects++;
}

bomb::bomb(std::string identification, std::string red_or_blue_team)
{
    id = identification;
    team_check(red_or_blue_team);
    team = red_or_blue_team;
    
    existing_piece_objects++;

}

spy::spy(std::string identification, std::string red_or_blue_team)
{
    id = identification;
    team_check(red_or_blue_team);
    team = red_or_blue_team;
    
    existing_piece_objects++;

}

scout::scout(std::string identification, std::string red_or_blue_team)
{
    id = identification;
    team_check(red_or_blue_team);
    team = red_or_blue_team;
    
    existing_piece_objects++;

}

miner::miner(std::string identification, std::string red_or_blue_team)
{
    id = identification;
    team_check(red_or_blue_team);
    team = red_or_blue_team;
    
    existing_piece_objects++;

}

captain::captain(std::string identification, std::string red_or_blue_team)
{
    id = identification;
    team_check(red_or_blue_team);
    team = red_or_blue_team;
    

    existing_piece_objects++;

}

major::major(std::string identification, std::string red_or_blue_team)
{
    id = identification;
    team_check(red_or_blue_team);
    team = red_or_blue_team;
    
    existing_piece_objects++;

}

sergeant::sergeant(std::string identification, std::string red_or_blue_team)
{
    id = identification;
    team_check(red_or_blue_team);
    team = red_or_blue_team;
    
    existing_piece_objects++;

}

lieutenant::lieutenant(std::string identification, std::string red_or_blue_team)
{
    id = identification;
    team_check(red_or_blue_team);
    team = red_or_blue_team;
    
    existing_piece_objects++;

}

colonel::colonel(std::string identification, std::string red_or_blue_team)
{
    id = identification;
    team_check(red_or_blue_team);
    team = red_or_blue_team;
    
    existing_piece_objects++;

}

general::general(std::string identification, std::string red_or_blue_team)
{
    id = identification;
    team_check(red_or_blue_team);
    team = red_or_blue_team;
    
    existing_piece_objects++;

}

marshal::marshal(std::string identification, std::string red_or_blue_team)
{
    id = identification;
    team_check(red_or_blue_team);
    team = red_or_blue_team;
    
    existing_piece_objects++;

}
