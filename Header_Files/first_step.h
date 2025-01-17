#ifndef FIRT_STEP_H
#define FIRT_STEP_H

#include <stdio.h>

#include "node.h"
#include "globals.h"
#include "word_rep.h"

/* void print_array_as_binary(Word_Rep *array, int size); */
/* void print_word_as_binary(FILE *output_file, Word_Rep word); */

/**
 * Prepares the first step of the assembly process.
 * 
 * @param temp_node The temporary node.
 * @param input_file The input file.
 * @param argv The command line arguments.
 * @return 1 if an error is found, 0 otherwise.
 */
int first_step_prepers(Node **temp_node, FILE **input_file, char *argv);

/**
 * Checks for comma-related errors in a line of assembly code.
 * 
 * @param line The line of code to check.
 * @param file_name The name of the file being processed.
 * @param line_number The current line number in the file.
 * @return 1 if an error is found, 0 otherwise.
 */
int check_comma (char *line, char *file_name, int line_number);

/**
 * Checks if a string is a number.
 * 
 * @param word The string to check.
 * @return 1 if the string is a number, 0 otherwise.
 */
int isNumber(char *word);

/**
 * Checks if the opcode is in the second instruction set.
 * 
 * @param opcode The opcode to check.
 * @return 1 if the opcode is in the second instruction set, 0 otherwise.
 */
int is_in_second_instruction_set(int opcode);

/**
 * Checks if the opcode is in the third instruction set.
 * 
 * @param opcode The opcode to check.
 * @return 1 if the opcode is in the third instruction set, 0 otherwise.
 */
int is_in_therd_instruction_set(int opcode);

/**
 * Checks if a label is legal.
 * 
 * @param word The label to check.
 * @param macro_table The macro table.
 * @param symbol_table The symbol table.
 * @param argv The command line arguments.
 * @param line_number The current line number in the file.
 * @return 1 if the label is legal, 0 otherwise.
 */
int legal_label(char *word, Node **macro_table, Node **symbol_table, char *argv, int line_number);

/**
 * Checks if a string is valid.
 * 
 * @param line The string to check.
 * @param argv The command line arguments.
 * @param line_number The current line number in the file.
 * @return 1 if the string is valid, 0 otherwise.
 */
int valid_string (char *line, char *argv, int line_number);

/**
 * Checks if a number is valid.
 * 
 * @param word The number to check.
 * @param argv The command line arguments.
 * @param line_number The current line number in the file.
 * @return 1 if the number is valid, 0 otherwise.
 */
int valid_num (char *word, char *argv, int line_number);

/**
 * Checks for errors before the while loop.
 * 
 * @param des_flag The destination flag.
 * @param argv The command line arguments.
 * @param line_number The current line number in the file.
 * @param entry_flag The entry flag.
 * @param external_flag The external flag.
 * @param word_count The current word count.
 * @param LABEL_flag The label flag.
 * @param word The current word.
 * @return 1 if an error is found, 0 otherwise.
 */
int check_error_word_before_while (int des_flag, char *argv, int line_number, int entry_flag, int external_flag, int word_count, int LABEL_flag, char *word);

/**
 * Gets the opcode of an order.
 * 
 * @param word The order to check.
 * @return The opcode of the order.
 */
int get_opcode(char *word);

/**
 * Gets the type of a word.
 * 
 * @param word The word to check.
 * @return The type of the word.
 */
enum words get_word_type(char *word);

/**
 * Updates the content of a node.
 * 
 * @param temp_node The node to update.
 * @param word The new content for the node.
 */
void update_node_content(Node *temp_node, char *word);

/**
 * Handles the case of a register.
 * 
 * @param I_array The instruction array.
 * @param D_array The data array.
 * @param word The register to handle.
 * @param source_operand The source operand.
 * @param des_operand The destination operand.
 * @param source_flag The source flag.
 * @param reg_flag The register flag.
 * @param IC_flag The instruction counter flag.
 * @param DC_flag The data counter flag.
 * @param IC The instruction counter.
 * @param DC The data counter.
 */
void reg_case(Word_Rep *I_array, Word_Rep *D_array, char *word, Word_Rep *source_operand, Word_Rep *des_operand, int *source_flag, int *reg_flag, int *IC_flag, int *DC_flag, int *IC, int *DC);

/**
 * Performs the first step of the assembly process.
 * 
 * @param input_file The input file.
 * @param macro_table The macro table.
 * @param symbol_table The symbol table.
 * @param temp_symbol_table The temporary symbol table.
 * @param temp_node The temporary node.
 * @param I_array The instruction array.
 * @param D_array The data array.
 * @param IC The instruction counter.
 * @param DC The data counter.
 * @param argv The command line arguments.
 * @return 1 if an error is found, 0 otherwise.
 */
int first_step(FILE *input_file, Node **macro_table, Node **symbol_table, Node **temp_symbol_table, Node **temp_node, Word_Rep *I_array, Word_Rep *D_array, int *IC, int *DC, char *argv);

#endif /* FIRT_STEP.H */
