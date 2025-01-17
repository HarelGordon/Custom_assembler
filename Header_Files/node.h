#ifndef C_Node_H
#define C_Node_H

typedef struct Node {
    char *name;
    int value;
    char *content;
    struct Node *next;
} Node;

/** 
 * Adds a new node to the end of the linked list.
 * @param head - double pointer to the head of the list
 * @param new_node - pointer to the new node
 **/
void add_node(Node **head, Node *new_node);

/** 
 * Returns the value of a node with a given name.
 * @param head - double pointer to the head of the list
 * @param name - the name of the node
 * @return the value of the node if found, -2 otherwise
 **/
int get_node_value(Node **head, char *name);

/** 
 * Returns the content of a node with a given name.
 * @param head - double pointer to the head of the list
 * @param first_field - the name of the node
 * @return the content of the node if found, NULL otherwise
 **/
char *is_in(Node **head, char *first_field);

/** 
 * Prints the details of all the nodes in the list.
 * @param head - double pointer to the head of the list
 **/
void print_node(Node **head);

/** 
 * Frees all the nodes in the list.
 * @param head - double pointer to the head of the list
 **/
void free_node(Node **head);

/** 
 * Allocates memory for a new node and initializes its fields.
 * @param temp_node - double pointer to the new node
 * @param name - the name of the new node
 * @param argv - the name of the file
 * @param line_number - the line number in the file
 * @return 1 if the allocation was successful, 0 otherwise
 **/
int allocate_node(Node **temp_node, char *name, char *argv, int line_number);


#endif  /* C_Node_H */
