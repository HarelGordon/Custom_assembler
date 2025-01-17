#ifndef GLOBALS_H
#define GLOBALS_H

/* The File contains all the global values in the program */

/* Maximum length of a label in command line  */
#define MAX_LABEL_LENGTH 31

/* Maximum length of a single command line  */
#define MAX_LINE_LENGTH 81

/* Maximum length of a content in symbol table  */
#define MAX_CONTENT_LENGTH 15

/* Maximum words in file */
#define MAX_WORDS 4096

/* Extern declarations for global arrays */
extern const char *order[];
extern const char *directive[];
extern const char *register_names[];
enum words {ORDER, DIRECTIVE, REGISTER, LABEL_DEF, NUMBER, STRING, LABEL, REGULAR_NUMBER, REGISTER_POINTER, ERROR};

#endif /* GLOBALS_H */
