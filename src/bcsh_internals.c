/**
 * * * * * * * * * * * * * * * * * * *
 * Brennan Couturier
 * * * * * * * * * * * * * * * * * * *
 * bcsh_internals.c
 * * * * * * * * * * * * * * * * * * *
 * The internal commands cd and exit
 *  will be implemented here
 * * * * * * * * * * * * * * * * * * *
 */
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>

#include "../include/bcsh_constants.h"

/**
 * This function attempts to change the current working directory with the path
 *  provided in the argument.
 */
int cd_internal(char** tokens){
    /********************************************************************
    Declare variables
    ********************************************************************/
    char* path;
    int err;

    /********************************************************************
    Check if the provided path is null (no provided path)
    ********************************************************************/
    if(tokens[1] == NULL){
        fprintf(stderr, "Error in cd_internal() : No path provided\n");
        return EXIT_SUCCESS;
    }

    /********************************************************************
    Chdir() requires the path variable be addressable, so put it in heap
    ********************************************************************/
    path = malloc((strlen(tokens[1]) + 1) * sizeof(char));
    if(path == NULL){
        fprintf(stderr, "Error in cd_internal() : Could not allocate space for path\n");
        return EXIT_SUCCESS;
    }

    /********************************************************************
    Copy the path token into the allocated space
    ********************************************************************/
    strncpy(path, tokens[1], strlen(tokens[1]) + 1);

    /********************************************************************
    Change the current directory using chdir()
    ********************************************************************/
    err = chdir(path);
    if(err == -1){
        perror("Error in cd_internal ");
    }

    free(path);

    return EXIT_SUCCESS;
}

int exit_internal(char** tokens){
    return 1;
}

int fg_internal(char** tokens){
    return 0;
}

int bg_internal(char** tokens){
    fprintf(stderr, "bg\n");
    return 0;
}

char* internal_command_names[] =
    {
        "cd",
        "exit",
        "fg",
        "bg"
    };

int (*internal_commands[])(char** tokens) =
    {
        &cd_internal,
        &exit_internal,
        &fg_internal,
        &bg_internal
    };
