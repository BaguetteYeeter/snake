#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>

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

//all linked list stuff nicked from learn-c.org
typedef struct node_t {
    pointer val;
    struct node_t * next;
} node;

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


void update(pointer op, node *points) {
    node *current = points;

    mvchgat(op.y, op.x*2,   1, A_NORMAL, 0, NULL);
    mvchgat(op.y, op.x*2+1, 1, A_NORMAL, 0, NULL);

    while (current != NULL) {
        mvchgat(current->val.y, current->val.x*2,   1, A_REVERSE, 0, NULL);
        mvchgat(current->val.y, current->val.x*2+1, 1, A_REVERSE, 0, NULL);
        current = current->next;
    }

    refresh();
}

int main() {
    int row,col;
    int ch;
    enum Directions dir = RIGHT;
    bool moved;

    pointer p;
    p.x = 0;
    p.y = 0;

    //old pointer, used to clear the space behind
    pointer op;
    op.x = 0;
    op.y = 0;

    node * points = NULL;
    points = (node *) malloc(sizeof(node));
    if (points == NULL) {
        return 1;
    }

    pointer head;
    head.x = 0;
    head.y = 0;

    points->val = head;
    points->next = NULL;

    pointer p1;
    p1.y = 0;
    for (int i = 0; i < 4; i++) {
        p1.x = i+1;
        array_push(points, p1);
    }
    

    initscr();

    getmaxyx(stdscr,row,col);

    raw();
    noecho();
    keypad(stdscr, TRUE);
    cbreak();
    curs_set(0);
    timeout(500);

    printw("Hello world!\n");
    printw("The terminal is %dx%d", row, col);
    refresh();

    update(op, points);

    while (true) {
        moved = false;
        ch = getch();
        if (ch != ERR) {
            if (ch == KEY_F(1)) {
                break;
            }

            p = array_last(points);

            if (ch == KEY_UP && p.y > 0) {
                p.y--;
                dir = UP;
                moved = true;
            } else if (ch == KEY_DOWN && p.y < row-1) {
                p.y++;
                dir = DOWN;
                moved = true;
            } else if (ch == KEY_LEFT && p.x > 0) {
                p.x--;
                dir = LEFT;
                moved = true;
            } else if (ch == KEY_RIGHT && p.x*2 < col-2) {
                p.x++;
                dir = RIGHT;
                moved = true;
            }

            if (moved) {
                op = array_pop(&points);
                array_push(points, p);
            }

            update(op,points);

            //printw("%d\n", ch);
        } else {
            continue;
            if (dir == UP) {
                op.x = p.x;
                op.y = p.y;
                p.y--;
                update(op, points);
            } else if (dir == DOWN) {
                op.x = p.x;
                op.y = p.y;
                p.y++;
                update(op, points);
            } else if (dir == LEFT) {
                op.x = p.x;
                op.y = p.y;
                p.x--;
                update(op, points);
            } else if (dir == RIGHT) {
                op.x = p.x;
                op.y = p.y;
                p.x++;
                update(op, points);
            }
        }
    }
    
    endwin();
    return 0;
}