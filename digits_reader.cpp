#include "stdafx.h"

//#include "digits_reader.h"

#include<iostream>
using std::cin;
using std::cout;
using std::cerr;
using std::endl;
using std::streamsize;

#include<cctype>
using std::isdigit;

#include<limits>
using std::numeric_limits;

#include<stdexcept>
using std::exception;
using std::runtime_error;

#include<iomanip>
using std::setprecision;

#include<cmath>
using std::pow;

// gets a number from input
vector<int> get_number()
// reads a set of characters representing an integer number from input (e.g. 154)
// returns the number as a vector of single digits
// returns an empty vector if the operation fails (e.g. non digit characters, space)
{
	vector<int> number;
	while (cin.peek() != '\n' && !isspace(cin.peek())) {
		char c{ '?' };
		cin >> c;
		if (cin && isdigit(c))
			number.push_back(c - '0');
		else
			throw runtime_error("unable to read the number: expected a single digit.");
	}
	if (!cin)
		cin.clear();
	cin.ignore(numeric_limits<streamsize>::max(), '\n');
	return number;
}

// spells out a number
void read_digits(vector<int>& number)
// reads elements from a vector of single digits
// e.g. a vector containing 3 single digits 1, 2 and 3 is read one hundred and twenty-three
// handles a vector with a size ranging from one to twelve
{
	int number_size = number.size();

	// checks if the elements of number are valid single digits
	for (int i = 0; i < number_size; ++i)
		if (number[i] < 0 || number[i] > 9)
			throw runtime_error("unable to read number: expected a single digit.");

	// removes the unnecessary zeros from number
	while (!number.empty() && number[0] == 0 && number_size != 1) {
		number.erase(number.begin());
		--number_size;
	}

	// updates number_size
	number_size = number.size();

	// computes the double value
	double n{ 0 };
	for (int i = 0; i < number_size; ++i)
		n += number[i] * pow(10, (number_size - i - 1));

	int previous{ 0 }; // stores the previous computed integer value

	if (number_size > 12 || number.empty())
		cout << "only handles numbers with a number of single digits ranging from one to twelve." << endl;
	else
	{
		cout << setprecision(numeric_limits<double>::digits10) << n << " is ";

		while (number_size > 0) {
			vector<string> unit = { "" , " ten" , " hundred" , " thousand", " ten of thousands", " hundred of thousands", " million", " ten of millions", " hundreds of millions", " billion", "" , " tens" , " hundreds" , " thousands", " tens of thousands", " hundreds of thousands", " millions", " tens of millions", " hundreds of millions", " billions" };

			// removes the unnecessary zeros from number
			while (!number.empty() && number[0] == 0 && number_size != 1) {
				number.erase(number.begin());
				--number_size;
			}

			// computes a small part of number
			vector<int> part;
			if (number_size > 3) {
				int i = number_size / 3;
				if (number_size % 3 == 0)
					--i;
				part = vector<int>(number.begin(), number.begin() + number_size - 3 * i);
				number.erase(number.begin(), number.begin() + number_size - 3 * i);
			}
			else {
				part = number;
				number.clear();
			}

			// updates number_size
			number_size = number.size();

			int part_size = part.size();
			const int original_size = part.size();

			while (part_size > 0) {
				// computes the integer value
				int x{ 0 };
				for (int i = 0; i < part_size; ++i) {
					x += part[i] * int(pow(10, (part_size - i - 1)));
				}

				// computes the integer value stored in number
				int next{ 0 };
				for (int i = 0; i < number_size; ++i) {
					next += number[i] * int(pow(10, (number_size - i - 1)));
				}

				// deals with "and"
				if (part_size != original_size) {
					if ((has_name(x) && x != 0) ||
						(!has_name(x) && (part_size == 2)) ||
						(!(has_name(x) || (part_size == 2)) && (x - part[0] * pow(10, part_size - 1) == 0)))
						cout << " and ";
					else
						if (!(has_name(x) || (part_size == 2)) && !(x - part[0] * pow(10, part_size - 1) == 0))
							cout << " ";
				}
				else if (previous != 0) {
					if ((has_name(x) && x != 0 && next == 0) ||
						(!has_name(x) && (part_size == 2)) ||
						(!(has_name(x) || (part_size == 2)) && (x - part[0] * pow(10, part_size - 1) == 0)))
						cout << "and ";
				}

				// spells out numbers
				if (has_name(x)) {
					if (x != 0)
						cout << spell_number(x);
					else
						if (part_size == original_size && previous == 0)
							cout << spell_number(x);

					part_size = 0;
				}
				else if (part_size == 2) {
					cout << spell_number(part[0] * 10) << "-" << spell_number(part[1]);
					part_size = 0;
				}
				else {
					unsigned left_bound = unsigned(part_size - 1);
					unsigned right_bound = unsigned(part_size + 6 + 3);
					if (!(left_bound < unit.size() && right_bound < unit.size()))
						throw runtime_error("unable to display unit: unit not found.");

					cout << spell_number(part[0]) << ((part[0] > 1) ? unit[part_size + 6 + 3] : unit[part_size - 1]);
				}

				// saves x to previous
				if (x > 0 && x > previous)
					previous = x;

				// updates part_size
				if (part_size > 0) {
					part.erase(part.begin());
					--part_size;
				}

				// removes the unnecessary zeros from number
				while (!part.empty() && part[0] == 0 && part_size != 1) {
					part.erase(part.begin());
					--part_size;
				}
			}

			if (number_size > 0) {
				// computes the integer value stored in number
				int next{ 0 };
				for (int i = 0; i < number_size; ++i) {
					next += number[i] * int(pow(10, (number_size - i - 1)));
				}

				unsigned left_bound = unsigned(number_size);
				unsigned right_bound = unsigned(number_size + 7 + 3);
				if (!(left_bound < unit.size() && right_bound < unit.size()))
					throw runtime_error("unable to display unit: unit not found.");

				// displays unit
				cout << ((previous > 1) ? unit[number_size + 7 + 3] : unit[number_size]) << ((next == 0) ? "" : " ");
			}
		}
		// displays end point
		cout << "." << endl;
	}
}

