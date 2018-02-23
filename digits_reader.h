#pragma once

#include<vector>
using std::vector;

#include<string>
using std::string;

// gets a number from input
vector<int> get_number();

// spells out a number
void read_digits(vector<int>&);

// spells a number to words
string spell_number(const int&);

// checks if a number has a conventional name
bool has_name(const int&);