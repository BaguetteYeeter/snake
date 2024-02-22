#include <stdlib.h>

#include "struct.h"

void array_push(node * head, pointer val) {
    node * current = head;
    while (current->next != NULL) {
        current = current->next;
    }
    current->next = (node *) malloc(sizeof(node));
    current->next->val = val;
    current->next->next = NULL;
}

pointer array_pop(node ** head) {
    pointer retval;
    node * next_node = NULL;
    if (*head == NULL) {
        return retval;
    }
    next_node = (*head)->next;
    retval = (*head)->val;
    free(*head);
    *head = next_node;
    return retval;
}

pointer array_last(node * head) {
    pointer retval;
    if (head->next == NULL) {
        retval = head->val;
        return retval;
    }

    node * current = head;
    while (current->next->next != NULL) {
        current = current->next;
    }

    retval = current->next->val;
    return retval;
}

int array_contains(node * head, pointer val) {
    node * current = head;
    while (current->next != NULL) {
        if (current->val.x == val.x && current->val.y == val.y) {
            return 1;
        }
        current = current->next;
    }
    return 0;
}



int valueinarray(int val, int *arr, size_t n) {
    for (size_t i = 0; i < n; i++) {
        if(arr[i] == val)
            return 1;
    }
    return 0;
}

void addtoarray(int val, int *arr, size_t n) {
    for (size_t i = 0; i < n; i++) {
        if(arr[i] == -1) {
            arr[i] = val;
            break;
        }
    }
}

void cleararray(int *arr, size_t n) {
    for (size_t i = 0; i < n; i++) {
        arr[i] = -1;
    }
}