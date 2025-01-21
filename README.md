# Custom_assembler
Custom Assembler Implementation in C language

In the main folder there is the makefile and the runtime file.
Also, the headers folder (h files), the Source folder (c files), the Valid_inputs folder which contains examples of valid inputs, the Invalid_inputs folder which contains examples of invalid inputs, that is, fall in the compilation phase of the assembler I built.

**Using the code:**
In the terminal, run the runtime file together with the file names, without their extension (which contain the invented assembly language). You can input as many files as you want.

**The logic of the code is divided into 4 transitions:**
Macro pass - goes through macros in the file, replaces macro definitions with their code and remove comments. Produces a macro table.
First pass - mainly checks the syntax in the file as well as small tests that will help us in the second pass. Produces a symbol table.
Second pass - thoroughly checks the correctness of the code written in the file (according to the assembly).
Creating the files - creates all the necessary files, depending on the file (external file, entry file, output file).

Also, there are definitions of data structures, use of constant files and more.
