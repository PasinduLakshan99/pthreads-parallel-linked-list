# Concurrent Linked List Operations

## Overview
This program demonstrates concurrent operations (Member, Insert, Delete) on a linked list using three different synchronization methods: serial (no synchronization), mutexes, and read-write locks (rwlocks). The performance of each method is measured and compared.

## Features
- Initializes a linked list with a specified number of elements.
- Performs a specified number of random operations (Member, Insert, Delete) on the list.
- Supports multi-threaded execution with a configurable number of threads.
- Compares performance of different synchronization techniques: no synchronization (serial), mutex, and rwlock.

## Requirements
- GCC compiler
- POSIX thread library (pthread)

## Compilation
To compile the program, run the following command in the terminal:
```sh
gcc -o concurrent_list_ops main.c linked_list.c -lpthread
```

## Usage
Run the program with the following command:
```sh
./concurrent_list_ops <number_of_threads>
```
For example:
```sh
./concurrent_list_ops 4
```

## Program Details
- The program generates a random initial list size (`n`) and a random number of operations to perform (`m`).
- The operations are divided into three categories: Member, Insert, and Delete, with random proportions that sum up to 100%.
- The list is initially populated with `n` random elements.
- The operations are then performed serially, using mutexes, and using rwlocks.

## Example Output
```plaintext
=====================================================================
|                          Test Details                             |
=====================================================================
| Number of threads                        :                  5000  |
| n(Initial list size)                     :                 24865  |
| m(Operations to perform)                 :                 58721  |
---------------------------------------------------------------------
| Percentage of mMember                    :                  0.16  |
| Percentage of mInsert                    :                  0.16  |
| Percentage of mDelete                    :                  0.68  |
=====================================================================

--------------------------------------------------------------------

Populating list with 24865 random elements
List populated

--------------------------------------------------------------------

=====================================================================
|                   Performing serial operations                    |
=====================================================================
    => Each operation as a percentage of Total operations
       - Member: 0.16
       - Insert: 0.16
       - Delete: 0.67
    Elapsed time with serial: 7.8626260000 seconds
=====================================================================


=====================================================================
|                Performing operations with mutex                   |
=====================================================================
make: *** [Makefile:18: run] Segmentation fault (core dumped)
┌─[charindu-thisara@charindu-thisara-GF75-Thin-9SC] - [~/CodeBases/Pthread_LinkedList_LAB] - [Mon Jul 29, 20:50]
└─[$]> make run
./pth_linked_list.o 5000

=====================================================================
|                          Test Details                             |
=====================================================================
| Number of threads                        :                  5000  |
| n(Initial list size)                     :                 13758  |
| m(Operations to perform)                 :                 62648  |
---------------------------------------------------------------------
| Percentage of mMember                    :                  0.73  |
| Percentage of mInsert                    :                  0.23  |
| Percentage of mDelete                    :                  0.04  |
=====================================================================

--------------------------------------------------------------------

Populating list with 13758 random elements
List populated

--------------------------------------------------------------------

=====================================================================
|                   Performing serial operations                    |
=====================================================================
    => Each operation as a percentage of Total operations
       - Member: 0.73
       - Insert: 0.23
       - Delete: 0.04
    Elapsed time with serial: 6.3804070000 seconds
=====================================================================


=====================================================================
|                Performing operations with mutex                   |
=====================================================================
    => Each operation as a percentage of Total operations
       - Member: 0.73
       - Insert: 0.23
       - Delete: 0.04
    Elapsed time with mutex: 2.8328110000 seconds
=====================================================================


=====================================================================
|              Performing operations with rwlock                    |
=====================================================================
    => Each operation as a percentage of Total operations
      - Member: 0.73
      - Insert: 0.23
      - Delete: 0.04
    Elapsed time with mutex: 3.3895120000 seconds
=====================================================================
```

## Acknowledgments
- POSIX Threads Programming tutorial.
- Random number generation and normalization techniques.

Feel free to contribute or raise issues for improvements. Enjoy experimenting with concurrent programming!