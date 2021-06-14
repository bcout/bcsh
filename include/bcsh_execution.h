/**
 * * * * * * * * * * * * * * * * * * *
 * Brennan Couturier
 * * * * * * * * * * * * * * * * * * *
 * bcsh_execution.h
 * * * * * * * * * * * * * * * * * * *
 * Functions used to execute commands
 * * * * * * * * * * * * * * * * * * *
 */

int execute_command(char** tokens);

int execute_piped_commands(char*** prepared_commands);
