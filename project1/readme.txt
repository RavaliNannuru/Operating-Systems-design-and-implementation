CS 551 Project 1 Minix Shell Implementation

Team Members
Balaji A R (A20347964)
Paulo Vitor Becker Ayrosa Monterio de Andrade (A20341532)
Sai Ravali Nannuru (A20354346)

The folder contains the following files :
1. myshell.c, myshell.h : Contains code for our shell implementation with command suggestion feature.
2. .profile : contains initialization values for shell
3. cmd_history.txt: A text file to store history of commands executed by the user.
4. myshell: An executable binary for our shell
5. project1_design_document : A design document for our shell implementation.
6. execshell: shell script to launch our shell


Steps to build:
The folder contains the build binaries. If you want to make changes and rebuild
again, use following commands:
1. make myshell to create the binary file for our shell.

Run the following command to execute our shell:
	./execshell from the directory where our file is stored.

Steps to change .profile:
 1. the .profile file contains PATH HOME TIMER key value paris for input to our shell. change these values and rerun our shell to take these changes into effect.	
	
Steps to test:
1. To test the timer, run the following commands after starting the shell created by us. :
	# ./execshell
	-> cat
   If the timer prompt appears, enter ctrl+d to terminate. else prompt will continue to appear of the next timer expires.
2.  To test command suggestion: follow execution of below commands
	#./execshell
	$ how are you
	$ ho
	after typing ho, press tab to see the suggestion from history file. it will display “how are you”
3.  To test input redirection:
	# ./execshell
	-> ls => test
	after the command execution the test file will be created in the current directory and cat the test file to see the contents matches the files in the current directory.
4. Shell functionality testing:
	# ./execshell
	-> ls
	after the command execution our shell will shows the list of files present in the current directory.
5. Type exit command to exit the shell.