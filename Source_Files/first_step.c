#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "../Header_Files/globals.h"
#include "../Header_Files/node.h"
#include "../Header_Files/word_rep.h"
#include "../Header_Files/macro_step.h"
#include "../Header_Files/first_step.h"

int first_step_prepers(Node **temp_node, FILE **input_file, char *argv) {
    char *input_filename = NULL;
	argv[strlen(argv) - 1] = 'm';
    /* Attempt to open the input file in read mode. */
    if (!(*input_file = fopen(argv, "r"))) {
        fprintf(stderr, "Can't open file %s\n", argv);
        return 0;
    }

    /* Free the allocated memory for input_filename. */
    free(input_filename);

    return 1;
}

int check_comma(char *line, char *file_name, int line_number) {
    int i, j = 1;
    int word_count = 0, label_def_flag = 0, apostrophe = 0, dont_count_words_flag = 0, count_non_blank_char = 0;
    char *temp_line = NULL, *word = NULL;
    char deli[] = " \t";
    char temp_ch;

    /* Check if the command ends with a comma by skipping trailing spaces and tabs */
/*     while ((line[strlen(line) - j] == ' ') || (line[strlen(line) - j] == '\t'))
        j++;
 */
    /* Check for consecutive commas in the line */
    for (i = 0; i < strlen(line) - 1; i++) {
        if (line[i] == ',') {
            /* Skip spaces after a comma */
            while (isspace(line[i + 1]))
                i++;
            /* If the next character is also a comma, report an error */
            if (line[i + 1] == ',') {
                fprintf(stderr, "ERROR: Consecutive commas (file - %s, line - %d)\n", file_name, line_number);
                return 1;
            }
        }
    }

    /* Allocate memory for a temporary copy of the line */
    temp_line = (char *)malloc(strlen(line) + 1);
    if (temp_line == NULL) {
        fprintf(stderr, "ERROR: Memory allocation failed (file - %s, line - %d)\n", file_name, line_number);
        return 1;
    }

    /* Check the spacing between the command and the first operand/data */
    strcpy(temp_line, line);  /* Copy the line into temp_line */
    word = strtok(temp_line, deli);  /* Tokenize the line using spaces and tabs as delimiters */
    while (word) {
        word_count++;  /* Increment the word count */
        /* Check if the word is a label definition */
        if (word[strlen(word) - 1] == ':') {
            label_def_flag = 1;
        }
        /* Check if there is a comma after the first word */
        if ((word[strlen(word) - 1] == ',') && (word_count == 1)) {
            fprintf(stderr, "ERROR: After first word there is comma (file - %s, line - %d)\n", file_name, line_number);
            free(temp_line);  /* Free allocated memory */
            return 1;
        }
        /* Check if there is a comma at the beginning of the second word */
        if ((word[0] == ',') && (word_count == 2)) {
            fprintf(stderr, "ERROR: After first word there is comma (file - %s, line - %d)\n", file_name, line_number);
            free(temp_line);  /* Free allocated memory */
            return 1;
        }
        /* Check if there is a comma between label definition and instruction */
        if ((word_count == 1) && (label_def_flag == 1)) {
            if (word[0] == ',') {
                fprintf(stderr, "ERROR: Between label definition and order/instruction, must be only spaces or tabs (file - %s, line - %d)\n", file_name, line_number);
                free(temp_line);  /* Free allocated memory */
                return 1;
            }
        }
        /* Check if there is a comma between label definition and instruction */
        if (((word_count == 1) && (label_def_flag == 0)) || ((word_count == 2) && (label_def_flag == 1))) {
            if (word[0] == ',') {
                fprintf(stderr, "ERROR: Between label definition and order/instruction, must be only spaces or tabs (file - %s, line - %d)\n", file_name, line_number);
                free(temp_line);  /* Free allocated memory */
                return 1;
            }
        }
        word = strtok(NULL, deli);  /* Get the next token */
    }

    /* Reset flags and counters for the next check */
    label_def_flag = word_count = apostrophe = dont_count_words_flag = count_non_blank_char = 0;

    /* Check for missing commas */
    for (i = 0; i < strlen(line); i++) {
        if (line[i] == ':') {
            label_def_flag = 1;  /* Set flag if a label definition is found */
        }
        if (line[i] == '"') {
            apostrophe++;  /* Count apostrophes to handle string literals */
        }
        if ((line[i] == '"') && (apostrophe % 2 == 1)) {
            /* Inside a string literal, skip until the closing quote */
            while (line[i + 1] != '"') {
                if (line[i + 1] == '\0') {
                    /* If the closing quote is missing, report an error */
                    fprintf(stderr, "ERROR: Missing \" (file - %s, line - %d)\n", file_name, line_number);
                    free(temp_line);  /* Free allocated memory */
                    return 1;
                }
                i++;
            }
        }
        if (!dont_count_words_flag) {
            /* Check for word boundaries and count words */
            if (((word_count >= 2) && (label_def_flag == 1)) || ((word_count >= 1) && (label_def_flag == 0))) {
                if ((!isspace(line[i])) && ((isspace(line[i + 1])) || (line[i + 1] == ','))) {
                    /* Skip spaces and count words */
                    while (isspace(line[i + 1])) {
                        i++;
                    }
                    if (line[i + 1] != '\0') {
                        word_count++;
                    }
                }
            } else if ((!isspace(line[i])) && (isspace(line[i + 1]))) {
                /* Skip spaces and count words */
                while (isspace(line[i + 1])) {
                    i++;
                }
                if (line[i + 1] != '\0') {
                    word_count++;
                }
            }
        }
        /* Check for missing commas between operands */
        if ((((word_count >= 2) && (label_def_flag == 0)) || ((word_count >= 3) && (label_def_flag == 1))) && (isspace(line[i]))) {
            dont_count_words_flag = 1;  /* Stop counting words */
            temp_ch = line[i - 1];  /* Store the previous character */
            while (isspace(line[i])) {
                i++;
            }
            if ((line[i] == '\0') && (temp_ch == ',')) {
                /* If the line ends with a comma, report an error */
                fprintf(stderr, "ERROR: Line ends with comma (file - %s, line - %d)\n", file_name, line_number);
                free(temp_line);  /* Free allocated memory */
                return 1;
            }
            if (line[i] == '\0') {
                free(temp_line);  /* Free allocated memory */
                return 0;
            }
            if ((line[i] == ',') && (temp_ch == ',')) {
                /* If there are consecutive commas, report an error */
                fprintf(stderr, "ERROR: Consecutive commas (file - %s, line - %d)\n", file_name, line_number);
                free(temp_line);  /* Free allocated memory */
                return 1;
            }
            if ((line[i] != ',') && (temp_ch != ',')) {
                /* If a comma is missing between operands, report an error */
                fprintf(stderr, "ERROR: Missing comma between operands (file - %s, line - %d)\n", file_name, line_number);
                free(temp_line);  /* Free allocated memory */
                return 1;
            }
        }
    }

    /* Check for spaces and tabs between operands separated by commas */
    for (i = 0; i < strlen(line); i++) {
        if (line[i] == ',') {
            j = 1;
            count_non_blank_char = 0;  /* Reset the counter for non-blank characters */
            while (line[i + j] != ',') {
                if ((line[i + j] != ' ') && (line[i + j] != '\t')) {
                    count_non_blank_char++;  /* Count non-blank characters */
                }
                if ((i + j) == strlen(line) - 1) {
                    break;  /* Break if the end of the line is reached */
                }
                j++;
            }
            if (count_non_blank_char == 0) {
                /* If there are only blank characters between commas, report an error */
                fprintf(stderr, "ERROR: You entered comma, blank characters, and comma again (file - %s, line - %d)\n", file_name, line_number);
                free(temp_line);  /* Free allocated memory */
                return 1;
            }
        }
    }

    j = 1;
    /* Check if the line ends with a comma */
    while (isspace(line[strlen(line) - j])) 
        j++;
    if (line[strlen(line) - j] == ',') {
        /* If the line ends with a comma, report an error */
        fprintf(stderr, "ERROR: Line ends with comma (file - %s, line - %d)\n", file_name, line_number);
        free(temp_line);  /* Free allocated memory */
        return 1;
    }

    free(temp_line);  /* Free allocated memory */
    return 0;
}

