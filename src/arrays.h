#include "struct.h"

#ifndef	ARRAY_H
#define	ARRAY_H

void array_push(node * head, pointer val);
pointer array_pop(node ** head);
pointer array_last(node * head);
bool array_contains(node * head, pointer val);

bool valueinarray(int val, int *arr, size_t n);
void addtoarray(int val, int *arr, size_t n);
void cleararray(int *arr, size_t n);

#endif