# Assembler-Emulator

AUTHOR: Kunj Taneja (kunj017)

* It is compilable with: g++ assembler.cpp -o asm and g++ emu.cpp -o emu
* To execute: ./asm test1.asm and ./emu test5.o

# Program format and structure:

1. The assembler:
	* Uses single routine for both pases to assemble the program
	* Handles any type of formatting in a line including leading or trailing white spaces.
	* Case insensitive.
	* Handles comments in the same line and separate lines
	* Can get data in decimal, octal and hexadecimal 2s complement
	* It can detect the following errors:
		* Incorrect mnemonic
		* Label name is a programming keyword
		* Incorrect label naming format (allows for underscore in beginning but doesn't allow numerics - in start or other special characters)
		* Label repeated
		* Label missing
		* Missing operand
		* More operands than expected (incorrect operand format)
	* It can detect the following warnings:
		* Unused labels
		* Possible numeric overflow

	* Keeps a track of mnemonic, opcodes and possible operands
	* Keeps a track of the labels
	* Generates a log file
	* Generates an advanced listing file (showing the bytes produced for each instruction, and that instruction's mnemonic)
	* Generates an object file
	* Listing and object files are only created if there are no errors. They may be created if there are only warnings
	* Assembles the pseudo instruction SET
	
2. The emulator:

	* Can load object file
	* can produce memory dump
	* can execute test program
	* has -t (trace), -dump and -all options
	* can detect errors:
		* Incorrect machine code
		* Segmentation fault
		* Stack overflow
		* Invalid opcode
	* can print metadata such as number of instructions executed, hexadecimal forms, memory dump, commands can be changed during runtime

