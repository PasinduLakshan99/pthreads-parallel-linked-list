// linked_list.h

#ifndef LINKED_LIST_H
#define LINKED_LIST_H

struct list_node_s {
    int data;
    struct list_node_s *next;
};

int Member(int value, struct list_node_s *head_p);
int Insert(int value, struct list_node_s **head_pp);
int Delete(int value, struct list_node_s **head_pp);
struct list_node_s *CopyList(struct list_node_s *head_p);
void FreeList(struct list_node_s *head_pp);

#endif // LINKED_LIST_H