#ifndef C_WORD_REP_H
#define C_WORD_REP_H

#include "../Header_Files/node.h"

typedef struct Word_Rep {
    unsigned short short_num; /* The numerical representation of the converted line */
    unsigned char adress; /* The adress of the word in the memory */
} Word_Rep;


/**
 * Creates a Word_Rep struct from an opcode, source operand, destination operand, and ARF.
 * @param opcode The opcode.
 * @param source_operand The source operand.
 * @param des_operand The destination operand.
 * @param ARF The ARF.
 * @return The Word_Rep struct.
 **/
Word_Rep create_word_rep(int opcode, int source_operand, int des_operand, char ARF);

/**
 * Creates a Word_Rep struct from a number.
 * @param num The number to convert.
 * @return The Word_Rep struct.
 **/
Word_Rep create_num_rep(char *num);

/**
 * Creates a Word_Rep struct from a label.
 * @param label The label to convert.
 * @param symbol_table The symbol table.
 * @return The Word_Rep struct.
 **/
Word_Rep create_label_rep(char *label, Node **symbol_table);

/**
 * Creates a Word_Rep struct from a register.
 * @param reg The register to convert.
 * @param source_or_des The source or destination register.
 * @return The Word_Rep struct.
 **/
Word_Rep create_reg_rep(char *reg, char *source_or_des);

/**
 * Recreates a register representation.
 * @param operand The operand to recreate.
 * @param reg The register to recreate.
 * @return The recreated operand.
 **/
Word_Rep recreate_reg_rep(Word_Rep *operand, char *reg);

#endif  /* C_WORD_REP_H */
