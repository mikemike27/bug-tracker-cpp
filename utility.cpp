#include "utility.h"

void create_line_break(){

    write_line("-----------------------------------------------------------------------");

}

string read_input_range(int start, int end){

    string input = read_line();
    
    if(!is_integer(input) || (std::stoi(input) < start && std::stoi(input) > end)){
        
        write_line("Invalid option.");
        write("PLease select your option: ");
        input = read_line();

    }

    return input;

}

//split the csv file with delimiter ','
vector<string> split(const string& line, char delimiter){

    vector<string> tokens;

    std::stringstream ss(line);
    string token;

    while(getline(ss, token, delimiter)){
        //write_line(token);
        tokens.push_back(token);
    }

    return tokens;

}

//create the time status for eg tickets/comment
std::string get_create_time(){

    //get the current system time
    auto now = std::chrono::system_clock::now();

    //convert the system time to a time point in time_t
    std::time_t currentTime = std::chrono::system_clock::to_time_t(now);

    //convert the time_t value to a local time struct
    std::tm* localTime = std::localtime(&currentTime);

    //extract date and time components from the local time struct
    int year = localTime->tm_year + 1900; //since 1900
    int month = localTime->tm_mon + 1; //start from 0 so add 1
    int day = localTime->tm_mday; //day of the month
    int hour = localTime->tm_hour; //24hr format
    int minute = localTime->tm_min;
    int second = localTime->tm_sec;

    return std::to_string(hour)+":"+std::to_string(minute)+" "+std::to_string(day)+"/"+std::to_string(month)+"/"+std::to_string(year);

}