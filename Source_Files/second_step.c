#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "../Header_Files/globals.h"
#include "../Header_Files/node.h"
#include "../Header_Files/word_rep.h"
#include "../Header_Files/first_step.h"


void update_symbol_table (Node **symbol_table, int IC) {
    Node *temp = *symbol_table; /* Initialize a temporary pointer to traverse the symbol table */
    while (temp) {
        /* Check if the symbol is of type "code" */
        if ((strcmp(temp->content, "code")) == 0) {
            temp->value += 100; /* Update the value by adding 100 */
        }
        /* Check if the symbol is of type "data" */
        else if (strcmp(temp->content, "data") == 0) 
            temp->value = temp->value + IC + 100; /* Update the value by adding IC and 100 */
        /* Check if the symbol is of type "string" */
        else if (strcmp(temp->content, "string") == 0) 
            temp->value = temp->value + IC + 100; /* Update the value by adding IC and 100 */
        temp = temp->next; /* Move to the next symbol in the table */
    }
}

int update_external_in_symbol_table(Node **symbol_table, char *word, int index, char *argv, int line_number) {
    Node *temp = *symbol_table; /* Initialize a temporary pointer to traverse the symbol table */
    Node *new_node = NULL; /* Initialize a pointer for the new node */
    while (temp) {
        /* Check if the symbol name matches the word and its value is -1 */
        if (strcmp(temp->name, word) == 0 && temp->value == -1) {
            allocate_node(&new_node, word, argv, line_number); /* Allocate a new node */
            new_node->value = index + 100; /* Set the value of the new node */
            strcpy(new_node->content, "external"); /* Set the content of the new node to "external" */
            add_node(symbol_table, new_node); /* Add the new node to the symbol table */
            break; /* Exit the loop after updating the symbol */
        }
        temp = temp->next; /* Move to the next symbol in the table */
    }
    return 0; /* Return 0 to indicate success */
}

int update_entry_in_symbol_table(Node **symbol_table, char *word, char *argv, int line_number) {
    Node *temp = *symbol_table; /* Pointer to traverse the symbol table */
    
    while (temp) {
        /* Check if the symbol name matches the word and its value is -1 */
        if ((strcmp(temp->name, word) == 0) && (temp->value == -1)) {
            fprintf(stderr, "ERROR: Entry label Undefined (file - %s, line - %d)\n", argv, line_number);
            return 1; /* Return 1 to indicate an error */
        }
        /* Check if the symbol name matches the word */
        if (strcmp(temp->name, word) == 0) {
            strcpy(temp->content, "entry"); /* Update the content to "entry" */
            return 0; /* Return 0 to indicate success */
        }
        temp = temp->next; /* Move to the next symbol */
    }
    
    /* Print an error message if the entry label is not found */
    fprintf(stderr, "ERROR: Entry label Undefined (file - %s, line - %d)\n", argv, line_number);
    return 1; /* Return 1 to indicate an error */
}

void remove_old_external_labels(Node **symbol_table) {
    Node *temp = *symbol_table; /* Pointer to traverse the symbol table */
    Node *prev = NULL; /* Pointer to keep track of the previous node */
    
    while (temp) {
        /* Check if the symbol value is -1 (old external label) */
        if (temp->value == -1) {
            /* Remove the node from the list */
            if (prev) {
                prev->next = temp->next;
                free(temp); /* Free the memory */
                temp = prev->next; /* Move to the next node */
            } else {
                *symbol_table = temp->next;
                free(temp); /* Free the memory */
                temp = *symbol_table; /* Move to the next node */
            }
        } else {
            prev = temp; /* Update the previous node pointer */
            temp = temp->next; /* Move to the next node */
        }
    }
}

int temp_symbol_and_symbol_table_compare(Node **symbol_table, Node **temp_symbol_table, char *argv) {
    Node *temp = *temp_symbol_table; /* Pointer to traverse the temporary symbol table */
    Node *temp2 = *symbol_table; /* Pointer to traverse the main symbol table */
    int error = 0; /* Error flag */

    while (temp) {
        /* If the main symbol table is empty */
        if (!temp2) {
            fprintf(stderr, "ERROR: Undefined label \"%s\" (file - %s, line - %d)\n", temp->name, argv, temp->value);
            error = 1; /* Set error flag */
        }
        /* Traverse the main symbol table */
        while (temp2) {
            /* Check if the names match */
            if (strcmp(temp->name, temp2->name) == 0)
                break; /* Exit the loop if a match is found */
            /* If end of the main symbol table is reached without a match */
            if (temp2->next == NULL) {
                fprintf(stderr, "ERROR: Undefined label \"%s\" (file - %s, line - %d)\n", temp->name, argv, temp->value);
                error = 1; /* Set error flag */
            }
            temp2 = temp2->next; /* Move to the next symbol */
        }
        temp2 = *symbol_table; /* Reset the main symbol table pointer */
        temp = temp->next; /* Move to the next symbol in the temporary table */
    }
    return error; /* Return the error flag */
}

