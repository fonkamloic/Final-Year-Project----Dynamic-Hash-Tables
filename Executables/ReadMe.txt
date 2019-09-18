
Execution:  

After compilation, run the code as follows:-



SYNOPSIS
./<dic_hashTable | log_hashTable>		//this option with no argument opens an interface one number options to create, delete 							insert, etc

./<dic_hashTable | log_hashTable>        -s TABLE_SIZE

./<dic_hashTable | log_hashTable>        -l INPUT_SIZE

./<dic_hashTable | log_hashTable>        -s TABLE_SIZE -l INPUT_SIZE

DESCRIPTION
	Create Hash Table of initial size TABLE_SIZE and loads INPUT_SIZE number of data(integers)

-s
	Flag to specify size of hash Table. If not provided DEFAULT SIZE is 10000
-l
	Flag to specify size of input to be loaded. If not provided DEFAULT SIZE is 1000000

	
	NOTE: 
	
	1-	run ./<dic_hashTable | log_hashTable>  with no command line argument will not create a output file. To have an output file containing the execution time of the various operations one did, you need to run ./<dic_hashTable | log_hashTable> with command line agurment -s or -l or both. At the end of the execution, a Benchmark.txt file will be created in the working directory containing output results. 

	2-	We can use a bash shell loop to execute our program with varied input size as follows:
	
	   $ for ((inputSize = value ; inputSize <= upperBound ; inputSize *= 2)) 
			do 
				./<dic_hashTable | log_hashTable>	-s	TABLE_SIZE	-l	$inputSize
			done
		
	this loop will run the program many chosen number of times.
