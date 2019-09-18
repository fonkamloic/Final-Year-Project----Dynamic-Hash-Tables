Compilation: 

To compile the source codes, simple run the command 'make' in a terminal window (while in the source code directory). This will execute the Makefile and generate an excutable
called log_hashTable. 

Execution:

After compilation, run the code as follows:-



SYNOPSIS
./log_hashTable				//this option with no argument opens an interface one number options to create, delete 							insert, etc

./log_hashTable        -s TABLE_SIZE

./log_hashTable        -l INPUT_SIZE

./log_hashTable        -s TABLE_SIZE -l INPUT_SIZE

DESCRIPTION
	Create Hash Table of initial size TABLE_SIZE and loads INPUT_SIZE number of data(integers)

-s
	Flag to specify size of hash Table. If not provided DEFAULT SIZE is 10000
-l
	Flag to specify size of input to be loaded. If not provided DEFAULT SIZE is 1000000

	
NOTE: 
1- run ./log_hashTable  with no command line argument will not create a output file. To have an output file containing the execution time of the various operations one did, you need to run ./log_hashTable with command line agurment -s or -l or both. At the end of the execution, a Benchmark.txt file will be created in the working directory containing output results. 

