
/**
 * * * * * * * * * * * * * * * * * * *
 * Brennan Couturier
 * * * * * * * * * * * * * * * * * * *
 * bcsh_execution.c
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
#include <sys/wait.h>
#include <sys/types.h>

#include "../include/bcsh_constants.h"
#include "../include/bcsh_internals.h"

/**
 * This function takes a list of tokens and executes the commands they represent. It
 *  first checks if the first command is an internal one (cd or exit), before dealing
 *  with it accordingly.
 *  This function is used if there are no pipes in the input
 * @param tokens An array of string tokens representing commands and arguments
 * @return 0 if the shell can continue, 1 if we have to stop (got exit command, or something)
 */
int execute_command(char** tokens){

    /********************************************************************
    Declare variables
    ********************************************************************/
    char* command = tokens[0];
    int i;
    pid_t pid;
    int status;
    int return_code;

    /********************************************************************
    Check if user entered an empty line
    ********************************************************************/
    if(command == NULL){
        return 0;
    }

    /********************************************************************
    Check if program is an internal command (cd or exit)
    ********************************************************************/
    for(i = 0; i < NUM_INTERNAL_COMMANDS; i++){
        if(strncmp(command, internal_command_names[i], strlen(internal_command_names[i])) == 0){
            return internal_commands[i](tokens); //This runs the function using its pointer, passing tokens as an argument
        }
    }

    /********************************************************************
    Otherwise, run the external command
        If there is an error, abort but allow the user to continue using
        the shell. Basically, we're always going to return 0. The internal
        command exit is the only one that returns 1 (stop the shell).
    ********************************************************************/
    pid = fork();
    if(pid == 0){
        //We're in child
        return_code = execvp(command, tokens);
        if(return_code == -1){
            perror("Error in execute_command() : execvp failed ");
            exit(EXIT_FAILURE); //End the child process
        }
    }else{
        //We're in parent
        if(pid == -1){
            //Fork failed
            perror("Error in execute_command() : fork failed ");
            return EXIT_SUCCESS;
        }
        waitpid(pid, &status, 0);
    }

    return EXIT_SUCCESS;
}

/**
 * If the input has pipes in it, this function will handle them, executing each command
 *  within this function (it does not use execute_command()).
 */
int execute_piped_commands(char*** prepared_commands){

    /********************************************************************
    Declare variables
    ********************************************************************/
    int old_pipe[2];
    int new_pipe[2];
    int return_code;
    int err;
    pid_t pid;
    int num_commands = 0;
    int status;

    int i = 0;
    int j;
    while(prepared_commands[i] != NULL){
        num_commands++;

        /********************************************************************
        If there is a next command, create a pipe using new_pipe
        ********************************************************************/
        if(prepared_commands[i + 1] != NULL){
            err = pipe(new_pipe);
            if(err == -1){
                perror("Error in execute_piped_commands() : Could not create pipe ");
                return EXIT_SUCCESS;
            }
        }


        pid = fork();
        if(pid == 0){
            //We're in child
            /********************************************************************
            If there is a command before this, make this process read from pipe
                Copy read end using dup2 and close both file descriptors
                Use old_pipe
            ********************************************************************/
            if(i > 0){
                dup2(old_pipe[0], 0);
                close(old_pipe[0]);
                close(old_pipe[1]);
            }

            /********************************************************************
            If there is a command after this, make this process write to pipe
                Copy write end using dup2 and close both file descriptors
                Use new_pipe
            ********************************************************************/
            if(prepared_commands[i + 1] != NULL){
                close(new_pipe[0]);
                dup2(new_pipe[1], 1);
                close(new_pipe[1]);
            }

            /********************************************************************
            Execute command
                If it is an internal command, execute it accordingly
            ********************************************************************/
            for(j = 0; j < NUM_INTERNAL_COMMANDS; j++){
                if(strncmp(prepared_commands[i][0], internal_command_names[j], strlen(internal_command_names[j])) == 0){
                    return internal_commands[j](prepared_commands);
                }
            }

            return_code = execvp(prepared_commands[i][0], prepared_commands[i]);
            if(return_code == -1){
                perror("Error in execute_piped_commands() : execvp failed ");
                exit(EXIT_FAILURE); //End the child process
            }

        }else{
            //We're in parent
            /********************************************************************
            Check if fork failed
            ********************************************************************/
            if(pid == -1){
                //Fork failed
                perror("Error in execute_command() : fork failed ");
                return EXIT_SUCCESS;
            }

            /********************************************************************
            If there is a previous command, close old_pipe
            ********************************************************************/
            if(i > 0){
                close(old_pipe[0]);
                close(old_pipe[1]);
            }

            /********************************************************************
            If there is a next command, old_pipe = new_pipe
            ********************************************************************/
            if(prepared_commands[i + 1] != NULL){
                old_pipe[0] = new_pipe[0];
                old_pipe[1] = new_pipe[1];
            }
        }
        i++;
    }

    if(i > 0){
        close(old_pipe[0]);
        close(old_pipe[1]);
    }

    for(i = 0; i < num_commands; i++){
        waitpid(-1, &status, WUNTRACED);
    }



    return EXIT_SUCCESS;

}
