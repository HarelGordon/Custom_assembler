#ifndef MACRO_STEP_H
#define MACRO_STEP_H

#include "node.h"

/**
 * Prepares the macro step by opening the input file, creating the output file, and allocating memory for a temporary node.
 * @param input_file - pointer to the input file
 * @param output_file - pointer to the output file
 * @param argv - the name of the file
 * @param temp_node - pointer to the temporary node
 * @return 1 if the preparation was successful, 0 otherwise
 */
int prepers_macro_step(FILE **input_file, FILE **output_file, char *argv, Node **temp_node);

/**
 * Checks if a word is an order.
 * @param word - the word to check
 * @return 1 if the word is an order, 0 otherwise
 */
int word_is_order(char *word);

/**
 * Checks if a word is a directive.
 * @param word - the word to check
 * @return 1 if the word is a directive, 0 otherwise
 */
int word_is_directive(char *word);

/**
 * Checks if a word is a register.
 * @param word - the word to check
 * @return 1 if the word is a register, 0 otherwise
 */
int word_is_register(char *word);

/**
 * Loops until the end of the file, reading the input file line by line and writing to the output file.
 * @param input_file - pointer to the input file
 * @param output_file - pointer to the output file
 * @param temp_node - pointer to the temporary node
 * @param macro_table - pointer to the macro table
 * @param argv - the name of the file
 * @return 1 if there was an error, 0 otherwise
 */
int loop_until_eof(FILE *input_file, FILE *output_file, Node *temp_node, Node **macro_table, char *argv);

#endif /* MACRO_STEP.H */
