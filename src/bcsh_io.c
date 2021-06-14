/**
 * * * * * * * * * * * * * * * * * * *
 * Brennan Couturier
 * * * * * * * * * * * * * * * * * * *
 * bcsh_io.c
 * * * * * * * * * * * * * * * * * * *
 * All the functions I will use for I/O
 *  are defined here
 * * * * * * * * * * * * * * * * * * *
 */

#include <stdio.h>
#include <stdlib.h>

/**
 * Reads a line from stdin using getline() and returns it
 * @return The line read from stdin, NULL if getline() throws an error
 */
char* read_line_stdin(){

    /********************************************************************
    Declare variables
    ********************************************************************/
    char* line = NULL;
    size_t buffer = 0; //getline decides how long this buffer should be
    size_t chars_read;

    /********************************************************************
    getline() allocates space for the line, no matter how big it is
        That means we need to free() it in main(), though.
    ********************************************************************/
    chars_read = getline(&line, &buffer, stdin); //Returns a null terminated string with a newline at the end
    if(chars_read == -1){
        perror("ERROR in read_line_stdin() : getline() returned -1 characters read\n");
        return NULL;
    }

    return line;
}
