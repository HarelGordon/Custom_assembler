#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "../Header_Files/node.h"
#include "../Header_Files/word_rep.h"
#include "../Header_Files/first_step.h"


int create_object_file(Word_Rep *I_array, Word_Rep *D_array, int IC, int DC, char *input_file_name) {
    FILE *output_file; /* Declare a file pointer for the output file */
    char *output_file_name; /* Declare a pointer for the output file name */
    int i; /* Declare a loop counter */

    /* Check if both instruction and data arrays are empty */
    if ((I_array[0].short_num == 0) && (D_array[0].short_num == 0)) {
        /* Print an error message indicating the object file is empty */
        fprintf(stderr, "ERROR: Empty object file (file - %s)\n", input_file_name);
        return 1; /* Return 1 to indicate an error */
    }

    /* Allocate memory for the output file name */
    output_file_name = (char *)malloc((strlen(input_file_name) + 4) * sizeof(*output_file_name));
    /* Check if memory allocation failed */
    if (!output_file_name) {
        /* Print an error message indicating memory allocation failure */
        fprintf(stderr, "ERROR: Memory allocation to outfile object name failed (file - %s)\n", input_file_name);
        return 1; /* Return 1 to indicate an error */
    }

    strcpy(output_file_name, input_file_name); /* Copy the input file name to the output file name */
    strcat(output_file_name, ".ob"); /* Append the ".ob" extension to the output file name */

    /* Open the output file in write mode */
    output_file = fopen(output_file_name, "w");
    if (!output_file) {
        /* Print an error message if the output file cannot be opened */
        fprintf(stderr, "ERROR: Cannot open outfile object file (file - %s)\n", input_file_name);
        free (output_file_name); /* Free the memory allocated for the output file name */
        return 1; /* Return 1 to indicate an error */
    }

    /* Write the instruction and data counters to the output file */
    fprintf(output_file, "%d %d\n", IC, DC);

    free(output_file_name); /* Free the memory allocated for the output file name */

    /* Loop through the instruction array and write each instruction to the output file */
    for (i = 0; i < IC; i++)
        /* Write the instruction address and value in octal format */
        fprintf(output_file, "%04d %05o\n", i + 100, (unsigned short int) ((I_array[i].short_num) & 0x7FFF));

    /* Loop through the data array and write each data value to the output file */
    for (i = 0; i < DC; i++)
        /* Write the data address and value in octal format */
        fprintf(output_file, "%04d %05o\n", i + 100 + IC, (unsigned short int) ((D_array[i].short_num) & 0x7FFF));

    /* Close the output file */
    fclose(output_file);

    /* Return 0 to indicate success */
    return 0;
}

int create_extern_file(Node **symbol_table, char *input_file_name) {
    FILE *output_file; /* Declare a file pointer for the output file */
    char *output_file_name; /* Declare a pointer for the output file name */
    Node *temp_node; /* Declare a temporary node pointer */
    int ext_count = 0; /* Initialize a counter for external symbols */

    /* Allocate memory for the output file name */
    output_file_name = malloc(strlen(input_file_name) + 5);
    if (!output_file_name) {
        /* Print an error message if memory allocation fails */
        fprintf(stderr, "ERROR: Memory allocation to outfile extern name failed (file - %s)\n", input_file_name);
        return 1; /* Return 1 to indicate an error */
    }

    /* Copy the input file name to the output file name */
    strcpy(output_file_name, input_file_name);
    /* Append the ".ext" extension to the output file name */
    strcat(output_file_name, ".ext");

    /* Open the output file in write mode */
    output_file = fopen(output_file_name, "w");

    /* Initialize the temporary node to the head of the symbol table */
    temp_node = *symbol_table;

    /* Loop through the symbol table */
    while (temp_node) {
        /* Check if the symbol is external */
        if (strcmp(temp_node->content, "external") == 0) {
            /* Write the symbol name and value to the output file */
            fprintf(output_file, "%s %04d\n", temp_node->name, temp_node->value);
            ext_count++; /* Increment the external symbol counter */
        }
        /* Move to the next node in the symbol table */
        temp_node = temp_node->next;
    }

    /* Close the output file */
    fclose(output_file);

    /* If there are no external symbols, remove the output file */
    if (ext_count == 0)
        remove(output_file_name);

    /* Return 0 to indicate success */
    return 0;
}

int create_entry_file(Node **symbol_table, char *input_file_name) {
    FILE *output_file; /* Declare a file pointer for the output file */
    char *output_file_name; /* Declare a pointer for the output file name */
    Node *temp_node; /* Declare a temporary node pointer */
    int ent_count = 0; /* Initialize a counter for entry symbols */

    /* Allocate memory for the output file name */
    output_file_name = malloc(strlen(input_file_name) + 5);
    if (!output_file_name) {
        /* Print an error message if memory allocation fails */
        fprintf(stderr, "ERROR: Memory allocation to outfile entry name failed (file - %s)\n", input_file_name);
        return 1; /* Return 1 to indicate an error */
    }

    /* Copy the input file name to the output file name */
    strcpy(output_file_name, input_file_name);
    /* Append the ".ent" extension to the output file name */
    strcat(output_file_name, ".ent");

    /* Open the output file in write mode */
    output_file = fopen(output_file_name, "w");

    /* Initialize the temporary node to the head of the symbol table */
    temp_node = *symbol_table;

    /* Loop through the symbol table */
    while (temp_node) {
        /* Check if the symbol is an entry */
        if (strcmp(temp_node->content, "entry") == 0) {
            /* Write the symbol name and value to the output file */
            fprintf(output_file, "%s %04d\n", temp_node->name, temp_node->value);
            ent_count++; /* Increment the entry symbol counter */
        }
        /* Move to the next node in the symbol table */
        temp_node = temp_node->next;
    }

    /* Close the output file */
    fclose(output_file);

    /* If there are no entry symbols, remove the output file */
    if (ent_count == 0)
        remove(output_file_name);

    /* Return 0 to indicate success */
    return 0;
}

int create_output_files(Word_Rep *I_array, Word_Rep *D_array, int IC, int DC, char *input_file_name, Node **symbol_table) {
    /* Remove the last three characters from the input file name */
    input_file_name[strlen(input_file_name) - 3] = '\0';

    /* Create the object file, extern file, and entry file */
    if ((create_object_file(I_array, D_array, IC, DC, input_file_name)) || 
        (create_extern_file(symbol_table, input_file_name)) || 
        (create_entry_file(symbol_table, input_file_name))) {
        /* If any of the file creation functions return an error, return 1 */
        return 1;
    }

    /* Return 0 to indicate success */
    return 0;
}
