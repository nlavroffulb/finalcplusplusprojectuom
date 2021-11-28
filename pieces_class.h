#include<iostream>
#include <ostream>
#include <iomanip>
#include<string>
#include<sstream>
#include<vector>

//this header file contains the pieces class declarations as well its 11 derived classes declarations for each piece in Stratego.
//where possible functions and their arguments are made const to protect member data and function arguments. additionally where possible,
//arguments are passed by reference either for void functions or to for compilation speed.
class pieces {
protected:
    static int existing_piece_objects;//static variable to keep track of the number of pieces objects that exist.
public:
    static int get_number_of_piece_objects(){ return existing_piece_objects; }//static 'get' function for the above described static variable.

    //virtual function and destructor declarations
    virtual ~pieces() {};
    virtual int rank()const=0; 
    virtual std::string special_interaction()const =0;
    virtual std::string get_type()const =0;
    virtual std::string get_id()const =0;
    virtual std::string get_team()const = 0;
    void team_check(std::string& team_input)const;

    static void valid_piece_name(std::string& type_name);


};

//the derived class declarations are all quite similar in that they consist of id and team private members and then definitions of 
//the pieces virtual classes as well as destructor and constructor in the public part.
//the flag class is the only exception as it contains two private static integer variables which are to keep track of
//the flags of each team.
//these variables are used by static 'get' functions in the public part of the class.

class flag : public pieces {
private:
    std::string id{ "" };
    std::string team{ "" };
    static int red_flag;//static variables
    static int blue_flag;

public:


    flag() = default;
    flag(std::string identification, std::string red_or_blue_team);

    //declaration of all the virtual functions
    std::string get_type() const { return "Flag"; }
    int rank() const { return 0; }
    std::string special_interaction()const { return get_type(); }
    std::string get_id() const { return id; }
    std::string get_team() const { return team; }


    //static function to get flag numbers in total and per team.
    static int get_flag_num() { return red_flag+blue_flag; };
    static int get_flag(std::string team) { return (team == "Red") ? red_flag : blue_flag; }

    ~flag() { (team == "Red")?--red_flag:--blue_flag; std::cout << "Flag destructor called." << std::endl; }
};
class bomb : public pieces {
private:
    std::string id{ "" };
    std::string team{ "" };


public:
    bomb() = default;
    bomb(std::string identification, std::string red_or_blue_team);
    std::string get_type()const { return "Bomb"; }
    int rank() const { return 11; }
    std::string special_interaction()const { return "bomb-miner"; }
    std::string get_id()const { return id; }
    std::string get_team()const { return team; }

    ~bomb() { std::cout << "Bomb destructor called." << std::endl; }
};
class spy : public pieces {
private:
    std::string team{ "" };
    std::string id{ "" };

public:
    spy() = default;
    spy(std::string identification, std::string red_or_blue_team);
    std::string get_type()const { return "Spy"; }
    int rank()const { return 1; }
    std::string special_interaction() const { return "spy-marshal"; }
    std::string get_id()const { return id; }
    std::string get_team()const { return team; }

    ~spy() { std::cout << "Spy destructor called." << std::endl; }
};
class scout :public pieces {
private:
    std::string id{ "" };
    std::string team{ "" };

public:
    scout() = default;
    scout(std::string identification, std::string red_or_blue_team);
    std::string get_type()const { return "Scout"; }
    int rank() const { return 2; }
    std::string special_interaction()const { return get_type(); }
    std::string get_id() const { return id; }
    std::string get_team() const { return team; }

    ~scout() { std::cout << "Scout destructor called." << std::endl; }
};
class miner : public pieces {
private:
    std::string team{ "" };
    std::string id{ "" };

public:
    miner() = default;
    miner(std::string identification, std::string red_or_blue_team);
    std::string get_type()const { return "Miner"; }
    int rank()const { return 3; }
    std::string special_interaction()const { return "bomb-miner"; }
    std::string get_id()const { return id; }
    std::string get_team()const { return team; }

    ~miner() { std::cout << "Miner destructor called." << std::endl; }

};
class sergeant : public pieces {
private:
    std::string id{ "" };
    std::string team{ "" };

public:
    sergeant() = default;
    sergeant(std::string identification, std::string red_or_blue_team);
    std::string get_type() const { return "Sergeant"; }
    int rank() const { return 4; }
    std::string special_interaction() const { return get_type(); }
    std::string get_id() const { return id; }
    std::string get_team() const { return team; }

    ~sergeant() { std::cout << "Sergeant destructor called." << std::endl; }

};
class lieutenant : public pieces {
private:
    std::string id{ "" };
    std::string team{ "" };

public:
    lieutenant() = default;
    lieutenant(std::string identification, std::string red_or_blue_team);
    std::string get_type() const { return "Lieutenant"; }
    int rank() const { return 5; }
    std::string special_interaction()const { return get_type(); }
    std::string get_id()const { return id; }
    std::string get_team()const { return team; }

    ~lieutenant() { std::cout << "Lieutenant destructor called." << std::endl; }

};
class captain : public pieces {
private:
    std::string id{ "" };
    std::string team{ "" };

public:
    captain() = default;
    captain(std::string identification, std::string red_or_blue_team);
    std::string get_type()const { return "Captain"; }
    int rank() const { return 6; }
    std::string special_interaction()const { return get_type(); }
    std::string get_id() const { return id; }
    std::string get_team() const { return team; }


    ~captain() { std::cout << "Captain destructor called." << std::endl; }

};
class major : public pieces {
private:
    std::string id{ "" };
    std::string team{ "" };

public:
    major() = default;
    major(std::string identification, std::string red_or_blue_team);
    std::string get_type()const { return "Major"; }
    int rank()const { return 7; }
    std::string special_interaction()const { return get_type(); }
    std::string get_id()const { return id; }
    std::string get_team()const { return team; }

    ~major() { std::cout << "Major destructor called." << std::endl; }

};
class colonel : public pieces {
private:
    std::string id{ "" };
    std::string team{ "" };

public:
    colonel() = default;
    colonel(std::string identification, std::string red_or_blue_team);
    std::string get_type()const { return "Colonel"; }
    int rank()const { return 8; }
    std::string special_interaction() const { return get_type(); }
    std::string get_id()const { return id; }
    std::string get_team()const { return team; }

    ~colonel() { std::cout << "Colonel destructor called." << std::endl; }

};
class general : public pieces {
private:
    std::string id{ "" };
    std::string team{ "" };

public:
    general() = default;
    general(std::string identification, std::string red_or_blue_team);
    std::string get_type()const { return "General"; }
    int rank()const { return 9; }
    std::string special_interaction()const { return get_type(); }
    std::string get_id()const { return id; }
    std::string get_team()const { return team; }

    ~general() { std::cout << "General destructor called." << std::endl; }

};
class marshal : public pieces {
private:
    std::string id{ "" };
    std::string team{ "" };

public:
    marshal() = default;
    marshal(std::string identification, std::string red_or_blue_team);
    std::string get_type()const { return "Marshal"; }
    int rank()const { return 10; }
    std::string special_interaction()const { return "spy-marshal"; }
    std::string get_id()const { return id; }
    std::string get_team()const { return team; }

    ~marshal() { std::cout << "Marshal destructor called" << std::endl; }

};
#pragma once
