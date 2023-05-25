# Process Manager

## Table of Contents

- [Technology Stack 🛠️](#technology-stack-)
- [Prerequisites 🍪](#prerequisites-)
- [Setup 🔧](#setup-and-deployment-)

## Technology Stack 🛠️

This project uses C and Makefile.

## Prerequisites 🍪

You should make sure that you are running this program in a Linux environment

## Setup 🔧

How to compile and run the process manager

1. Compile the program using "make" on the command line:

   ```bash
     make
   ```

2. Run the program using "./processmanager" on the command line

   ```bash
     ./processmanager
   ```

3. Here are a list of commands the program accepts:
	- bg: You can use this command to start a background process
	
		Syntax: bg <program name>
		(Ex. "bg ./test" will run the program ./test)
		```bash
		bg ./test
		```

	- bgstop: You can use this command to stop a running program from its pid

		Syntax: bgstop <pid>
		(Ex. "bgstop 1234" will stop the program with the pid 1234)
		```bash
		bgstop 1234
		```

	- bgstart: You can use this command to start a program which was stopped from its pid

		Syntax: bgstart <pid>
		(Ex. "bgstart 1234" will start the program with the pid 1234)
		```bash
		bgstart 1234
		```

	- bgkill: You can use this command to kill a running program from its pid

		Syntax: bgkill <pid>
		(Ex. "bgkill 1234" will kill the program with the pid 1234)
		```bash
		bgkill 1234
		```

	- bglist: You can use this command to list all the active background jobs

		Syntax: bglist
		```bash
		bglist
		```
		

	- pstat: You can use this command to check the status of the process with its pid

		Syntax: pstat <pid>
		(Ex. "pstat 1234" will list information related to the process pid)
		```bash
		pstat 1234
		```

