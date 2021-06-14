/**
 * * * * * * * * * * * * * * * * * * *
 * Brennan Couturier
 * * * * * * * * * * * * * * * * * * *
 * bcsh_utils.c
 * * * * * * * * * * * * * * * * * * *
 * All the functions I will use for I/O
 *  are declared here
 * * * * * * * * * * * * * * * * * * *
 */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

#include "../include/bcsh_constants.h"
#include "../include/bcsh_internals.h"

/**
 * This function is given a reference to a string and will set the reference to point to the
 *  current working directory.
 * @param path_ref A reference to the current working directory string
 */
void get_cwd_direct(char** path_ref){

    /********************************************************************
    Declare variables
    ********************************************************************/
    int offset = 1;
    char* path;
    size_t actual_space_needed;

    /********************************************************************
    Get the current working directory using getcwd()
        If the buffer isn't big enough, keep retrying with larger buffers
    ********************************************************************/
    path = getcwd(NULL, INITIAL_PATH_LENGTH * sizeof(char));
    while(path == NULL){
        if(errno == ERANGE){
            //Buffer was not big enough, we can come back from this.
            offset = offset * 2;
            path = getcwd(NULL, INITIAL_PATH_LENGTH * offset);
        }else{
            //A different error occured, we will not attempt to come back from this.
            perror("Error in get_cwd_direct()");
            exit(EXIT_FAILURE);
        }
    }

    /********************************************************************
    Path now holds the current working directory, but it was probably
        given too much space, so we'll reallocate it to only use what it
        needs
    ********************************************************************/
    actual_space_needed = strlen(path) + 1;
    path = realloc(path, actual_space_needed);
    if(path == NULL){
        fprintf(stderr, "Error in get_cwd_direct() : Failed to reallocate space for current working directory\n");
        exit(EXIT_FAILURE);
    }

    /********************************************************************
    Now we need to make path_ref point to path
        We know that *path_ref has been freed or is NULL at this point
        because main() frees *path_ref at the end of the execution loop,
        allowing us to do this.
    ********************************************************************/
    *path_ref = path;
}

/**
 * This function takes a string whitespace separated tokens and will separate them using strtok_r. It will then
 *  populate and return an array of these tokens. It will exit() upon encountering memory allocation errors.
 * @param line A string containing all the tokens to be split up
 * @return An array of string tokens
 */
char** get_tokens(char* line){

    /********************************************************************
    Declare variables
    ********************************************************************/
    char** tokens;
    char* token;
    char* saveptr = NULL;
    size_t buffer_position = 0;
    size_t buffer_size = INITIAL_TOKEN_BUFFER;
    size_t num_tokens = 0;

    /********************************************************************
    Allocate space for tokens
    ********************************************************************/
    tokens = malloc(buffer_size * sizeof(char*));
    if(tokens == NULL){
        fprintf(stderr, "Error in get_tokens() : Could not allocate space for tokens array\n");
        exit(EXIT_FAILURE);
    }

    /********************************************************************
    Tokenize the string
    ********************************************************************/
    token = strtok_r(line, TOKEN_DELIM, &saveptr);
    while(token != NULL){

        tokens[buffer_position] = token;
        buffer_position++;
        num_tokens++;

        //If there are more tokens than we allocated space for, reallocate more space
        if(buffer_position >= buffer_size){
            tokens = realloc(tokens, (buffer_size + INITIAL_TOKEN_BUFFER) * sizeof(char*));
            if(tokens == NULL){
                fprintf(stderr, "Error in get_tokens() : Could not reallocate space for tokens array\n");
                exit(EXIT_FAILURE);
            }
        }

        token = strtok_r(NULL, TOKEN_DELIM, &saveptr);
    }

    /********************************************************************
    Realloc the tokens array to only use what it needs
    ********************************************************************/
    tokens = realloc(tokens, (num_tokens + 1) * sizeof(char*));
    if(tokens == NULL){
        fprintf(stderr, "Error in get_tokens() : Could not reallocate space for tokens array\n");
        exit(EXIT_FAILURE);
    }

    /********************************************************************
    Null terminate the array to that we can traverse it without knowing
        the size
    ********************************************************************/
    tokens[num_tokens] = NULL;

    return tokens;
}


/**
 * This function parses the list of tokens, looking for a pipe
 * @param tokens A null-terminated list of string arguments
 * @return 1 if one or more pipes are found, 0 otherwise
 */
int has_pipes(char** tokens){
    int i = 0;
    while(tokens[i] != NULL){
        if(strncmp(tokens[i], "|", strlen("|")) == 0){
            return 1;
        }
        i++;
    }
    return 0;
}

/**
 * This function takes an array of tokens separated by one or more pipes.
 *  It groups the tokens on each side of the pipes together and stores
 *  them all in an array of string arrays
 * ex. ls -l | grep a | more
 *  prepared_commands = {{ls, -l}, {grep, a}, {more}}
 * @param tokens A null-terminated list of string arguments
 * @return An array of prepared commands to be executed using execute_piped_commands(), NULL if there is an error.
 */
char*** prepare_commands(char** tokens){
    /********************************************************************
    Declare variables
    ********************************************************************/
    char*** prepared_commands;
    char** grouped_commands;
    int num_commands = 0;
    int num_commands_in_group = 0;
    int i, j, n, m;

    /********************************************************************
    Find out how many actual commands there are
        This works out to be the number of pipes plus one. So, count
        the number of pipes, then add one when it's all done.
    ********************************************************************/
    i = 0;
    while(tokens[i] != NULL){
        if(strncmp(tokens[i], "|", strlen("|")) == 0){
            num_commands++;
        }
        i++;
    }
    num_commands++;

    /********************************************************************
    Allocate space for the list of command groups plus space for a null
        terminator
    ********************************************************************/
    prepared_commands = malloc((num_commands + 1) * sizeof(char**));
    if(prepared_commands == NULL){
        fprintf(stderr, "Error in prepare_commands() : Could not allocate space for prepared_commands array\n");
        return NULL;
    }

    /********************************************************************
    Populate the prepared_commands list
    ********************************************************************/
    n = 0;
    for(i = 0; i < num_commands; i++){
        num_commands_in_group = 0;
        m = n;

        /********************************************************************
        Count how many commands are in the command group
            ex. ls -l would have two 'commands' (ls and -l)
        ********************************************************************/
        while(tokens[n] != NULL && strncmp(tokens[n], "|", strlen("|")) != 0){
            num_commands_in_group++;
            n++;
        }
        n++;

        /********************************************************************
        Allocate space for the group of commands plus a null terminator
        ********************************************************************/
        grouped_commands = malloc((num_commands_in_group + 1) * sizeof(char*));
        if(grouped_commands == NULL){
            fprintf(stderr, "Error in prepare_commands() : Could not allocate space for grouped_commands array\n");
            return NULL;
        }

        /********************************************************************
        Put the commands into the group, null terminate it, then add that
            group to the list
        ********************************************************************/
        for(j = 0; j < num_commands_in_group; j++){
            grouped_commands[j] = tokens[m];
            m++;
        }
        grouped_commands[num_commands_in_group] = NULL;
        prepared_commands[i] = grouped_commands;
    }

    /*******************************************************************
    Terminate the list with a null terminator then return it
    ********************************************************************/
    prepared_commands[num_commands] = NULL;
    return prepared_commands;
}
