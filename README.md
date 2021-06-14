#TO COMPILE
	
	1) Navigate to A2/src
	2) type 'make' without the quotes

#TO RUN
	
	1) Navigate to A2/src
	2) type 'make run' without the quotes
    or 	
	1) Navigate to A2/bin after compiling (see TO COMPILE)
	2) type './bcsh' without the quotes. bcsh stands for Brennan Couturier's Shell fyi

#TO CLEAN
	
	make clean

#NOTES
	
	Job control was not implemented. I did not set it up properly from the start to allow for job management, and remaking the entire shell could not be done in time.
	This shell supports unlimited arguments per command, there is no hardcoded limit.
	It can run single commands
	It can run an unlimited number of piped commands, there is no hardcoded limit to the number of pipes.
	The project is divided into subdirectories, so make sure you are in the proper directory before deciding if a command works or not.
		e.g. make will only work if you are in the A2/src directory, ./bcsh will only work if you are in the A2/bin directory