int isNumber(char *word) {
/*     Check if the first character is '-' for negative numbers*/
    if (*word == '-') 
        word++; /* Move to the next character */
    /* If the string is empty after skipping '-', it's not a number */
    if (!*word) 
        return 0;
    /* Check each character to ensure it's a digit */
    while (*word) {
        if (*word < '0' || *word > '9') {
            return 0; /* Not a number if any character is not a digit */
        }
        word++; 
    }
    return 1;
}

int is_in_second_instruction_set(int opcode) {
    /* Return true if the opcode matches any value in the second instruction set */
    return (opcode == 5 || opcode == 6 || opcode == 7 || opcode == 8 || opcode == 9 || opcode == 10 || opcode == 11 || opcode == 12 || opcode == 13);
}

int is_in_therd_instruction_set(int opcode) {
    /* Return true if the opcode matches any value in the third instruction set */
    return (opcode == 14 || opcode == 15);
}

int legal_label(char *word, Node **macro_table, Node **symbol_table, char *argv, int line_number) {
    int j = 0;
    /* Check if the label length exceeds the maximum allowed length */
    if (strlen(word) > MAX_LABEL_LENGTH) {
        fprintf(stderr, "ERROR: Label too long (file - %s, line - %d)\n", argv, line_number);
        return 0;  /* Return 0 indicating an invalid label */
    }
    switch (get_word_type(word)) {
        case ORDER:
            fprintf(stderr, "ERROR: Label cannot be order (file - %s, line - %d)\n", argv, line_number);
            return 0;  /* Return 0 indicating an invalid label */
        case DIRECTIVE:
            fprintf(stderr, "ERROR: Label cannot be directive (file - %s, line - %d)\n", argv, line_number);
            return 0;  /* Return 0 indicating an invalid label */
        case REGISTER:
            fprintf(stderr, "ERROR: Label cannot be register (file - %s, line - %d)\n", argv, line_number);
            return 0;  /* Return 0 indicating an invalid label */
        case REGISTER_POINTER:
            fprintf(stderr, "ERROR: Label cannot be register pointer (file - %s, line - %d)\n", argv, line_number);
            return 0;  /* Return 0 indicating an invalid label */
        case REGULAR_NUMBER:
            fprintf(stderr, "ERROR: Label cannot be regular number (file - %s, line - %d)\n", argv, line_number);
            return 0;  /* Return 0 indicating an invalid label */
        case LABEL:
        case NUMBER:
        case STRING:
        case ERROR:
        case LABEL_DEF:
            break;
    }
    if (isalpha(word[0]) == 0) {
        fprintf(stderr, "ERROR: Label must start with a letter (file - %s, line - %d)\n", argv, line_number);
        return 0;  /* Return 0 indicating an invalid label */
    }

    while (*(word + j) != '\0') {
        if (isalnum(*(word + j)) == 0) {
            fprintf(stderr, "ERROR: Label must contain only letters and numbers (file - %s, line - %d)\n", argv, line_number);
            return 0;  /* Return 0 indicating an invalid label */
        }
        j++;
    }
    if (is_in(macro_table, word)) {
        fprintf(stderr, "ERROR: Label can not be macro name (file - %s, line - %d)\n", argv, line_number);
        return 0;  /* Return 0 indicating an invalid label */
    }
    return 1;  /* Return 1 if the word is a valid label */
}

