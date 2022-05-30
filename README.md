# RISC Processor

## About
This project is an implementation of a risc-based processor with pipeline developed in C++ by means of systemc C++library. 

## Requirements
- Linux.
- Systemc 2.1.3 library.
- GCC compiler (>= 9.2.1).


## Compilation and Execution

Open up the command line and execute the following instructions:

- export SYSTEMC_HOME=/usr/local/systemc-2.3.1/

- g++ -I. -I\$SYSTEMC_HOME/include -L. -L\$SYSTEMC_HOME/lib-linux64 -Wl -rpath=$SYSTEMC_HOME/lib-linux64 -o main main.cpp -lsystemc -lm

- ./main < file.txt
    > Ps.: The file.txt is the file which has the algorithm to be executed.

## Input file

For creating your own algorithm and making the processor run it, you must create a .txt file with the following instructions:

Loading values into the data memory (RAM):
- memset memory_position value
  >memset 6 4

Processor instruction:

- OPCODE OP1 OP2 OP3
    >OPCODEs available: ADD, SUB, AND, OR, NOT, XOR, LD, ST, LI, J, JZ, JN