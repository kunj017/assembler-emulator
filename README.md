# Assembler-Emulator

AUTHOR: Kunj Taneja (kunj017)

* It is compilable with: g++ assembler.cpp -o asm and g++ emu.cpp -o emu
* To execute: ./asm test1.asm and ./emu test5.o

# Program format and structure:

### The assembler
* Uses single routine for both pases to assemble the program
* Handles any type of formatting in a line including leading or trailing white spaces.
* Case insensitive.
* Handles comments in the same line and separate lines
* Can get data in decimal, octal and hexadecimal 2s complement
* It can detect the following errors:
	- Bogus mnemonic
	- Invalid Label Name
	- Repeated label Name
	- Reference to an invalid label name
	- Label name can not be a keyword
	- Missing operand
	- Unexpected operand
	- Wrong format of numbers
	- Invalid input format
	- There can not be two labels in same line
	- No label for set instruction
	- Label not defined
* Assembler can issue the following warnings:
	- Unused label
	- Infinite loops

* Stores mnemonic and their opcodes.
* Stores defined labels in program.
* Uses single routine for both pases to assemble the program.
* Generates an advanced listing file.
* Generates an object file with .o extension
* Listing and object files are empty if there are any errors.
* Can implement the instruction SET.
	
### The emulator

* The emulator is interactive and can run single or multiple instructions according to user's input.

* Assumes that set and data insrtuctions will be written only after halt statement and should never be run else can produce ambigious results.

* Error handling: Throws error if instruction with invalid opcode comes, Throws error is segmentation fault occurs.

* Functionalities:
	- Single trace: Performs a single operation and shows current register values. ( Command: -t )
	- Multiple trace: Performs a given number of operations and shows current register values. ( Command: -t 10 (Here 10 has the value in decimal))
	- Full program trace: Performs all remaining operations. ( Command: -f )
	- Memory dump: Shows current memory dump i.e. data values at all PC values(total PC = number of instructions)  ( Command: -bd )
	- Show data: Displays the contents of given data location ( Command: -data 0xFF (Here the data has to be inputted as hexadecimal) )
	- After memory dump: Performs all the instructions in a program and then dumps the final memory. ( Command: -ad )
	- Show future instructions: Displays instructions written after current pc (displays at max 10 future instructions). ( Command: -u )