int valid_string(char *line, char *argv, int line_number) {
    int i = 0, count = 0;

    /* Iterate through each character in the line */
    while (line[i] != '\0') {
        /* Check if there are non-whitespace characters after the closing quote */
        if ((count == 2) && (!isspace(line[i]))) {
            fprintf(stderr, "ERROR: After string there is something (file - %s, line - %d)\n", argv, line_number);
            return 0;  /* Return 0 indicating an invalid string */
        }
        /* Count the number of double quotes */
        if (line[i] == '"') {
            count++;
        }
        i++;
    }

    /* Check if the string does not end with a closing quote */
    if (count != 2) {
        fprintf(stderr, "ERROR: String not end with \" (file - %s, line - %d)\n", argv, line_number);
        return 0;  /* Return 0 indicating an invalid string */
    }

    return 1;  /* Return 1 indicating a valid string */
}

int valid_num(char *word, char *argv, int line_number) {
    /* Check if the word is not "0" and cannot be converted to a valid integer */
    if ((strcmp(word, "0") != 0) && atoi(word) == 0) {
        fprintf(stderr, "ERROR: Illegal number (file - %s, line - %d)\n", argv, line_number);
        return 1;  /* Return 1 indicating an invalid number */
    }

    /* Check if the number is out of the valid range (-16834 to 16833) */
    if ((atoi(word) < -16834) || (atoi(word) > 16833)) {
        fprintf(stderr, "ERROR: Number out of range (file - %s, line - %d)\n", argv, line_number);
        return 1;  /* Return 1 indicating an invalid number */
    }

    return 0;  /* Return 0 indicating a valid number */
}

