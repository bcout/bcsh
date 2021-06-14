/**
 * * * * * * * * * * * * * * * * * * *
 * Brennan Couturier
 * * * * * * * * * * * * * * * * * * *
 * bcsh_utils.h
 * * * * * * * * * * * * * * * * * * *
 * Some miscellaneous useful functions
 * * * * * * * * * * * * * * * * * * *
 */

void get_cwd_direct();

char** get_tokens(char* line);

int has_pipes(char** tokens);

char*** prepare_commands(char** tokens);
