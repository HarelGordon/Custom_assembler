#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../Header_Files/node.h"
#include "../Header_Files/word_rep.h"
#include "../Header_Files/first_step.h"


Word_Rep create_word_rep(int opcode, int source_operand, int des_operand, char ARF) {
    Word_Rep temp_word_rep;
    temp_word_rep.short_num = 0; /* Initialize short_num to 0 */
    
    /* Check if the source operand is 4 */
    if (source_operand == 4) {
        /* If both source and destination operands are 4 */
        if (des_operand == 4)
            temp_word_rep.short_num = (opcode << 11); /* Set short_num with opcode shifted left by 11 bits */
        else
            temp_word_rep.short_num = (opcode << 11) | (1 << (des_operand + 3)); /* Set short_num with opcode and destination operand */
    } else {
        /* Set short_num with opcode, source operand, and destination operand */
        temp_word_rep.short_num = (opcode << 11) | (1 << (source_operand + 7)) | (1 << (des_operand + 3));
    }
    
    /* Set the addressing mode based on ARF */
    if (ARF == 'A') {
        temp_word_rep.short_num |= (1 << 2); /* Set the 3rd bit for 'A' */
    } else if (ARF == 'R') {
        temp_word_rep.short_num |= (1 << 1); /* Set the 2nd bit for 'R' */
    } else if (ARF == 'F') {
        temp_word_rep.short_num |= 1; /* Set the 1st bit for 'F' */
    }
    
    return temp_word_rep; /* Return the created Word_Rep */
}

Word_Rep create_num_rep(char *num) {
    Word_Rep temp_word_rep;
    temp_word_rep.short_num = 0; /* Initialize short_num to 0 */
    
    /* Check if the number is prefixed with '#' */
    if (num[0] == '#') {
        /* Skip the '#' character and convert the rest to an integer */
        temp_word_rep.short_num = atoi(num + 1) << 3 | 1 << 2;
    } else if (num[0] == '+') {
        /* Convert the rest of the string to an integer */
        temp_word_rep.short_num = atoi(num + 1);
    } else {
        /* Convert the string to an integer */
        temp_word_rep.short_num = atoi(num);
    }
    
    return temp_word_rep; /* Return the created Word_Rep */
}

Word_Rep create_label_rep(char *label, Node **symbol_table) {
    Word_Rep temp_word_rep;
    int node_value = get_node_value(symbol_table, label); /* Get the value of the node from the symbol table */
    temp_word_rep.short_num = 0; /* Initialize short_num to 0 */
    
    /* Check if the node value is -1 (undefined) */
    if (node_value == -1) {
        temp_word_rep.short_num = 1; /* Set short_num to 1 for undefined label */
    } 
    /* Check if the node value is -2 (not found) */
    else if (node_value == -2) {
        fprintf(stderr, "Error: Label %s not found in symbol table\n", label); /* Print error message */
        exit(1); /* Exit the program with error code 1 */
    } 
    /* If the node value is valid */
    else {
        temp_word_rep.short_num = (node_value << 3) | (1 << 1); /* Set short_num with node value shifted left by 3 bits and OR with 1 shifted left by 1 bit */
    }
    
    return temp_word_rep; /* Return the created Word_Rep */
}

Word_Rep create_reg_rep(char *reg, char *source_or_des) {
    Word_Rep temp_word_rep;
    int reg_num = reg[1] - '0'; /* Convert the register number from character to integer */
    temp_word_rep.short_num = 0; /* Initialize short_num to 0 */
    
    /* Check if the register is a source register */
    if (strcmp(source_or_des, "source") == 0) {
        temp_word_rep.short_num = (reg_num << 6) | (1 << 2); /* Set short_num with register number shifted left by 6 bits and OR with 1 shifted left by 2 bits */
    } 
    /* If the register is a destination register */
    else {
        temp_word_rep.short_num = (reg_num << 3) | (1 << 2); /* Set short_num with register number shifted left by 3 bits and OR with 1 shifted left by 2 bits */
    }
    
    return temp_word_rep; /* Return the created Word_Rep */
}

Word_Rep recreate_reg_rep(Word_Rep *operand, char *reg) {
    int reg_num = reg[1] - '0'; /* Convert the register number from character to integer */
    operand->short_num |= (reg_num << 3); /* OR the short_num with register number shifted left by 3 bits */
    
    return *operand; /* Return the modified Word_Rep */
}
