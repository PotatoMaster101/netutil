///////////////////////////////////////////////////////////////////////////////
// Helper functions for IO.
// https://github.com/PotatoMaster101/netutil
///////////////////////////////////////////////////////////////////////////////

#ifndef IOUTIL_H
#define IOUTIL_H
#include <stdio.h>
#include <stdbool.h>

// Determines whether the specified file exists.
//
// PARAMS:
// f - the file to check
//
// RETURN:
// 1 if the file exists, 0 otherwise.
_Bool file_exist(const char *f);

// Returns the file size in bytes.
//
// PARAMS:
// f - the file to retrieve the size
//
// RETURN:
// The file size in bytes. On error, -1 will be returned.
long file_size(const char *f);

// Wipes the specified file.
//
// PARAMS:
// f - the file to wipe
void file_wipe(const char *f);

// Promps user for an integer between the given lower and upper bounds.
//
// PARAMS:
// msg - the message to display for the user
// low - the lower bound to use
// up  - the upper bound to use
//
// RETURN:
// An integer between the lower and upper bound.
long prompt_int(const char *msg, long low, long up);

// Prompts user for a number between the given lower and upper bounds.
//
// PARAMS:
// msg - the message to display for the user
// low - the lower bound to use
// up  - the upper bound to use
//
// RETURN:
// A number between the lower and upper bound.
double prompt_double(const char *msg, double low, double up);

// Clears the stdin buffer.
void clear_stdin();

#endif
