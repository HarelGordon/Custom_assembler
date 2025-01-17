#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../Header_Files/node.h"
#include "../Header_Files/macro_step.h"
#include "../Header_Files/globals.h"
#include "../Header_Files/first_step.h"
#include "../Header_Files/second_step.h"
#include "../Header_Files/word_rep.h"
#include "../Header_Files/output_files.h"

/**
 * The main function.
 * 
 * @param argc The number of arguments.
 * @param argv The arguments.
 * @return 0 if the program ran successfully, 1 otherwise.
 **/
int main(int argc, char **argv) {
    FILE *input_file = NULL, *output_file = NULL; /* File pointers for input and output files */
    char *input_filename = NULL; /* Pointer to store the input filename */
    Node *macro_table = NULL, *symbol_table = NULL, *temp_symbol_table = NULL; /* Pointers to symbol tables */
    Node *temp_node = NULL; /* Pointer to a temporary node */
    Word_Rep I_array[MAX_WORDS], D_array[MAX_WORDS]; /* Arrays to store instruction and data words */
    int i; /* Loop counter and error flag */
    int first_step_error = 0;

    /* Check if no arguments are passed */
    if (argc == 1) {
        fprintf(stderr, "No arguments passed\n");
        exit(1); /* Exit the program with error code 1 */
    }
    
    /* Loop through each argument passed to the program */
    for (i = 1; i < argc; i++) {
        int IC = 0, DC = 0, error = 0; /* Initialize instruction, data counters and error flag */
        printf("\n");

        /* Free previously allocated memory for symbol tables, macro table and input filename */
        if (macro_table) free_node(&macro_table);
        if (symbol_table) free_node(&symbol_table);
        if (temp_symbol_table) free_node(&temp_symbol_table);
        if (input_filename) free(input_filename);
        
        /* Initialize pointers to NULL */
        macro_table = symbol_table = temp_node = temp_symbol_table = NULL;
        input_file = output_file = NULL;
        
        /* Allocate memory for the input filename */
        input_filename = (char *)malloc((strlen(argv[i]) + 4) * sizeof(char));
        if (!input_filename) {
            fprintf(stderr, "Memory allocation failed\n");
            exit(1); /* Exit the program with error code 1 */
        }
        
        /* Create the input filename with ".as" extension */
        sprintf(input_filename, "%s.as", argv[i]);

        /* Prepare the macro step */
        if (!(prepers_macro_step(&input_file, &output_file, input_filename, &temp_node))) {
            if (input_file) fclose(input_file); /* Close input file if open */
            if (output_file) fclose(output_file); /* Close output file if open */
            continue; /* Continue to the next argument */
        }
        
        /* Process the file until end of file */
        if (loop_until_eof(input_file, output_file, temp_node, &macro_table, input_filename)) {
            fclose(input_file); /* Close input file */
            continue; /* Continue to the next argument */
        }
        
        fclose(input_file); /* Close input file */
        
        printf("----- Macro Step Done Successfully for file %s -----\n", input_filename);
        
        /* Prepare for the first step */
        if (!(first_step_prepers(&temp_node, &input_file, input_filename))) {
            continue; /* Continue to the next argument */
        }
        
        /* Perform the first step */
        first_step_error = first_step(input_file, &macro_table, &symbol_table, &temp_symbol_table, &temp_node, I_array, D_array, &IC, &DC, input_filename); 
        if (first_step_error == -1)  
            continue; /* Break the loop */
        if (first_step_error)
            error = 1; /* Set error flag */
    
        printf("----- First Step Done Successfully for file %s -----\n", input_filename);
        
        rewind(input_file); /* Rewind the input file */
        
        /* Perform the second step */
        if (second_step(input_file, &symbol_table, &temp_symbol_table, I_array, D_array, IC, input_filename)) {
            error = 1; /* Set error flag */
        }
        
        fclose(input_file); /* Close input file */
        printf("----- Second Step Done Successfully for file %s----- \n", input_filename);
        if (error)
            continue; /* Continue to the next argument */
        
        /* Create the output files */
        if (create_output_files(I_array, D_array, IC, DC, input_filename, &symbol_table)) {
            continue; /* Continue to the next argument */
        }
        
        printf("----- Output Files Created Successfully for file %s -----\n\n", input_filename); 
    }

    /* Free allocated memory for input filename and symbol tables */
    if (input_filename) free(input_filename);
    if (macro_table) free_node(&macro_table);
    if (symbol_table) free_node(&symbol_table);
    if (temp_symbol_table) free_node(&temp_symbol_table);
    if (temp_node) free_node(&temp_node);

    
    printf("----- Finished ! -----\n");
    return 0;
}

