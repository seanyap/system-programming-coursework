## Project Setup

#### Compile main C program with gcc

```C
gcc -Wall -Wextra -o2 -g -o runsim runsim.c
```

#### Compile C program for test with gcc

```C
gcc -g -o test test.c
```

#### Run program from its directory and redirect the comamnds in test.txt as command-line arguments to program

```C
./runsim <max-processes-num> < test.txt
```
