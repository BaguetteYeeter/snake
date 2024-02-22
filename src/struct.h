#ifndef	STRUCT_H
#define	STRUCT_H

typedef struct {
    int x;
    int y;
} pointer;

enum Directions {
    UP,
    DOWN,
    LEFT,
    RIGHT
};

typedef struct node_t {
    pointer val;
    struct node_t * next;
} node;

#endif