int second_step(FILE *input_file, Node **symbol_table, Node **temp_symbol_table, Word_Rep *I_array, Word_Rep *D_array, int IC, char *argv) {
    int DC = 0, reg_flag = 0, external_flag = 0, entry_flag = 0, IC_flag = 0, DC_flag = 0;
    int line_index = 0, line_number = 0, error = 0;
    char ch;
    char line[MAX_LINE_LENGTH], deli[] = " \t,", *word;

    /* Update symbol table with IC */
    update_symbol_table(symbol_table, IC);
    IC = 0;

    /* Compare temporary symbol table with main symbol table - check whether label shows on temp symbol_table, but not in symbol table */
    if (temp_symbol_and_symbol_table_compare(symbol_table, temp_symbol_table, argv))
        return 1; /* Return if there is an error in symbol table comparison */
    
    /* Read the input file character by character */
    while ((ch = fgetc(input_file)) != EOF) {
        if (ch == '\n') {
            line_number++; /* Increment line number */
            line[line_index] = '\0'; /* Null-terminate the line */
            word = strtok(line, deli); /* Tokenize the line */
            while (word) {
                /* Determine the type of the word */
                switch (get_word_type(word)) {
                    case ORDER:
                        IC_flag = 1; /* Set IC flag */
                        IC++; /* Increment IC */
                        break;
                    case DIRECTIVE:
                        if ((strcmp(word, ".data") == 0) || (strcmp(word, ".string") == 0))
                            DC_flag = 1; /* Set DC flag */
                        if (strcmp(word, ".extern") == 0) external_flag = 1; /* Set external flag */
                        if (strcmp(word, ".entry") == 0) entry_flag = 1; /* Set entry flag */
                        break;
                    case NUMBER:
                        if (IC_flag) IC++; /* Increment IC if IC flag is set */
                        else if (DC_flag) DC++; /* Increment DC if DC flag is set */
                        break;
                    case REGULAR_NUMBER:
                        DC++; /* Increment DC */
                        break;
                    case LABEL:
                        if (IC_flag) {
                            I_array[IC++] = create_label_rep(word, symbol_table); /* Create label representation in I_array */
                            if (update_external_in_symbol_table(symbol_table, word, IC - 1, argv, line_number)) { /* Check if it's an external label and add to symbol table */
                                error = 1; /* Set error flag */
                                break;
                            }
                        } else if (DC_flag) {
                            D_array[DC++] = create_label_rep(word, symbol_table); /* Create label representation in D_array */
                            if (update_external_in_symbol_table(symbol_table, word, DC - 1, argv, line_number)) { /* Check if it's an external label and add to symbol table */
                                error = 1; /* Set error flag */
                                break;
                            }
                        }
                        if (entry_flag) 
                            if (update_entry_in_symbol_table(symbol_table, word, argv, line_number)) { /* Update entry in symbol table */
                                error = 1; /* Set error flag */
                                break;
                            }
                        break;
                    case STRING:
                        DC += strlen(word) - 1; /* Increment DC by the length of the string */
                        word = NULL; /* Set word to NULL */
                        break;
                    case REGISTER_POINTER:
                    case REGISTER:
                        if (!reg_flag) {
                            reg_flag = 1; /* Set register flag */
                            if (IC_flag) IC++; /* Increment IC if IC flag is set */
                            else if (DC_flag) DC++; /* Increment DC if DC flag is set */
                        }
                        break;
                    case LABEL_DEF:
                    case ERROR:
                    default:
                        break;
                }
                if (error) break; /* Break if there is an error */
                if (word)
                    word = strtok(NULL, deli); /* Get the next token */
            }
            /* Reset flags and line index */
            line_index = IC_flag = DC_flag = reg_flag = external_flag = entry_flag = 0;
        } else
            line[line_index++] = ch; /* Add character to line */
    }
    remove_old_external_labels(symbol_table); /* Remove old external labels */
    return error; /* Return error flag */
}