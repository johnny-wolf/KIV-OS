#include <set>
#include <regex>

#include "parser.h"

//Set of valid commands, other will result in error
const std::set<std::string> SET_valid_commands = { "dir", "echo", "freq", "md", "ps", "rd", "rgen", "sort", "type", "wc"};

//regex of valid letters of command
const std::regex RGX_nab("[^a-zA-z]");

//regex of valid letters in line with commands
const std::regex RGX_command_parameters("<|>|\"|/[a-zA-z]+|[^ \t\"]+");

/**
Function for returning error message

@return error message of parser. default set on "No Error"
*/
std::string Parser::getErrorMessage(){
	return error_msg;
}

/**
Function for parsing commands

@param line - line to be parsed
@param commands - vector of parameters of commands in line
@return true if parsed correctly, else false
*/
bool Parser::parse_commands(std::string line, std::vector<struct CommandParameters> * commandsPar){
	
	//vector of commands in line
	std::vector<std::string> commandsString;
	//Current position on line, if it contains " it has default value of its position, otherwise -1
	size_t currentPos = line.find('\"');
	//Last position on line, used when quotes on line (cannot use oldPos for that case)
	size_t lastPos = 0;
	//Old position on line
	size_t oldPos = 0;
	//Position of left quote
	size_t posLeftQuote = 0;
	//Position of right quote
	size_t posRightQuote = 0;
	//Position of pipe
	size_t posPipe = 0;

	//If there are no " in the line
	if (currentPos == std::string::npos){
		//do while line contains |
		while ((currentPos = line.find('|', oldPos)) != std::string::npos){
			//push command to the vector
			commandsString.push_back(line.substr(oldPos, currentPos - oldPos));
			oldPos = currentPos + 1;
		}
		commandsString.push_back(line.substr(oldPos, std::string::npos));
	}
	//If line contains "
	else {
		//do while line contains |
		while (posPipe != std::string::npos) {
			//find position of next two quotes and pipe
			posLeftQuote = line.find('\"', oldPos);
			posRightQuote = line.find('\"', posLeftQuote + 1);
			posPipe = line.find('|', oldPos);

			//If there is no right quote
			if (posLeftQuote != std::string::npos && posRightQuote == std::string::npos){
				error_msg = "Missing right \" ";
				return false;
			}

			//if | is between " ignore it and change position to end of quotes
			if (posPipe < posRightQuote && posPipe > posLeftQuote) {
				oldPos = posRightQuote;
			}
			else {
				//if there are two | next to each other
				if (posPipe - lastPos <= 1) {
					error_msg = "Multiple | symbols";
					return false;
				}
				//else push command to vector, change position after last |
				commandsString.push_back(line.substr(lastPos, posPipe - lastPos));
				oldPos = posPipe + 1;
				lastPos = oldPos;
			}
		}
	}
	for (std::string com : commandsString) {
		CommandParameters par;
		//if command was parsed wrongly
		if (!parse_command(com, &par)) {
			return false;
		}
		//push command parammeters into commands
		else {
			commandsPar->push_back(par);
		}
	}
	return true;
}

/**
Inside function for parsing single command

@param command - line to be parsed
@param commandPar - vector of parameters of commands in line
@return true if parsed correctly, else false
*/
bool Parser::parse_command(std::string command, struct CommandParameters * commandPar) {

	//Initialize command parameters
	*commandPar = { false, false, false, "", "", "", "" };
	// < and waiting for path
	bool inChangePending = false;
	// > and waiting for path
	bool outChangePending = false;
	//Position
	int pos = 0;

	//skip spaces on start
	while (isspace(command[pos]) != 0)
		pos++;
	command = command.substr(pos);

	//Separating command word in the command line
	std::smatch matchResult;
	std::regex_search(command, matchResult, RGX_nab);
	std::string commandWord = command.substr(0, matchResult.position());
	command = command.substr(matchResult.position(), std::string::npos);

	//If command word isn't in command set
	if (SET_valid_commands.count(commandWord) < 1) {
		error_msg = "Unkown command \"" + commandWord + "\"";
		return false;
	}
	commandPar->command = commandWord;

	while (std::regex_search(command, matchResult, RGX_command_parameters)) {
		//Get next match of ", \, <, > and substring it from command until this match
		std::string str = matchResult.str();
		command = matchResult.suffix();

		if (str == "\"") {
			size_t length = command.find('"');
			std::string temp = command.substr(0, length);
			//if there is input redirection without path
			if (inChangePending) {
				commandPar->inputPath;
				inChangePending = false;
			}
			//if there is output redirection without path
			else if (outChangePending) {
				commandPar->outputPath;
				outChangePending = false;
			}
			else {
				commandPar->parameters.push_back(temp);
			}
			//substring after end of path and continue from there
			command = command.substr(length + 1, std::string::npos);
		}
		else if (str == ">") {
			//If already redirected output
			if (commandPar->redirectedOutput) {
				error_msg = "Attempt for second output redirection";
				return false;
			}
			commandPar->redirectedOutput = true;
			if (inChangePending || outChangePending) {
				error_msg = "Two redirect symbols in row";
					return false;
			}
			outChangePending = true;
		}
		else if (str == "<") {
			//If already redirected output
			if (commandPar->redirectedInput) {
				error_msg = "Attempt for second input redirection";
				return false;
			}
			commandPar->redirectedInput = true;
			if (inChangePending || outChangePending) {
				error_msg = "Two redirect symbols in row";
					return false;
			}
			inChangePending = true;
		}
		//if there is possible path
		else if (str[0] == '/') {
			//If there is redirection symbol after /
			if (inChangePending || outChangePending) {
				error_msg = "Symbol / is followed by redirection symbol";
				return false;
			}
			//If there is no symbol after /
			if (str.length() == 1) {
				error_msg = "Symbol / does not represent path";
				return false;
			}
			// If there are two /
			if (str[1] == '/') {
				error_msg = "Symbol / followed by another /";
				return false;
			}
			commandPar->hasswitches = true;
			commandPar->switches = commandPar->switches + str.substr(1, std::string::npos);

		}
		else {
			//If there is input change pending
			if (inChangePending) {
				commandPar->inputPath = str;
				inChangePending = false;
			}
			//if there is output change pending
			else if (outChangePending) {
				commandPar->outputPath = str;
				outChangePending = false;
			}
			//if there is parameter push it into command parameters
			else {
				commandPar->parameters.push_back(str);
			}
		}
		if (inChangePending || outChangePending) {
			error_msg = "Command has blank redirection at the end";
			return false;
		}
	}
	return true;
}