int check_error_word_before_while(int des_flag, char *argv, int line_number, int entry_flag, int external_flag, int word_count, int LABEL_flag, char *word) {
    /* Check if there are too many operands */
    if (des_flag) {
        fprintf(stderr, "ERROR: Too many operands (file - %s, line - %d)\n", argv, line_number);
        return 1;  /* Return 1 indicating an error */
    }

    /* Check if there are too many operands for entry or external flags */
    if (((entry_flag) || (external_flag)) && (word_count > 2)) {
        if (word_count > 1) {
            fprintf(stderr, "ERROR: Too many operands (file - %s, line - %d)\n", argv, line_number);
            return 1;  /* Return 1 indicating an error */
        }
    }

    /* Check if the label definition is followed by an invalid word */
    if (LABEL_flag && (word_count == 2) && (strcmp(word, ".string") != 0) && (strcmp(word, ".data") != 0) && (get_word_type(word) != ORDER)) {
        fprintf(stderr, "ERROR: After label definition must be order or .string or .data (file - %s, line - %d)\n", argv, line_number);
        return 1;  /* Return 1 indicating an error */
    }

    return 0;  /* Return 0 indicating no error */
}

int get_opcode(char *word) {
    int i;

    /* Iterate through the list of known instructions */
    for (i = 0; i < 16; i++) {
        /* Compare the given word with the current instruction */
        if (strcmp(word, order[i]) == 0) {
            return i;  /* Return the opcode if a match is found */
        }
    }

    return -1;  /* Return -1 if the instruction is not found */
}

enum words get_word_type(char *word) {
    int i;

    /* Check if the word matches any known order */
    for (i = 0; i < 16; i++) {
        if (strcmp(word, order[i]) == 0)
            return ORDER;  /* Return ORDER if a match is found */
    }

    /* Check if the word matches any known directive */
    for (i = 0; i < 4; i++) {
        if (strcmp(word, directive[i]) == 0)
            return DIRECTIVE;  /* Return DIRECTIVE if a match is found */
    }

    /* Check if the word matches any known register name */
    for (i = 0; i < 8; i++) {
        if (strcmp(word, register_names[i]) == 0)
            return REGISTER;  /* Return REGISTER if a match is found */
    }

    /* Check if the word is a number (starts with '#' and followed by a regular number) */
    if ((word[0] == '#') && (get_word_type(word + 1) == REGULAR_NUMBER))
        return NUMBER;  /* Return NUMBER if the condition is met */

    /* Check if the word is a string (starts with '"') */
    if (word[0] == '"')
        return STRING;  /* Return STRING if the condition is met */

    /* Check if the word is a register pointer (starts with '*' and followed by a register) */
    if ((word[0] == '*') && (get_word_type(word + 1) == REGISTER))
        return REGISTER_POINTER;  /* Return REGISTER_POINTER if the condition is met */

    /* Check if the word is a label definition (ends with ':') */
    if (word[strlen(word) - 1] == ':') 
        return LABEL_DEF;  /* Return LABEL_DEF if the condition is met */

