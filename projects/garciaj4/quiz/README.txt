Jacob Garcia
CS362_400_U2018
Random Quiz Submission
7/22/2018

Exposes the error in the testme.c code.

To run the test run 'make' from the directory.

The results of the test will be output to 'randomtestresults.out'
Because the error causes an exit status the make command will not conclude,
exiting with the 200 error status present in the source.

To confirm the error message was printed, use the command
'tail -2 randomtestresults.out' to view the last two lines of the .out file.  This should
contain the state of the file upon exit and the error message.

Command 'make clean' will remove all created files.