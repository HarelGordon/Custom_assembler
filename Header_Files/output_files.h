#ifndef OUTPUT_FILES_H
#define OUTPUT_FILES_H

#include <stdio.h>

#include "../Header_Files/node.h"
#include "../Header_Files/word_rep.h"

/**
 * Creates an object file from the given arrays.
 * @param I_array - the array of instruction words
 * @param D_array - the array of data words
 * @param IC - the number of instruction words
 * @param DC - the number of data words
 * @param input_file_name - the name of the input file
 * @return 0 if the file was created successfully, 1 otherwise
 **/
int create_object_file(Word_Rep *I_array, Word_Rep *D_array, int IC, int DC, char *input_file_name);

/**
 * Creates an extern file from the given symbol table.
 * @param symbol_table - the symbol table
 * @param input_file_name - the name of the input file
 * @return 0 if the file was created successfully, 1 otherwise
 **/
int create_extern_file(Node **symbol_table, char *input_file_name);

/**
 * Creates an entry file from the given symbol table.
 * @param symbol_table - the symbol table
 * @param input_file_name - the name of the input file
 * @return 0 if the file was created successfully, 1 otherwise
 **/
int create_entry_file(Node **symbol_table, char *input_file_name);

/**
 * Creates the output files for the given arrays and symbol table.
 * @param I_array - the array of instruction words
 * @param D_array - the array of data words
 * @param IC - the number of instruction words
 * @param DC - the number of data words
 * @param input_file_name - the name of the input file
 * @param symbol_table - the symbol table
 * @return 0 if the files were created successfully, 1 otherwise
 **/
int create_output_files (Word_Rep *I_array, Word_Rep *D_array, int IC, int DC, char *input_file_name, Node **symbol_table) ;

#endif /* OUTPUT_FILES_H */