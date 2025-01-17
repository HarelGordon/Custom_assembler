#ifndef SECOND_STEP_H
#define SECOND_STEP_H

#include <stdio.h>

#include "../Header_Files/node.h"
#include "../Header_Files/word_rep.h"

/**
 * Updates the symbol table.
 * @param symbol_table - the symbol table
 * @param IC - the number of instruction words
 **/
void update_symbol_table (Node **symbol_table, int IC);

/**
 * Updates the symbol table with external labels.
 * @param symbol_table - the symbol table
 * @param word - the label to update
 * @param index - the index of the label
 * @param argv - the command line arguments
 * @param line_number - the current line number
 * @return 0 if the label was updated successfully, 1 otherwise
 **/
int update_external_in_symbol_table(Node **symbol_table, char *word, int index, char *argv, int line_number);

/**
 * Updates the symbol table with entry labels.
 * @param symbol_table - the symbol table
 * @param word - the label to update
 * @param argv - the command line arguments
 * @param line_number - the current line number
 * @return 0 if the label was updated successfully, 1 otherwise
 **/
int update_entry_in_symbol_table(Node **symbol_table, char *word, char *argv, int line_number);

/**
 * Removes old external labels from the symbol table.
 * @param symbol_table - the symbol table
 **/
void remove_old_external_labels(Node **symbol_table);

/**
 * Compares the temporary symbol table with the symbol table.
 * @param symbol_table - the symbol table
 * @param temp_symbol_table - the temporary symbol table
 * @param argv - the command line arguments
 * @return 1 if an error is found, 0 otherwise
 **/
int temp_symbol_and_symbol_table_compare(Node **symbol_table, Node **temp_symbol_table, char *argv);

/**
 * The second step of the assembly process.
 * @param input_file - the input file
 * @param symbol_table - the symbol table
 * @param temp_symbol_table - the temporary symbol table
 * @param I_array - the instruction array
 * @param D_array - the data array
 * @param IC - the number of instruction words
 * @param argv - the command line arguments
 * @return 1 if an error is found, 0 otherwise
 **/
int second_step(FILE *input_file, Node **symbol_table, Node **temp_symbol_table, Word_Rep *I_array, Word_Rep *D_array, int IC, char *argv);

#endif /* SECOND_STEP.H */