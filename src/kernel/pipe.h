#pragma once

#include <queue>
#include <Windows.h>

typedef struct pipe {

private:
	//Pipe size
	static const int MAX_SIZE = 2048;
	//Bool if pipe closed input
	bool closed_in;
	//Bool if pipe closed output
	bool closed_out;
	//Queue
	std::queue<char> queue;
	//Critical section of pipe
	CRITICAL_SECTION crit_sec;
	//Condition variables of buffer - empty and full
	CONDITION_VARIABLE buffer_full, buffer_empty;

	bool write(char c);
	char read();

public:

	pipe();
	bool write(char * s, size_t len, size_t * written);
	bool pipe::read(size_t count, char *str, size_t * read);
	bool pipe::peek(size_t * available);
	void close_read();
	void close_write();
	~pipe();

} pipe;
