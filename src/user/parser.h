#pragma once
#include <string>
#include <vector>

//Struct containing all neccesary parameters for parser
typedef struct CommandParameters {

	//Is input redirected
	bool redirectedInput;

	//Is output redirected
	bool redirectedOutput;
	
	//Switches location
	bool hasswitches;

	//String representing Command;
	std::string command;

	//String representing path of input stream
	std::string inputPath;

	//String representing path of output stream
	std::string outputPath;
	
	//Switch location path
	std::string switches;

	//Vector of parameters of command
	std::vector<std::string> parameters;

} CommandParameters;

//Parser construktor
typedef struct Parser {

private:
	//Parser error message
	std::string error_msg;

	//inside function for parsing of single command
	bool parse_command(std::string command, struct CommandParameters * commandPar);

public:
	//Setting Error to no error
	Parser() : error_msg("No error") {};

	//Function for returning error message
	std::string getErrorMessage();

	//Function for parsing commands
	bool parse_commands(std::string line, std::vector<struct CommandParameters> * commandsPar);

} Parser;