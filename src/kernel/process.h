#pragma once
#include <string>
#include <vector>
#include <mutex>
//#include "filesystem.h"
#include "../common/api.h"

//Table size, actual 2048, can be changed
const size_t PROCESS_TABLE_SIZE = 1024;
//
const size_t CREATE_PROCESS_ERROR = 200;
const size_t CREATE_PROCESS_OK = 100;

void HandleProcess(CONTEXT &regs);

typedef struct process_control_block {

	//Process id
	unsigned int pid;
	//Process name
	const char * name;
	//Pointers to nodes for current directory and root directory
	//node *current_dir, *root_dir;//mozna node asi
	//I/O descriptors
	std::vector<FDHandle> IO_decriptors; //tabulka souboru daneho procesu index by mel byt file descriptor
										 //0 = stdin, 1 = stdout, 2 = stderr

} PCB;//kazdej proces bude mit tuto strukturu

typedef struct create_process_params {

	//Handles for in, out and error
	FDHandle STDOUT, STDIN, STDERR;
	//Nodes
	//node  *current_node, *root_node; //?
	//If switched directory
	char * switches;
	//parameters
	char ** argv;
	//number of parameters
	int argc;
	//name
	const char * name;
	//boolean if process have to wait for another one to finish
	bool waitForProcess;

} command_params;


int createProcess(command_params * par);

extern PCB *process_table[1024];
