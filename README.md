# parallel-programming
Some examples of parallel programming

>> WARNING: EXAMPLES BELOW ARE NOT MOST EFFICIENT. THEY WERE CONSTRUCTED FOR TESTING, PERFORMANCE COMPARISON AND PARALLEL PROGRAMMING PRACTICE.

------
# File: primeNumber.c

- Prime Number Finder with OpenMP (Open Multi-Processing)

- This program seeks by all the prime numbers in a stipulated range of 2 to m and displays the result according to the chosen by user option.

- Compile: gcc -fopenmp -o primeNumber primeNumber.c

- Exec: ./primeNumber “limit” “option” “number of threads”
  - argv[1] = Limit m (Find prime number of 2 to m);
  - argv[2] = Output Option;
    - time: One line with time of execution;
    - list: One line with a list of prime numbers;
    - sum: One line with sum of prime numbers;
    - all: List of prime numbers, the sum of these numbers and the time of execution. In this order;
  - argv[3] = Threads number to be created;
 
------
# File: primeNumberS.c

- Prime Number Finder Serial Version

- This program seeks by all the prime numbers in a stipulated range of 2 to m and displays the result according to the chosen by user option.

- Compile: gcc -o primeNumberS primeNumberS.c

- Exec: ./primeNumberS “limit” “option” “number of threads”
  - argv[1] = Limit m (Find prime number of 2 to m);
  - argv[2] = Output Option;
    - time: One line with time of execution;
    - list: One line with a list of prime numbers;
    - sum: One line with sum of prime numbers;
    - all: List of prime numbers, the sum of these numbers and the time of execution. In this order;
  - argv[3] = Threads number to be created;
  
------
# File: oceanSerial.c

- Ocean Approuch Serial Version

- Compile: gcc -o oceanSerial oceanSerial.c

- Exec: ./oceanSerial “rows” “columns” “cycles”  “number of threads” “option”

- Soon the description

------
# File: oceanParallel.c

- Ocean Approuch with POSIX Threads (Pthreads)

- Compile: gcc -g -Wall -o oceanParallel oceanParallel.c -lpthread

- Exec: ./oceanParallel “rows” “columns” “cycles”  “number of threads” “option”

- Soon the description

------
# File: oceanParallelMPI.c

- Ocean Approuch with Message Passing Interface (MPI)

- Compile: mpicc -g -Wall -o oceanParallelMPI oceanParallelMPI.c

- Exec: mpiexec -n “number of threads” ./oceanParallelMPI “rows” “columns” “cycles” “option”

- Soon the description
