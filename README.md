# parallel-programming
Some examples of parallel programming

------
# File: primeNumber.c

- Prime Number Finder with OpenMP (Open Multi-Processing)

- Compile: gcc -fopenmp -o primeNumber primeNumber.c

- Exec: ./primeNumber “limit” “option” “number of threads”
  - argv[1] = Limit m (Find prime number of 2 to m);
  - argv[2] = Output Option;
    - time: One line with time of execution;
    - list: One line with a list of prime numbers;
    - sum: One line with sum of prime numbers;
    - all: List of prime numbers, the sum of these numbers and the time of execution. In this order;
  - argv[3] = Threads number to be created;

