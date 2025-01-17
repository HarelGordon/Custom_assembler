#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../Header_Files/globals.h"
#include "../Header_Files/node.h"
#include "../Header_Files/macro_step.h"


int prepers_macro_step(FILE **input_file, FILE **output_file, char *input_filename, Node **temp_node) {
    char *output_filename = NULL;
    
    /* Try to open the input file in read mode */
    if (!(*input_file = fopen(input_filename, "r"))) {
        fprintf(stderr, "ERROR: File %s does not exist\n", input_filename);
        return 0; /* Return 0 if the file does not exist */
    }
    fclose(*input_file); /* Close the input file */
    
    /* Try to open the input file in append mode */
    if (!(*input_file = fopen(input_filename, "a"))) {
        fprintf(stderr, "ERROR: Can't open file %s\n", input_filename);
        return 0; /* Return 0 if the file cannot be opened */
    }
    
    /* Add a newline character to the end of the file */
    if (fputc('\n', *input_file) == EOF) {
        fprintf(stderr, "ERROR: Failed to add newline character to the end of the file (file - %s)\n", input_filename);
        return 0; /* Return 0 if the newline character cannot be added */
    }
    fclose(*input_file); /* Close the input file */
    
    /* Try to open the input file in read mode again */
    if (!(*input_file = fopen(input_filename, "r"))) {
        fprintf(stderr, "ERROR: Can't open file %s\n", input_filename);
        return 0; /* Return 0 if the file cannot be opened */
    }
    
    /* Allocate memory for the output filename */
    output_filename = (char *)malloc((strlen(input_filename) + 1) * sizeof(*output_filename));
    if (!output_filename) {
        fprintf(stderr, "ERROR: Memory allocation failed (file - %s)\n", input_filename);
        return 0; /* Return 0 if memory allocation fails */
    }
    
    /* Copy the input filename to the output filename and change the extension */
    strcpy(output_filename, input_filename);
    output_filename[strlen(input_filename) - 1] = 'm'; /* Change 'as' to 'am' */
    
    /* Try to create the output file in write mode */
    if (!(*output_file = fopen(output_filename, "w"))) {
        fprintf(stderr, "ERROR: Can't create output file %s\n", output_filename);
        free(output_filename); /* Free the allocated memory */
        return 0; /* Return 0 if the output file cannot be created */
    }
    
    /* Allocate memory for the temporary node */
    *temp_node = malloc(sizeof(Node));
    if (!(*temp_node)) {
        fprintf(stderr, "ERROR: Memory allocation failed (file - %s)\n", input_filename);
        free(output_filename); /* Free the allocated memory */
        return 0; /* Return 0 if memory allocation fails */
    }
    
    (*temp_node)->name = NULL; /* Initialize the name of the temporary node to NULL */
    (*temp_node)->content = (char *)malloc(MAX_LINE_LENGTH * sizeof(char)); /* Allocate memory for the content of the temporary node */
    if (!(*temp_node)->content) {
        fprintf(stderr, "ERROR: Memory allocation failed (file - %s)\n", input_filename);
        free(output_filename); /* Free the allocated memory */
        free(*temp_node); /* Free the temporary node */
        return 0; /* Return 0 if memory allocation fails */
    }
    
    (*temp_node)->content[0] = '\0'; /* Initialize the content of the temporary node to an empty string */
    (*temp_node)->next = NULL; /* Initialize the next pointer of the temporary node to NULL */
    
    free(output_filename); /* Free the allocated memory for the output filename */
    return 1; /* Return 1 to indicate success */
}

int word_is_order(char *word) {
    int i;
    /* Loop through the array of orders */
    for (i = 0; i < 16; i++) {
        /* Compare the given word with the current order */
        if (strcmp(word, order[i]) == 0)
            /* If the word matches an order, return 1 */
            return 1;
    }
    /* If no match is found, return 0 */
    return 0;
}

int word_is_directive(char *word) {
    int i;
    /* Loop through the array of directives */
    for (i = 0; i < 4; i++) {
        /* Compare the given word with the current directive */
        if (strcmp(word, directive[i]) == 0)
            /* If the word matches a directive, return 1 */
            return 1;
    }
    /* If no match is found, return 0 */
    return 0;
}

int word_is_register(char *word) {
    int i;
    /* Loop through the array of register names */
    for (i = 0; i < 8; i++) {
        /* Compare the given word with the current register name */
        if (strcmp(word, register_names[i]) == 0)
            /* If the word matches a register name, return 1 */
            return 1;
    }
    /* If no match is found, return 0 */
    return 0;
}

