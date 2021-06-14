/**
 * * * * * * * * * * * * * * * * * * *
 * Brennan Couturier
 * * * * * * * * * * * * * * * * * * *
 * bcsh_constants.h
 * * * * * * * * * * * * * * * * * * *
 * All the constants I will use are here
 * * * * * * * * * * * * * * * * * * *
 */

#define INITIAL_TOKEN_BUFFER 25

#define MAX_TOKENS 100

#define INITIAL_PATH_LENGTH 100

#define NUM_INTERNAL_COMMANDS 4

extern char* internal_command_names[];

extern int (*internal_commands[]) ();

#define TOKEN_DELIM " \t\n"
