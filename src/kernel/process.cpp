#include "process.h"
#include "kernel.h"
#include <iostream>
#include <thread>
#include <Windows.h>

//mutex for process table
std::mutex process_table_mtx; 
//process table with max 1024 processes
PCB * process_table[PROCESS_TABLE_SIZE] = { nullptr };

/**
Function for handling process

@param regs - address of context regiters for handling process
*/
void HandleProcess(CONTEXT & regs) {
	switch (Get_AL((__int16)regs.Rax)) {
	case scCreateProcess:
		regs.Rax = (decltype(regs.Rax))createProcess((command_params *)regs.Rcx);
		Set_Error(regs.Rax == -1, regs);
		break;
	}
}
/**
Function for running process

@param func - function to handle
@param pid - process ID
@param argc - number of arguments
@param argv - arguments
@param switches - if there is switch from folder to another
*/
void runProcess(TEntryPoint func, int pid, int argc, char ** argv, char * switches) {

	CONTEXT regs;
	//Stdin of process
	regs.R8 = (decltype(regs.R8))process_table[pid]->IO_decriptors[0];
	//Stdout of process
	regs.R9 = (decltype(regs.R9))process_table[pid]->IO_decriptors[1];
	//Stderr of process
	regs.R10 = (decltype(regs.R10))process_table[pid]->IO_decriptors[2];
	//Folder to switch
	regs.R11 = (decltype(regs.R11))switches;
	//Process name
	regs.Rax = (decltype(regs.Rax))process_table[pid]->name;
	//Process ID
	regs.Rbx = (decltype(regs.Rbx))pid;
	//Number of arguments
	regs.Rcx = (decltype(regs.Rcx))argc;
	//Arguments
	regs.Rdx = (decltype(regs.Rdx))argv;

	size_t ret = func(regs);
	//TODO delat neco s navratovou hodnotou
	//TODO uklid:
	//ZAPSAT EOF NA STDOUT

	//Cleaning up process from PCB when done
	std::lock_guard<std::mutex> lock(process_table_mtx);
	delete process_table[pid];
	process_table[pid] = nullptr;
}

/**
Function for creating process
@param par - parameters for creating processes, structure from parser
@return Process ID
*/
int createProcess(command_params * par)
{
	int pid = -1;
	{
		//if table not full, get first open PID and slot in PCB
		std::lock_guard<std::mutex> lock(process_table_mtx);
		for (int i = 0; i < PROCESS_TABLE_SIZE; i++) {
			if (process_table[i] == nullptr) {
				process_table[i] = new PCB();
				pid = i;
				break;
			}
		}
	}

	//If wrong PID (table is full), set error and return -1
	if (pid == -1) {
		SetLastError(CREATE_PROCESS_ERROR);
		return pid;
	}

	//Fill PCB information
	process_table[pid]->pid = pid;
	process_table[pid]->IO_decriptors.push_back(par->STDIN);
	process_table[pid]->IO_decriptors.push_back(par->STDOUT);
	process_table[pid]->IO_decriptors.push_back(par->STDERR);
	process_table[pid]->name = par->name;
	
	/*
	TODO
	newProc.current_dir = ...
	newProc.root_dir = ...
	*/

	//Get right function of process
	TEntryPoint func = (TEntryPoint)GetProcAddress(User_Programs, par->name);
	if (!func) {
		//If function doesn't exist, clear PCB slot
		std::lock_guard<std::mutex> lock(process_table_mtx);
		delete process_table[pid];
		process_table[pid] = nullptr;
		SetLastError(CREATE_PROCESS_ERROR);
		return pid;
	}

	//New process (thread)
	std::thread t(runProcess, func, pid, par->argc, par->argv, par->switches);

	//If process is dependent on other process, join if not, detach (independent run)
	if (par->waitForProcess) {
		t.join();
	}
	else {
		t.detach();
	}
	return pid;
}