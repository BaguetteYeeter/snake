#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <time.h>

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

//thank you stack overflow
bool valueinarray(int val, int *arr, size_t n) {
    for (size_t i = 0; i < n; i++) {
        if(arr[i] == val)
            return true;
    }
    return false;
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

int main() {
    int row,col;
    int ch;
    enum Directions dir = RIGHT;
    enum Directions lastdir = RIGHT;
    bool moved, automoved;

    struct timespec time1, time2;
    double timetaken;
    int delay = 20;
    int keys[10];
    cleararray(keys, 10);
    

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
    cbreak();
    keypad(stdscr, TRUE);
    curs_set(0);
    timeout(delay);

    printw("Hello world!\n");
    printw("The terminal is %dx%d", row, col);
    refresh();

    update(op, points);

    timetaken = 0;

    while (true) {
        //mvprintw(5, 0, "1 "); refresh();
        moved = false;
        automoved = false;

        clock_gettime(CLOCK_MONOTONIC, &time1);
        ch = getch();
        clock_gettime(CLOCK_MONOTONIC, &time2);

        //mvprintw(2, 0, "%d       ", 1000*(time2-time1)/CLOCKS_PER_SEC); refresh();

        if (!valueinarray(ch, keys, 10) && ch != ERR) {
            addtoarray(ch, keys, 10);
        }


        double elapsed = (time2.tv_sec - time1.tv_sec) + (time2.tv_nsec - time1.tv_nsec) / 1e9;

        timetaken += elapsed;

        if (timetaken*1000 < delay*10) {
            continue;
        }

        timetaken = 0;

        for (int i = 0; i < 10; i++) {
            ch = keys[i];

            if (ch != ERR) {
                if (ch == KEY_F(1)) {
                    break;
                } else if (ch == KEY_RESIZE) {
                    getmaxyx(stdscr,row,col);
                    mvprintw(1, 0, "The terminal is %dx%d", row, col);
                }

                p = array_last(points);

                if (ch == KEY_UP && p.y > 0) {
                    p.y--;
                    dir = UP;
                    lastdir = UP;
                    moved = true;
                } else if (ch == KEY_DOWN && p.y < row-1) {
                    p.y++;
                    dir = DOWN;
                    lastdir = DOWN;
                    moved = true;
                } else if (ch == KEY_LEFT && p.x > 0) {
                    p.x--;
                    dir = LEFT;
                    lastdir = LEFT;
                    moved = true;
                } else if (ch == KEY_RIGHT && p.x*2 < col-2) {
                    p.x++;
                    dir = RIGHT;
                    lastdir = RIGHT;
                    moved = true;
                }

                if (moved) {
                    op = array_pop(&points);
                    array_push(points, p);
                }

                update(op,points);

                //printw("%d\n", ch);
            } else if (!automoved && !moved) {
                automoved = true;

                p = array_last(points);

                if (dir == UP && p.y > 0) {
                    p.y--;
                    moved = true;
                } else if (dir == DOWN && p.y < row-1) {
                    p.y++;
                    moved = true;
                } else if (dir == LEFT && p.x > 0) {
                    p.x--;
                    moved = true;
                } else if (dir == RIGHT && p.x*2 < col-2) {
                    p.x++;
                    moved = true;
                }

                if (moved) {
                    op = array_pop(&points);
                    array_push(points, p);
                }

                update(op, points);
            }
        }

        cleararray(keys, 10);
    }
    
    endwin();
    return 0;
}