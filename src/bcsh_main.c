/**
 * * * * * * * * * * * * * * * * * * *
 * Brennan Couturier
 * * * * * * * * * * * * * * * * * * *
 * bcsh_main.c
 * * * * * * * * * * * * * * * * * * *
 */

//chdir(), fork(), pid_t, getcwd()
#include <unistd.h>
//execvp()
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>

#include "../include/bcsh_constants.h"
#include "../include/bcsh_io.h"
#include "../include/bcsh_utils.h"
#include "../include/bcsh_execution.h"
#include "../include/bcsh_signals.h"

volatile sig_atomic_t signal_handled = 0;

/**
 * Starts up Brennan Couturier's Shell (bcsh) and runs it
 * This program will read commands from stdin, separate them into tokens, then execute them if they're valid
 * @param argc The number of command line arguments
 * @param argv A reference to an array of command line arguments
 * @return Returns EXIT_SUCCESS if everything goes well, EXIT_FAILURE if things did not go well
 */
int main(int argc, char** argv){

    /********************************************************************
    Declare and initialize variables
    ********************************************************************/
    char* path = NULL;
    char** path_ref = &path;
    char* line = NULL;
    char** tokens = NULL;
    char*** piped_commands = NULL;
    int done = 0;

    /********************************************************************
    Register for SIGTSTP signal (ctrl-z)
        Also, store the current process id. The handler uses it to
        determine if a program is running on top of the shell or not.
    ********************************************************************/
    signal(SIGTSTP, signal_handler);

    do{

        /********************************************************************
        Get the current working directory and print it out
            We are getting it each iteration in case it was changed in the
            previous iteration.
        ********************************************************************/
        get_cwd_direct(path_ref);

        if(signal_handled){
            fprintf(stdout, "\n");
            signal_handled = 0;
        }
        fprintf(stdout, "%s%% ", *path_ref);

        /********************************************************************
        Get a line of input
        ********************************************************************/
        line = read_line_stdin();

        /********************************************************************
        Tokenize the input
        ********************************************************************/
        tokens = get_tokens(line);

        /********************************************************************
        Execute the commands contained in tokens
            Treat commands with pipes different than simple commands
        ********************************************************************/
        if(has_pipes(tokens)){
            piped_commands = prepare_commands(tokens);
            if(piped_commands != NULL){
                done = execute_piped_commands(piped_commands);
            }
        }else{
            done = execute_command(tokens);
        }


        /********************************************************************
        Free our malloced variables so that when we start the next iteration
            we don't need to worry about losing track of memory. We're going
            to end up mallocing them again, so it's better to free here than
            inside a bunch of different functions
        ********************************************************************/
        if(piped_commands != NULL){
            int k = 0;
            while(piped_commands[k] != NULL){
                free(piped_commands[k]);
                piped_commands[k] = NULL;
                k++;
            }
            free(piped_commands);
            piped_commands = NULL;
        }
        free(*path_ref);
        *path_ref = NULL;
        free(tokens);
        tokens = NULL;
        free(line);
        line = NULL;

    }while(!done);



    return EXIT_SUCCESS;
}
