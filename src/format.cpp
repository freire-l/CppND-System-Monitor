#include <string>
#include <iostream>
#include <iomanip>

#include "format.h"

using std::string;

string Format::ElapsedTime(long time) {

    long hours, minutes, seconds;
    hours = time/3600;
    time = time%3600;
    minutes = time/60;
    time = time%60;
    seconds = time;
    

    std::ostringstream stream;
    //From Knowledge section
    stream << std::setw(2) << std::setfill('0') << hours << ":" 
     << std::setw(2) << std::setfill('0') << minutes << ":"
     << std::setw(2) << std::setfill('0') << seconds;
    return stream.str();
}
