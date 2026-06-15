# Codexion

This project has been created as part of the 42 curriculum by [mosafa]https://profile-v3.intra.42.fr/users/mosafa

## Description

This project simulates multiple coder threads working to achieve a target number of compiles without burning out. It manages dongle distribution safely without data race 
issues and ensures each coder compiles before their deadline. The system uses FIFO (First In First Out) and EDF (Earliest Deadline First) scheduling algorithms with a 
queue-based approach, along with a monitoring thread that tracks coder deadlines and stops the entire process if any coder burns out.


## Instructions

to compile all files run:

```Bash
make
#or
make all
```

to remove all .o files run:

```Bash
make clean
```
to remove all .o and the codexion files run:

```Bash
make fclean
```
to clean and than recompile run:

```Bash
make re
```
to run the code run (./codexion (nb of coder) (burnout time) (time to compile) (time to debug) (time to refactor) (nb of compiles) (cooldown time) (edf or fifo)) example:

```Bash
./codexion 30 800 200 200 200 10 100 edf
```


## Resource

Thread and mutex understanding: https://youtube.com/playlist?list=PLfqABt5AS4FmuQf70psXrsMLEDQXNkLq2&si=ray0aiMME9fOf3ZV


**I used AI to understand how to fix data race in my code and how to implement the queue.**

## input instructions:

Each argument represents the simulation settings, including the number of coder threads, the maximum time before burnout, the time required for compiling, debugging, and 
refactoring tasks, the target number of compiles, the cooldown/rest duration, and the scheduling algorithm to use (EDF or FIFO).

**hint: dont use a time below 60 ms**

## Blocking cases handled

handled the blocking issue by using a monitor thread that continuously checks the queue and selects a coder to acquire the two dongles while holding the mutex. This way, 
no coder blocks another.


### Thread synchronization mechanisms:

Thread synchronization is achieved using a mutex to protect shared resources and ensure mutual exclusion. Threads must acquire the mutex before accessing or modifying 
shared data, preventing race conditions and maintaining data consistency.

