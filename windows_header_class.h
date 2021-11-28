#pragma once
#include<windows.h>

//class declaration for the parts of the windows.h file that will be used regularly.
//these are to set cursor position, get the cursor position, set colour and clear output in the console.

class windows_header {
public:
    static void set_colour(const int& unicode_colour);
    static void set_cursor_position(const SHORT& x, const SHORT& y);
    static COORD get_current_cursor_coordinates();
    static void clear_50_lines(const SHORT& start_x, const SHORT& start_y);
};