    /* Check if the word is a regular number (is a number or starts with '+'/'-' followed by a number) */
    if (isNumber(word) || (word[0] == '-' && isNumber(word + 1)) || (word[0] == '+' && isNumber(word + 1)))
        return REGULAR_NUMBER;  /* Return REGULAR_NUMBER if the condition is met */

    return LABEL;  /* Return LABEL if none of the above conditions are met */
}

void update_node_content(Node *temp_node, char *word) {
    /* Check if the word is ".data" and update the node content accordingly */
    if (strcmp(word, ".data") == 0)
        strcpy(temp_node->content, "data");
    /* Check if the word is ".string" and update the node content accordingly */
    else if (strcmp(word, ".string") == 0)
        strcpy(temp_node->content, "string");
    /* Check if the word is ".extern" and update the node content accordingly */
    else if (strcmp(word, ".extern") == 0)
        strcpy(temp_node->content, "external");
}

void reg_case(Word_Rep *I_array, Word_Rep *D_array, char *word, Word_Rep *source_operand, Word_Rep *des_operand, int *source_flag, int *reg_flag, int *IC_flag, int *DC_flag, int *IC, int *DC) {
    if (*source_flag) {   /* If source operand already set */
        if (*reg_flag) {   /* If this is the second register */
            if (*IC_flag) {
                /* Update the instruction array with the recreated register representation */
                I_array[(*IC) - 1] = recreate_reg_rep(source_operand, word);
            }
            else if (*DC_flag) {
                /* Update the data array with the recreated register representation */
                D_array[(*DC) - 1] = recreate_reg_rep(source_operand, word);
            }
        }
        else {
            /* Create the destination operand representation */
            *des_operand = create_reg_rep(word, "des");
            if (*IC_flag) {
                /* Update the instruction array with the destination operand */
                I_array[(*IC)++] = *des_operand;
            }
            else if (*DC_flag) {
                /* Update the data array with the destination operand */
                D_array[(*DC)++] = *des_operand;
            }
        }
    }
    else {
        /* Set the source flag and register flag */
        *source_flag = 1;
        *reg_flag = 1;
        /* Create the source operand representation */
        *source_operand = create_reg_rep(word, "source");
        if (*IC_flag) {
            /* Update the instruction array with the source operand */
            I_array[(*IC)++] = *source_operand;
        }
        else if (*DC_flag) {
            /* Update the data array with the source operand */
            D_array[(*DC)++] = *source_operand;
        }
    }
}

