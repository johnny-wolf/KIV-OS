#include "shell.h"
#include <iostream>
#include <string>
#include "rtl.h"


size_t __stdcall shell(const CONTEXT &regs) {
	//THandle stdin = Create_File("CONOUT$", FILE_SHARE_WRITE);	//nahradte systemovym resenim, zatim viz Console u CreateFile na MSDN
	std::string s;

	do
	{
		std::getline(std::cin, s);

		//const char* hello = "Hello world!\n";
		//size_t written;
		//Write_File(stdin, hello, strlen(hello), written);
		//Close_File(stdin);
		//return 0;

	} while (s != "exit");

	return 0;
}