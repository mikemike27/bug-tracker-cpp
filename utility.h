#ifndef UTILITY
#define UTILITY

#include <chrono>
#include <ctime>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include "splashkit.h"

using namespace std;
using std::string;

void create_line_break();
string read_input_range(int start, int end);
vector<string> split(const string& line, char delimiter);
string get_create_time();

#endif