int first_step(FILE *input_file, Node **macro_table, Node **symbol_table, Node **temp_symbol_table, Node **temp_node, Word_Rep *I_array, Word_Rep *D_array, int *IC, int *DC, char *argv) { 
    /* Initialize various counters and flags */
    int line_index = 0, word_count = 0, line_number = 0, line_error = 0, prog_error = 0, fatal_error = 0;
    int LABEL_flag = 0, reg_label_flag = 0, reg_flag = 0, data_flag = 0, string_flag = 0, external_flag = 0, entry_flag = 0, source_flag = 0, des_flag = 0, IC_flag = 0, DC_flag = 0, sec_instruct_set_flag = 0, therd_instruct_set_flag = 0;
    int IC_first_word, source_operand_address, des_operand_address, opcode;
    char ch;
    char line[MAX_LINE_LENGTH], temp_line[MAX_LINE_LENGTH], deli[] = " \t,", *word;
    Word_Rep des_operand, source_operand;
    int i;

    /* Initialize instruction and data arrays */
    for (i = 0; i < MAX_WORDS; i++) {
        I_array[i].short_num = 0;
        D_array[i].short_num = 0;
    }

    /* Read the input file character by character */
    while ((ch = fgetc(input_file)) != EOF) {
        if (ch == '\n') {
            /* Increment line number and terminate the current line */
            line_number++;
            line[line_index] = '\0';
            strcpy(temp_line, line);

            /* Check if the total number of words exceeds the limit */
            if (*DC + *IC > 4096) {
                fprintf(stderr, "ERROR: Number of words passed 4096. Exits the program to prevent data overflow  (file - %s)\n", argv);
                fatal_error = -1;
                break;
            }

            /* Check for comma errors in the line */
            if (check_comma(temp_line, argv, line_number))
                line_error = 1;

            /* Reset the line and tokenize it */
            strcpy(temp_line, line);
            word = strtok(line, deli);
            while (word) {
                word_count++;

                /* Check for errors before processing the word */
                if (check_error_word_before_while(des_flag, argv, line_number, entry_flag, external_flag, word_count, LABEL_flag, word)) {
                    line_error = 1;
                    break;
                }

                /* Determine the type of the word and process accordingly */
                switch (get_word_type(word)) {
                    case LABEL_DEF:
                        /* Handle label definition */
                        word[strlen(word) - 1] = '\0';
                            if (is_in(symbol_table, word)) { /* Check if the label already exists */
                                fprintf(stderr, "ERROR: Label already exists (file - %s, line - %d)\n", argv, line_number);
                                line_error = 1;  /* Set line error flag */
                            }
                        if (legal_label(word, macro_table, symbol_table, argv, line_number)) {
                            if (allocate_node(temp_node, word, argv, line_number)) {
                                LABEL_flag = 1;
                            }
                        }
                        else
                            line_error = 1;
                        break;

                    case ORDER:
                        /* Handle order type */
                        if (IC_flag) {
                            fprintf(stderr, "ERROR: Order after order (file - %s, line - %d)\n", argv, line_number);
                            line_error = 1;
                        }
                        if ((LABEL_flag) && (word_count == 2)) {
                            (*temp_node)->value = *IC;
                            strcpy((*temp_node)->content, "code");
                            (*temp_node)->next = NULL;
                            add_node(symbol_table, *temp_node);
                        }
                        IC_flag = 1;
                        IC_first_word = *IC;
                        (*IC) += 1;
                        opcode = get_opcode(word);

                        /* Check if the opcode is in the second instruction set */
                        if (is_in_second_instruction_set(opcode)) {
                            sec_instruct_set_flag = 1;
                            source_flag = 1;
                            source_operand_address = 4;
                        }

                        /* Check if the opcode is in the third instruction set */
                        if (is_in_therd_instruction_set(opcode)) {
                            therd_instruct_set_flag = 1;
                            word = strtok(NULL, deli);
                            if (word) {
                                fprintf(stderr, "ERROR: Too many operands (file - %s, line - %d)\n", argv, line_number);
                                line_error = 1;
                                break;
                            }
                            source_operand_address = 4;
                            des_operand_address = 4;
                        }
                        break;
                    case DIRECTIVE:
                        /* If a label is defined and it's the second word in the line */
                        if ((LABEL_flag) && (word_count == 2)) {
                            /* Set the value of the temporary node to the data counter */
                            (*temp_node)->value = *DC;
                            /* Update the content of the temporary node based on the directive */
                            update_node_content((*temp_node), word);
                            /* Add the temporary node to the symbol table */
                            add_node(symbol_table, *temp_node);
                            LABEL_flag = 0;
                        }
                        /* Check if the directive is ".data" */
                        if (strcmp(word, ".data") == 0) {
                            DC_flag = 1;
                            data_flag = 1;
                        }
                        /* Check if the directive is ".string" */
                        else if (strcmp(word, ".string") == 0) {
                            DC_flag = 1;
                            string_flag = 1;
                        }
                        /* Check if the directive is ".extern" */
                        else if (strcmp(word, ".extern") == 0) {
                            external_flag = 1;
                            /* If there are more than one word before ".extern", it's an error */
                            if (word_count > 1) {
                                fprintf(stderr, "ERROR: Before external there is something (file - %s, line - %d)\n", argv, line_number);
                            }
                        }
                        /* Check if the directive is ".entry" */
                        else if (strcmp(word, ".entry") == 0) {
                            entry_flag = 1;
                            /* If there are more than one word before ".entry", it's an error */
                            if (word_count > 1) {
                                fprintf(stderr, "ERROR: Before entry there is something (file - %s, line - %d)\n", argv, line_number);
                            }
                        }
                        break;

                    case NUMBER:
                        /* Validate the number and check for errors */
                        if (line_error += valid_num(word + 1, argv, line_number))
                            break;
                        /* Check if the source flag is set and the opcode is not 1 or 12 */
                        if ((source_flag) && (opcode != 1) && (opcode != 12)) {
                            fprintf(stderr, "ERROR: Illegal destination operand (file - %s, line - %d)\n", argv, line_number);
                            line_error = 1;
                            break;
                        }
                        /* Check if the source flag is not set and the opcode is greater than 3 */
                        if ((!source_flag) && (opcode > 3)) {
                            fprintf(stderr, "ERROR: Illegal source operand (file - %s, line - %d)\n", argv, line_number);
                            line_error = 1;
                            break;
                        }
                        /* If the source flag is set, set the destination operand address to 0 */
                        if (source_flag) {
                            des_operand_address = 0;
                            des_flag = 1;
                        }
                        /* Otherwise, set the source operand address to 0 */
                        else {
                            source_flag = 1;
                            source_operand_address = 0;
                        }
                        /* Add the number representation to the instruction or data array */
                        if (IC_flag) {
                            I_array[(*IC)++] = create_num_rep(word);
                        } else if (DC_flag) {
                            D_array[(*DC)++] = create_num_rep(word);
                        }
                        break;

                    case LABEL:
                        if (legal_label(word, macro_table, symbol_table, argv, line_number))
                            reg_label_flag = 1;
                        /* If the source flag is set, set the destination operand address to 1 */
                        if (source_flag) {
                            des_operand_address = 1;
                            des_flag = 1;
                        }
                        /* Otherwise, set the source operand address to 1 */
                        else {
                            source_flag = 1;
                            source_operand_address = 1;
                        }
                        /* Add a placeholder to the instruction or data array */
                        if (IC_flag)
                            I_array[(*IC)++].short_num = 0;
                        else if (DC_flag)
                            D_array[(*DC)++].short_num = 0;
                        
                        /* If the external flag is set, add the label as an external symbol */
                        if (external_flag) {
                            if (allocate_node(temp_node, word, argv, line_number)) {
                                strcpy((*temp_node)->name, word);
                                (*temp_node)->value = -1;
                                strcpy((*temp_node)->content, "external");
                                add_node(symbol_table, *temp_node);    
                            }
                        }
                        /* Otherwise, add the label to the temporary symbol table */
                        else if (allocate_node(temp_node, word, argv, line_number)) {
                            strcpy((*temp_node)->name, word);
                            (*temp_node)->value = line_number;
                            add_node(temp_symbol_table, *temp_node);
                        }
                        break;
                    case REGISTER_POINTER:
                        /* Check if the source flag is not set and the opcode is 4 */
                        if ((!source_flag) && (opcode == 4)) {
                            fprintf(stderr, "ERROR: Illegal source operand (file - %s, line - %d)\n", argv, line_number);
                            line_error = 1;
                            break;
                        }
                        /* Move to the next character in the word */
                        word++;
                        /* If the source flag is set, set the destination operand address to 2 */
                        if (source_flag) {
                            des_operand_address = 2;
                            des_flag = 1;
                        }
                        /* Otherwise, set the source operand address to 2 */
                        else 
                            source_operand_address = 2;
                        /* Call the reg_case function to handle the register case */
                        reg_case(I_array, D_array, word, &source_operand, &des_operand, &source_flag, &reg_flag, &IC_flag, &DC_flag, IC, DC);
                        break;

                    case REGISTER:
                        /* Check if the source flag is not set and the opcode is 4 */
                        if ((!source_flag) && (opcode == 4)) {
                            fprintf(stderr, "ERROR: Illegal source operand (file - %s, line - %d)\n", argv, line_number);
                            line_error = 1;
                            break;
                        }
                        /* Check if the source flag is set and the opcode is 9, 10, or 13 */
                        if ((source_flag) && ((opcode == 9) || (opcode == 10) || (opcode == 13))) {
                            fprintf(stderr, "ERROR: Illegal destination operand (file - %s, line - %d)\n", argv, line_number);
                            line_error = 1;
                            break;
                        }
                        /* Set the destination or source operand address based on the source flag */
                        source_flag ? (des_operand_address = 3, des_flag = 1) : (source_operand_address = 3);
                        /* Call the reg_case function to handle the register case */
                        reg_case(I_array, D_array, word, &source_operand, &des_operand, &source_flag, &reg_flag, &IC_flag, &DC_flag, IC, DC);
                        break;

                    case STRING:
                        /* Check if the string flag is set and the string is valid */
                        if ((string_flag) && (valid_string(temp_line, argv, line_number))) {
                            int i = 0;
                            /* Find the starting quote of the string */
                            while (temp_line[i] != '"')
                                i++;
                            i++;
                            /* Add each character of the string to the data array */
                            while (temp_line[i] != '"')
                                D_array[(*DC)++].short_num = (unsigned short int)temp_line[i++];
                            /* Add the null terminator to the data array */
                            D_array[(*DC)++].short_num = (unsigned short int)'\0';
                            word = NULL;
                        }
                        break;

                    case REGULAR_NUMBER:
                        /* Validate the number and check for errors */
                        if (line_error += valid_num(word, argv, line_number))
                            break;
                        /* If the data flag is set, add the number to the data array */
                        if (data_flag)
                            D_array[(*DC)++] = create_num_rep(word);
                        else {
                            fprintf(stderr, "ERROR: Number without .data or '#' (file - %s, line - %d)\n", argv, line_number);
                            line_error = 1;
                        }
                        break;

                    case ERROR:
                        /* Handle error case */
                        break;

                    default:
                        /* Handle default case */
                        break;
                    }
                
                    /* Check for errors after .data directive */
                    if (data_flag && (get_word_type(word) != REGULAR_NUMBER) && (strcmp(word, ".data") != 0)) {
                        fprintf(stderr, "ERROR: After .data not a number (file - %s, line - %d)\n", argv, line_number);
                        line_error = 1;
                    }

                    if (word)
                        word = strtok(NULL, deli); /* Reset the word count */
                }

            /* Check for errors with label without order or instruction */
            if (((reg_label_flag) || (LABEL_flag)) && (!IC_flag) && (!DC_flag) && (!external_flag) && (!entry_flag)) {
                fprintf(stderr, "ERROR: Label without order or instruction (file - %s, line - %d)\n", argv, line_number);
                line_error = 1;
            }
            
            /* Check for errors with instruction without anything */
            if ((word_count == 1) && ((DC_flag) || (external_flag) || (entry_flag))) {
                fprintf(stderr, "ERROR: After Instruction there is nothing (file - %s, line - %d)\n", argv, line_number);
                line_error = 1;
            }

            /* Check for too few operands errors */
            if (!line_error) {
                if ((sec_instruct_set_flag) && (((word_count == 2) && (LABEL_flag)) || ((word_count == 1) && (!LABEL_flag)))) {
                    fprintf(stderr, "ERROR: Too few operands (file - %s, line - %d)\n", argv, line_number);
                    line_error = 1;
                }
                if ((!therd_instruct_set_flag) && (!sec_instruct_set_flag) && (IC_flag) && (((word_count < 4) && (LABEL_flag)) || ((word_count < 3) && (!LABEL_flag)))) {
                    fprintf(stderr, "ERROR: Too few operands (file - %s, line - %d)\n", argv, line_number);
                    line_error = 1;
                }
                /* Create the word representation for the instruction array */
                if (IC_flag) {
                    I_array[IC_first_word] = create_word_rep(opcode, source_operand_address, des_operand_address, 'A');
                }
            }

            /* Update the overall program error status */
            prog_error += line_error;

            /* Reset line index, word count, and error flags for the next line */
            line_index = word_count = line_error = 0;
            LABEL_flag = reg_label_flag = data_flag = string_flag = external_flag = entry_flag = source_flag = des_flag = IC_flag = DC_flag = reg_flag = 0;
            sec_instruct_set_flag = therd_instruct_set_flag = 0;
        }

        else
            line[line_index++] = ch; /* Add character to the current line */
    }


    if (fatal_error)
        return fatal_error;
    return prog_error;
}