// spells a number to words
string spell_number(const int& number)
// spells a number to a string word
// the argument is a positive integer and has a conventional name
// returns the spelled number as a string word
{
	if (number < 0)
		throw runtime_error("expected a positive integer.");

	switch (number) {
	case 0:
		return "zero";

	case 1:
		return "one";

	case 2:
		return "two";

	case 3:
		return "three";

	case 4:
		return "four";

	case 5:
		return "five";

	case 6:
		return "six";

	case 7:
		return "seven";

	case 8:
		return "eight";

	case 9:
		return "nine";

	case 10:
		return "ten";

	case 11:
		return "eleven";

	case 12:
		return "twelve";

	case 13:
		return "thirteen";

	case 14:
		return "fourteen";

	case 15:
		return "fifteen";

	case 16:
		return "sixteen";

	case 17:
		return "seventeen";

	case 18:
		return "eighteen";

	case 19:
		return "nineteen";

	case 20:
		return "twenty";

	case 30:
		return "thirty";

	case 40:
		return "forty";

	case 50:
		return "fifty";

	case 60:
		return "sixty";

	case 70:
		return "seventy";

	case 80:
		return "eighty";

	case 90:
		return "eighty";

	default:
		throw runtime_error("no conventional name available.");
	}
}

// checks if a number has a conventional name
bool has_name(const int& number)
// checks if a number has a conventional name
// the argument is a positive integer
// returns true if the number has a conventional name
// returns false otherwise
{
	if (number < 0)
		throw runtime_error("expected a positive integer.");

	switch (number) {
	case 0: case 1: case 2: case 3: case 4: case 5:
	case 6: case 7: case 8: case 9: case 10: case 11:
	case 12: case 13: case 14: case 15: case 16: case 17:
	case 18: case 19: case 20: case 30: case 40: case 50:
	case 60: case 70: case 80: case 90:
		return true;

	default:
		break;
	}

	return false;
}

int main()
try
{
	cout << "Please enter a number:";
	while (true) {
		cout << "\n> ";
		try {
			vector<int> number = get_number(); // reads a number from input
			int number_size = number.size();

			if (number_size > 0)
				read_digits(number);
			else
				cout << "invalid input. Please try again." << endl;
		}
		catch (runtime_error& e) {
			cerr << "Error: " << e.what() << endl;

			if (!cin)
				cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
		}

	}
	return 0;
}
catch (exception& e) {
	cerr << "Error: " << e.what() << endl;
	return 1;
}
catch (...) {
	cerr << "Error: unknown exception caught." << endl;
	return 2;
}