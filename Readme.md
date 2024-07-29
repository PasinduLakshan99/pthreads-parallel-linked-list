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
| Number of threads                      :                    4     |
| n(Initial list size)                   :                12345     |
| m(Operations to perform)               :                67890     |
---------------------------------------------------------------------
| Percentage of mMember                  :                 33.33%   |
| Percentage of mInsert                  :                 33.33%   |
| Percentage of mDelete                  :                 33.33%   |
=====================================================================

--------------------------------------------------------------------

Populating list with 12345 random elements
List populated

--------------------------------------------------------------------

=====================================================================
|                   Performing serial operations                    |
=====================================================================
    => Each operation as a percentage of Total operations
       - Member: 33.33%
       - Insert: 33.33%
       - Delete: 33.33%
    Elapsed time with serial: 0.1234567890 seconds
=====================================================================


=====================================================================
|                Performing operations with mutex                   |
=====================================================================
    => Each operation as a percentage of Total operations
       - Member: 33.33%
       - Insert: 33.33%
       - Delete: 33.33%
    Elapsed time with mutex: 0.1234567890 seconds
=====================================================================


=====================================================================
|              Performing operations with rwlock                    |
=====================================================================
    => Each operation as a percentage of Total operations
       - Member: 33.33%
       - Insert: 33.33%
       - Delete: 33.33%
    Elapsed time with rwlock: 0.1234567890 seconds
=====================================================================
```

## Acknowledgments
- POSIX Threads Programming tutorial.
- Random number generation and normalization techniques.

Feel free to contribute or raise issues for improvements. Enjoy experimenting with concurrent programming!