int loop_until_eof(FILE *input_file, FILE *output_file, Node *temp_node, Node **macro_table, char *argv) {
    int macro_flag = 0, line_index = 0, out_line_count = 0, words_count = 0, line_number = 0, error = 0;
    char ch;
    char line[MAX_LINE_LENGTH], temp_line[MAX_LINE_LENGTH], deli[] = " \t", *word;

    /* Read characters from the input file until EOF is reached */
    while ((ch = fgetc(input_file)) != EOF) {
        /* Check if the character is a newline */
        if (ch == '\n') {
            line_number++; /* Increment the line number */
            line[line_index] = '\0'; /* Null-terminate the current line */
            strcpy(temp_line, line);  /* Copy the line to temp_line */
            word = strtok(line, deli);  /* Tokenize the line using delimiters */

            /* Check if there is a word in the line */
            if (word) {
                words_count++; /* Increment the word count */

                /* Check if the first word is a comment */
                if ((words_count == 1) && (word[0] == ';'))
                    word = NULL; /* Ignore the comment line */
                else if (is_in(macro_table, word)) { /* Check if the word is a macro */
                    if (words_count == 1)
                        /* If it is, write its content to the output file */
                        fprintf(output_file, "%s\n", is_in(macro_table, word));
                    else {
                        /* If the word is already defined as a macro, print an error message */
                        fprintf(stderr, "ERROR: Already defined as macro (file - %s, line - %d)\n", argv, line_number);
                        error = 1; /* Set the error flag */
                    }
                }
                else if (macro_flag) { /* Check if we are inside a macro definition */
                    if (strcmp(word, "endmacr") == 0) {
                        macro_flag = 0;  /* Reset macro_flag */
                        /* Prepare the macro content by removing the last newline character */
                        temp_node->content[strlen(temp_node->content) - 1] = '\0';
                        /* Add the macro to the macro table */
                        add_node(macro_table, temp_node);
                        /* Check if there are extra tokens after "endmacr" */
                        if ((word = strtok(NULL, deli))) {
                            fprintf(stderr, "ERROR: After endmacr there is something (file - %s, line - %d)\n", argv, line_number);
                            error = 1; /* Set the error flag */
                        }
                    }
                    else {
                        /* Append the line to the macro content */
			if (temp_node->content[0] == '\0')
				strcpy(temp_node->content, temp_line);
			else {
				temp_node->content = (char *) realloc(temp_node->content, strlen(temp_node->content) + MAX_LINE_LENGTH);
				strcat(temp_node->content, temp_line);
			}
                        strcat(temp_node->content, "\n"); /* Add a newline character */
                    }
                }
                else if (strcmp(word, "macr") == 0) { /* Check if the word is the start of a macro definition */
                    word = strtok(NULL, deli); /* Get the macro name */
                    if (!word) {
                        /* If there is no macro name, print an error message */
                        fprintf(stderr, "ERROR: After macr there is nothing (file - %s, line - %d)\n", argv, line_number);
                        error = 1; /* Set the error flag */
                    }
                    else {
                        /* Check if the macro name is valid */
                        if (word_is_order(word)) {
                            fprintf(stderr, "ERROR: Macro name can't be order (file - %s, line - %d)\n", argv, line_number);
                            error = 1; /* Set the error flag */
                        }
                        else if (word_is_directive(word)) {
                            fprintf(stderr, "ERROR: Macro name can't be directive (file - %s, line - %d)\n", argv, line_number);
                            error = 1; /* Set the error flag */
                        }
                        else if (word_is_register(word)) {
                            fprintf(stderr, "ERROR: Macro name can't be register (file - %s, line - %d)\n", argv, line_number);
                            error = 1; /* Set the error flag */
                        }
                        else if (is_in(macro_table, word)) {
                            fprintf(stderr, "ERROR: Macro name already exists (file - %s, line - %d)\n", argv, line_number);
                            error = 1; /* Set the error flag */
                        }
                        else if (strcmp(word, "macr") == 0) {
                            fprintf(stderr, "ERROR: Macro name can't be macr (file - %s, line - %d)\n", argv, line_number);
                            error = 1; /* Set the error flag */
                        }
                        else {
                            /* Allocate a new node for the macro */
                            allocate_node(&temp_node, word, argv, line_number);
                            macro_flag = 1; /* Set the macro_flag */
                            word = strtok(NULL, deli); /* Check for extra tokens */
                            if (word) {
                                fprintf(stderr, "ERROR: After macro name there is something (file - %s, line - %d)\n", argv, line_number);
                                error = 1; /* Set the error flag */
                            }
                        }
                    }
                }
                else {
                    /* Write the line to the output file */
                    fprintf(output_file, "%s\n", temp_line);
                    out_line_count++; /* Increment the output line count */
                }
            }
            /* Reset line_index and words_count for the next line */
            line_index = words_count = 0;
        }
        else {
            /* Check if the line length exceeds the maximum allowed length */
            if (line_index > 81) {
                line_number++; /* Increment the line number */
                fprintf(stderr, "ERROR: Line is too long (file - %s, line - %d)\n", argv, line_number);
                error = 1; /* Set the error flag */
                /* Skip the rest of the line */
                while ((ch = fgetc(input_file)) != '\n')
                    ;
                line_index = 0; /* Reset line_index */
            }
            else
                line[line_index++] = ch;  /* Add the character to the line */
        }
    }

	fclose(output_file); /* Close the output file */

    /* If there are no lines or there is an error, remove the file */
	argv[strlen(argv) - 1] = 'm';
    if ((out_line_count == 0) || (error))
        remove(argv);
    return error; /* Return the error status */
}

