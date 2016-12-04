
#include "pipe.h"
#include "..\common\api.h"
#include <iostream>

/*
Pipe Constructor
*/
pipe::pipe()
{
	InitializeConditionVariable(&buffer_full);
	InitializeConditionVariable(&buffer_empty);
	InitializeCriticalSection(&crit_sec);
	closed_in = false;
	closed_out = false;
}

/*
Function for writting multiple chars
@string array of chars
@len length of array
@written number of written chars
*/
bool pipe::write(char * string, size_t len, size_t * written)
{
	*written = 0;
	for (int i = 0; i < len; i++) {
		if (!write(string[i])) {
			*written = i;
			//SetLastError(ERR_IO_PIPE_READCLOSED);
			return false;
		}
	}
	*written = len;
	return true;
}
/*
Function for writting single character
@c character to write
*/
bool pipe::write(char c)
{
	//If pipe has closed output
	if (closed_out) {
		return false;
	}

	while (queue.size() >= MAX_SIZE && !closed_out) {
		SleepConditionVariableCS(&buffer_full, &crit_sec, INFINITE);
	}

	if (closed_out) {
		return false;
	}

	EnterCriticalSection(&crit_sec);
	queue.push(c);
	LeaveCriticalSection(&crit_sec);
	WakeConditionVariable(&buffer_empty);
	return true;
}

/*
Function for reading from file
@count number of chars to be read from file
@str char array to put read chars into
@r position in array
@return if all read
*/
bool pipe::read(size_t count, char *str, size_t *r)
{
	int pos = 0;
	while (pos < count) {
		char c = read();
		str[pos] = c;
		if (c == EOF) {
			break;
		}
		pos++;
	}
	*r = pos;
	return true;
}

/*
Function to determine, if there can be EOF
@available 1 if closed input, queue size otherwise
*/
bool pipe::peek(size_t * available)
{
	//if input is closed, return 1, otherwise return size of queue
	if (closed_in) {
		*available = 1; //EOF is available
	}
	else {
		*available = queue.size();
	}
	return true;
}

/*
Function for reading a char
@return read char
*/
char pipe::read()
{
	if (closed_in && queue.size() < 1) {
		return EOF;
	}
	while (queue.size() < 1 && !closed_in) {
		SleepConditionVariableCS(&buffer_empty, &crit_sec, INFINITE);
	}
	if (closed_in && queue.size() < 1) {
		return EOF;
	}
	EnterCriticalSection(&crit_sec);
	char ret = queue.front();
	queue.pop();
	LeaveCriticalSection(&crit_sec);
	WakeConditionVariable(&buffer_full);
	return ret;
}

/*
Function for closing pipe output
*/
void pipe::close_read()
{
	closed_out = true;
	WakeConditionVariable(&buffer_full);
}

/*
Function for closing pipe input
*/
void pipe::close_write()
{
	closed_in = true;
	WakeConditionVariable(&buffer_empty);
}

/*
Pipe Destructor
*/
pipe::~pipe()
{
	DeleteCriticalSection(&crit_sec);
	WakeAllConditionVariable(&buffer_empty);
	WakeAllConditionVariable(&buffer_full);
}