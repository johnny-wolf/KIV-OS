#include <iostream>

#include <string>
#include "rtl.h"
#include "shell.h"
#include "parser.h"


void parseLine(std::string line, Parser p) {
	std::vector<CommandParameters> commands;
	//if commands aren't "dir", "echo", "freq", "md", "ps", "rd", "rgen", "sort", "type", "wc" it is error
	if (!p.parse_commands(line, &commands)) {
		std::cout << p.getErrorMessage() << std::endl << std::endl;;
	}
	else {
		std::cout << "Parsing OK\n";
		for (size_t i = 0; i < commands.size(); i++) {
			std::string commandString = "command: " + commands[i].command + " params: ";
			std::cout << commandString;
			for (size_t j = 0; j < commands[i].parameters.size(); j++) {
				std::cout << commands[i].parameters.at(j) + " ";
			}
			std::cout << "\n";
		}
	}
}

size_t __stdcall shell(const CONTEXT &regs) {
	//THandle stdin = Create_File("CONOUT$", FILE_SHARE_WRITE);	//nahradte systemovym resenim, zatim viz Console u CreateFile na MSDN
	std::string s;
	Parser parser;

	//parser
	Parser p;

	//Parser testing code

	std::string test = "wc -a -b  | md -c | ps p1 p2 p3 /n |  wc a b c d ";
	std::vector<CommandParameters> commands;
	//if commands aren't "dir", "echo", "freq", "md", "ps", "rd", "rgen", "sort", "type", "wc" it is error
	parseLine(test, p);
	//Shell read
	do
	{
		std::getline(std::cin, s);
		std::cout << "You typed: " + s + ", parsing command\n";
		parseLine(s, p);

		//parser.parse_commands(s, nullptr);
		//const char* hello = "Hello world!\n";
		//size_t written;
		//Write_File(stdin, hello, strlen(hello), written);
		//Close_File(stdin);
		//return 0;

	} while (s != "exit");

	return 0;
}