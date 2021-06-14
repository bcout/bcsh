# TO COMPILE
	
Navigate to /src

```$ make```

# TO RUN
	
Navigate to /src

```$ make run```

or 	

Navigate to /bin after compiling (see TO COMPILE)

```$ ./bcsh```

# TO CLEAN
	
```$ make clean```

# USAGE

Use like a normal shell. Keep in mind it will most certainly not have as many features as most fully fledged shells, but it does function for basic commands

# NOTES

- Job control is not implemented.
- This shell supports unlimited arguments per command, there is no hardcoded limit.
- It can run single commands
- It can run an unlimited number of piped commands, there is no hardcoded limit to the number of pipes.
- The project is divided into subdirectories, so make sure you are in the proper directory before deciding if a command works or not.
	- e.g. make will only work if you are in the /src directory, ./bcsh will only work if you are in the /bin directory
