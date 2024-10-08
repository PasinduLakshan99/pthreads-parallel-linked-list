# Concurrent Linked List Operations

## Overview
This program demonstrates concurrent operations (Member, Insert, Delete) on a linked list using three different synchronization methods: serial (no synchronization), mutexes, and read-write locks (rwlocks). The performance of each method is measured and compared.

## Features
- Initializes a linked list with a specified number of elements.
- Performs a specified number of random operations (Member, Insert, Delete) on the list.
- Supports multi-threaded execution with a configurable number of threads.
- Enable Comparison of performance with different synchronization techniques: no synchronization (serial), mutex, and rwlock.

## Requirements
- GCC compiler
- POSIX thread library (pthread)
- make (optional)

## Compilation
To compile the program, run the following command in the terminal:
```sh
gcc -o concurrent_list_ops main.c linked_list.c -lpthread
```
or if you have make installed, then in the project root directory run:
```sh
make 
```

## Usage


### 1. Run the Program

#### Run the program with a specific number of threads, and set the proportions of Member and Insert operations:


```sh
./concurrent_list_ops <number_of_threads> <nMember> <nInsert>
```

Example:
```sh
./concurrent_list_ops 4 0.99 0.005
```
--- ---
#### If you have make installed, you can also use:
```sh
make run threads=<number_of_threads> mMember=<nMember> nInsert=<nInsert>
```

Example:
```sh
make run threads=4 mMember=0.99 mInsert=0.005
```

### 2. Generate a CSV File of Execution Times
Generate a CSV file of execution times for thread counts 1, 2, 4, and 8 with a specified sample size:

```sh
./concurrent_list_ops -gen-csv <number_of_samples> <nMember> <nInsert>
```

Example:
```sh
./concurrent_list_ops -gen-csv 10 0.99 0.005
```
--- ---
If you have make installed, you can also use:
```sh
make gen-csv samples=<number_of_samples> mMember=<nMember> nInsert=<nInsert>
```

Example:
```sh
make gen-csv samples=10 mMember=0.99 mInsert=0.005
```

## Program Details
- The program uses defined values for n(=1000) & m(=10000). And these values can be changes as necessary.
- The operations are divided into three categories: Member, Insert, and Delete, with random proportions that sum up to 100%.
- The list is initially populated with `n` random elements.
- The operations are then performed serially, using mutexes, and using rwlocks.

## Example Output
### Simple run
```plaintext
lucky@nitro5 ~/pthreads-parallel-linked-list $ make run threads=4 mMember=0.99 mInsert=0.005

./pth_linked_list.o 4 0.99 0.005

=====================================================================
|                          Test Details                             |
=====================================================================
| Number of threads                        :                     4  |
| n(Initial list size)                     :                 10000  |
| m(Operations to perform)                 :                100000  |
---------------------------------------------------------------------
| Percentage of mMember                    :                 0.990  |
| Percentage of mInsert                    :                 0.005  |
| Percentage of mDelete                    :                 0.005  |
=====================================================================

--------------------------------------------------------------------

Populating list with 10000 random elements
List populated

--------------------------------------------------------------------

=====================================================================
|                   Performing serial operations                    |
=====================================================================
    Elapsed time with serial: 1.4901407870 seconds
=====================================================================


=====================================================================
|                Performing operations with mutex                   |
=====================================================================
    => Thread count: 4
    Elapsed time with mutex: 1.9527659560 seconds
=====================================================================


=====================================================================
|              Performing operations with rwlock                    |
=====================================================================
    => Thread count: 4
    Elapsed time with rwlock: 0.6707091240 seconds
=====================================================================
```
### Run tests
```plaintext
lucky@nitro5 ~/pthreads-parallel-linked-list $ make gen-csv samples=10 mMember=0.99 mInsert=0.005
 
gcc -g -Wall -o pth_linked_list.o main.c linked_list.c linked_list.h -lpthread
./pth_linked_list.o -gen-csv 10 0.99 0.005
    Elapsed time with serial: 1.7694473610 seconds
    => Thread count: 1
    Elapsed time with mutex: 1.6090136150 seconds
    => Thread count: 1
    Elapsed time with rwlock: 1.7903477480 seconds
    Elapsed time with serial: 1.7227859240 seconds
    => Thread count: 1
    Elapsed time with mutex: 1.7721479240 seconds
    => Thread count: 1
    Elapsed time with rwlock: 1.9433979740 seconds
    Elapsed time with serial: 1.7031998890 seconds
    .
    .
    .
    .
    .
    => Thread count: 8
    Elapsed time with rwlock: 0.6814243760 seconds
    => Thread count: 8
    Elapsed time with mutex: 2.0017901690 seconds
    => Thread count: 8
    Elapsed time with rwlock: 0.6768141530 seconds
    => Thread count: 8
    Elapsed time with mutex: 1.9697233840 seconds
    => Thread count: 8
    Elapsed time with rwlock: 0.6221404040 seconds
Data written to output.csv
```
