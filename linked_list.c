#include <stdlib.h>
#include <stdio.h>
#include "linked_list.h"

int Member(int value, struct list_node_s *head_p)
{
    struct list_node_s *curr_p = head_p;

    while (curr_p != NULL && curr_p->data < value)
        curr_p = curr_p->next;

    if (curr_p == NULL || curr_p->data > value)
        return 0;
    else
        return 1;
} /* Member */

int Insert(int value, struct list_node_s **head_pp)
{
    struct list_node_s *curr_p = *head_pp;
    struct list_node_s *prev_p = NULL;
    struct list_node_s *temp_p;

    while (curr_p != NULL && curr_p->data < value)
    {
        prev_p = curr_p;
        curr_p = curr_p->next;
    }

    if (curr_p == NULL || curr_p->data > value) {
        temp_p = (struct list_node_s *) malloc(sizeof(struct list_node_s));
        if (temp_p == NULL) {
            // Handle malloc failure, e.g., log an error and exit
            printf("Error: malloc failed\n");
            exit(EXIT_FAILURE);
        }
        temp_p->data = value;
        temp_p->next = curr_p;

        if (prev_p == NULL) /* First element */
            *head_pp = temp_p;
        else
            prev_p->next = temp_p;

        return 1;
    }
    else
        return 0;
} /* Insert */

int Delete(int value, struct list_node_s **head_pp)
{
    struct list_node_s *curr_p = *head_pp;
    struct list_node_s *prev_p = NULL;

    while (curr_p != NULL && curr_p->data < value)
    {
        prev_p = curr_p;
        curr_p = curr_p->next;
    }

    if (curr_p != NULL && curr_p->data == value)
    {
        if (prev_p == NULL) /* Deleting first element */
        {
            *head_pp = curr_p->next;
            free(curr_p);
        }
        else
        {
            prev_p->next = curr_p->next;
            free(curr_p);
        }

        return 1;
    }
    else
        return 0;
} /* Delete */

struct list_node_s *CopyList(struct list_node_s *head) {
    if (head == NULL) {
        return NULL;
    }

    struct list_node_s *new_head = malloc(sizeof(struct list_node_s));
    new_head->data = head->data;
    new_head->next = NULL;

    struct list_node_s *current_new_node = new_head;
    struct list_node_s *current_orig_node = head->next;

    while (current_orig_node != NULL) {
        struct list_node_s *new_node = malloc(sizeof(struct list_node_s));
        new_node->data = current_orig_node->data;
        new_node->next = NULL;
        
        current_new_node->next = new_node;
        current_new_node = new_node;
        current_orig_node = current_orig_node->next;
    }

    return new_head;
} /* CopyList */

void FreeList(struct list_node_s *head) {
    struct list_node_s *current_node = head;
    while (current_node != NULL) {
        struct list_node_s *next_node = current_node->next;
        free(current_node);
        current_node = next_node;
    }
} /* FreeList */