#include "windows_header_class.h"
#include<iostream>
//this cpp file contains several functions which are used to format the output to the console by 
//controlling output colour, cursor position and clearing output on the console.

//functions are well described by their name.

void windows_header::set_colour(const int& unicode_colour)
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), unicode_colour);
}

void windows_header::set_cursor_position(const SHORT& x, const SHORT& y) {
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), { x,y });
}

COORD windows_header::get_current_cursor_coordinates()//COORD data type is specific to windows header. it stores coordinates.
{
    CONSOLE_SCREEN_BUFFER_INFO info;
    if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info))
    {
        return info.dwCursorPosition;
    }
    else {
        std::cout << "Error. Returned the origin." << std::endl;
        return { 0,0 };
    }
}

//this is a fairly rudimentary way of clearing the console. starting at some position
//we enter 1000 space characters and do this for 50 lines. usually enough for the needs of this program.
void windows_header::clear_50_lines(const SHORT& start_x, const SHORT& start_y) {
    set_cursor_position(start_x, start_y);
    for (int i{ 1 }; i <= 50; i++) {
        std::cout << std::string(1000, ' ');
        set_cursor_position(0, 45 + i);
    }
    set_cursor_position(start_x, start_y);

}

