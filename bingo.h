#ifndef BINGO_H
#define BINGO_H


#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include <stdbool.h>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <iostream>
#include <random>
#include <vector>


	class Carton
	{ 
		public:
			
			int print_version();
			///This method prints the version of the program.
			int print_help();
			///This method prints how to use the program.		
			//const char* generate_str(const char* ch);
			const char* generate_str(const char* str);
			///This method returns a const char* that have a random number.
			///@params receives a const char*.
			const char* generate_str(const char* str, const char* symbols_filename);
			///This method returns a const char* that have a random number.
			///@params receives a const char*, also a const char* that contains the symbols to fill the card.
			bool generate_file(std::ifstream& source, const char* temp_filename, std::ofstream& index, size_t cards, bool symbols = false, const char* symbols_filename = NULL);
			///This method returns true if it can process a file, else it returns a false.
			///@params it receives a file source from which we will extract the information of the cardboards. The second 
			///is a const char* that will be open and will be transform into the target.
			bool generate_filename(const char* cards, const char* source_filename, bool symbols = false, const char* symbols_filename = NULL);
			///This method returns true if it opens source and creates the name of our target file.
			///It calls generate_file with target and returns it's result.
			///@params receives a const char* cards that it's a number that limits how many times this method must be done and
			///receives a const char* source_filename that is the file we want to open to start creating target files.
			int analyze_params(int argc, char* argv[]);
			///This method analyze the params and process which method should be executed first. It returns an int.
			///@params it receives an int argc and a char* argv[] that will be the the commands to call a method.
			bool replace(const char* symbols_filename, const char* source_filename);
			///
			bool call(const char* index_filename = NULL, const char* gametype = NULL);
			///
			bool call_symbols(const char* symbols_filename);
			///
			void put_index(std::vector<std::string>& vec, std::ofstream& index, int count);
			///Creates a file .idx that contains all the cardboards that were created. Returns 0 if succesful, and 1 if not.
			///@params the params are a std vector of std strings that contains the cardboard and all the numbers that are in it
			/// a file index that will contain the vector and the count which will be the cardboard number.(e.g. if count is 1, then the 
			///cardboard will be cardboard1 and that will be the name that appears on the index file)
			friend std::string operator+(std::string& string, int num);
			///This method alouds the operator + to add an integer to a std string. It returns the concatenated string.
			///@params: a std string which will be concatenated to an integer, an integer that will be concatenated to a string.
			bool admin_gametype(std::ifstream& index, std::string& gametype, std::vector<int>& vec_call);
			///This method administrates the gametypes, when the user wants to play a specific gametype this method will call the method 
			///that controls that game. Returns 0 if succesful and 1 if not.
			///@params: a file index that has all the cardboards and numbers in it, a std string which will be the name of the gametype,
			///a std vector of ints that has all the ints that are called by the method call.
			bool search_int(int find, std::vector<int>& call_vector);
			///Searches if an integer exists in a std vector of integers. Returns true if succesful and false if not.
			///@params: an integer valuer to search, a std vector of integers.
			bool blackout(std::ifstream& index, std::vector<int>& vec_call);
			/// This method controls the gametype blackout in which all the cardboard must have all the numbers called in order to win
			/// when that happens the cardboard that wins will be anounce and the method will finish. It returns 0 if succesful and 1if not.
			///@params: a file index that has all the cardboards and numbers in them, and a std vector of integers that has all the numbers
			///called by call().
			bool four_corners(std::ifstream& index, std::vector<int>& vec);
			///This method controls the gametype four-corners in which all the corners of the cardboard must have all the numbers called in order to win
			/// when that happens the cardboard that wins will be anounce and the method will finish. It returns 0 if succesful, and 1 if not.
			///@params: a file index that has all the cardboards and numbers in them, and a std vector of integers that has all the numbers
			///called by call().
			bool roving_L(std::ifstream& index, std::vector<int>& vec);
			///This method controls the gametype roving-l in which the cardboard  called must form an L in order to win, it doesn't matter
			///if the L is in the "correct" side, as long as the numers form an L the user wins.
			/// when that happens the cardboard that wins will be anounce and the method will finish.It returns 0 if succesful, and 1 if not.
			///@params: a file index that has all the cardboards and numbers in them, and a std vector of integers that has all the numbers
			///called by call().
			bool straight_line(std::ifstream& index, std::vector<int>& vec);
			///This method controls the gametype straight-line in which you win by having the numbers that 
			///form a straight line in the cardboard. This method calls Vertical_Line, and Horizontal_Line. 
			/// when that happens the cardboard that wins will be anounce and the method will finish.It returns 0 if succesful, and 1 if not.
			///@params: a file index that has all the cardboards and numbers in them, and a std vector of integers that has all the numbers
			///called by call().
			bool Vertical_Line(std::ifstream& index, std::vector<int>& vec);
			////Checks if the numbers in any vertical line in the cardboard have been called, returns true and calls announce_winner.
			///@params receives an index that has all the cardboards that have been created, and a std::vector of ints with all the numers
			///that have been call.
			bool Horizontal_Line(std::ifstream& index, std::vector<int>& vec);
			////Checks if the numbers in any horizontal line in the cardboard have been called, returns true and calls announce_winner.
			///@params receives an index that has all the cardboards that have been created, and a std::vector of ints with all the numers
			///that have been call.
			bool diagonal(std::ifstream& index, std::vector<int>& vec);
			////Checks if the numbers in any diagonal line in the cardboard have been called, returns true and calls announce_winner.
			///@params receives an index that has all the cardboards that have been created, and a std::vector of ints with all the numers
			///that have been call.
			void announce_winner(std::string& cardboard_check);
			///prints which cardboard won the game.
			///@params a std string with the name of the cardboard that won.
	};
	
	std::string operator+(std::string& string, int num);
	///this method alouds the operator + to concatenate an integer to a std string.
	///@params: a std string which will be concatenated to an integer, an integer that will be concatenated to a string.

#endif //BINGO_H
