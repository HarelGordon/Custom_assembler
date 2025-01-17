#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "../Header_Files/globals.h"
#include "../Header_Files/node.h"

void add_node(Node **head, Node *new_node) {
    Node *p1, *p2;
    if (*head == NULL) {
        /* If the list is empty, set the new node as the head */
        *head = new_node;
    } else {
        p1 = *head;
        while (p1) { /* Traverse to the end of the list */
            p2 = p1;
            p1 = p1->next;
        }
        /* Add the new node at the end */
        p2->next = new_node;
    }
}

int get_node_value(Node **head, char *name) {
    Node *p1 = *head;
    while (p1) {
        /* Compare the name of the current node with the given name */
        if (strcmp(p1->name, name) == 0)
            return p1->value;
        p1 = p1->next;
    }
    return -2; /* Node not found */
}

char *is_in(Node **head, char *first_field) {
    Node *p1 = *head;
    char *str;
    if (p1 == NULL)
        return NULL; /* List is empty */
    while (p1) {
        /* Compare the name of the current node with the given name */
        if (strcmp(p1->name, first_field) == 0) {
            /* Allocate memory for the content copy */
            str = malloc(strlen(p1->content) + 1);
            if (!str) {
                fprintf(stderr, "Memory allocation failed\n");
                exit(1);
            }
            /* Copy the content */
            strcpy(str, p1->content);
            return str;
        }
        p1 = p1->next;
    }
    return NULL; /* Node not found */
}

void print_node(Node **head) {
    Node *current = *head;
    while (current != NULL) {
        /* Print the current node's details */
        printf("%s: %s, %d -->\n", current->name, current->content, current->value);
        current = current->next;
    }
}

void free_node(Node **head) {
    Node *p1, *p2;
    p1 = *head;
    while (p1) {
        p2 = p1;
        p1 = p1->next;
        /* Free the name and content of the current node */
        if (p2->name) {
            free(p2->name);
        }
        if (p2->content) {
            free(p2->content);
        }
        /* Free the current node */
        free(p2);
    }
    *head = NULL;  /* Set head to NULL after freeing */
}

int allocate_node(Node **temp_node, char *name, char *argv, int line_number) {
    /* Allocate memory for the new node */
    *temp_node = malloc(sizeof(Node));
    if (!(*temp_node)) {
        fprintf(stderr, "ERROR: Memory allocation failed (file - %s, line - %d)\n", argv, line_number);
        return 0;
    }
    /* Allocate memory for the node's name */
    (*temp_node)->name = malloc(strlen(name) + 1);
    if (!(*temp_node)->name) {
        fprintf(stderr, "ERROR: Memory allocation failed (file - %s, line - %d)\n", argv, line_number);
        free(*temp_node);
        *temp_node = NULL;
        return 0;
    }
    /* Copy the name */
    strcpy((*temp_node)->name, name);
    /* Allocate memory for the node's content */
    (*temp_node)->content = malloc(MAX_LINE_LENGTH);
    if (!(*temp_node)->content) {
        fprintf(stderr, "ERROR: Memory allocation failed (file - %s, line - %d)\n", argv, line_number);
        free((*temp_node)->name);
        free(*temp_node);
        *temp_node = NULL;
        return 0;
    }
    /* Initialize the content and value */
    (*temp_node)->content[0] = '\0';
    (*temp_node)->value = 0;
    (*temp_node)->next = NULL;
    return 1;
}

