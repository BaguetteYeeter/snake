#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <time.h>

#define DELAY 20
#define DELAY_MULTIPLIER 8

//temporary while food doesnt exist
#define SNAKE_LENGTH 5

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

bool array_contains(node * head, pointer val) {
    node * current = head;
    while (current->next != NULL) {
        if (current->val.x == val.x && current->val.y == val.y) {
            return true;
        }
        current = current->next;
    }
    return false;
}

void update(pointer op, node *points, pointer fp) {
    node *current = points;

    mvchgat(op.y, op.x*2, 2, A_NORMAL, 0, NULL);

    mvchgat(fp.y, fp.x*2, 2, A_REVERSE, 0, NULL);

    while (current != NULL) {
        mvchgat(current->val.y, current->val.x*2, 2, A_REVERSE, 0, NULL);
        current = current->next;
    }

    refresh();
}

int rand_lim(int limit) {
    int divisor = RAND_MAX/(limit+1);
    int retval;

    do { 
        retval = rand() / divisor;
    } while (retval > limit);

    return retval;
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
    int dontgrow = 0;

    srand(time(NULL));

    struct timespec time1, time2;
    double timetaken;
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
    for (int i = 0; i < SNAKE_LENGTH-1; i++) {
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
    timeout(DELAY);

    printw("Hello world!\n");
    printw("The terminal is %dx%d", row, col);
    refresh();

    //food pointer, where the food is
    pointer fp;
    fp.x = rand_lim((int) (col-1)/2);
    fp.y = rand_lim(row-1);

    update(op, points, fp);

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

        if (timetaken * 1000 < DELAY * DELAY_MULTIPLIER) {
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
                    mvchgat(fp.y, fp.x*2, 2, A_NORMAL, 0, NULL);
                    while (true) {
                        fp.x = rand_lim((int) (col-1)/2);
                        fp.y = rand_lim(row-1);
                        if (!array_contains(points, fp)) {
                            break;
                        }
                    }
                }

                p = array_last(points);

                if (ch == KEY_UP && p.y > 0 && dir != DOWN) {
                    p.y--;
                    dir = UP;
                    lastdir = UP;
                    moved = true;
                } else if (ch == KEY_DOWN && p.y < row-1 && dir != UP) {
                    p.y++;
                    dir = DOWN;
                    lastdir = DOWN;
                    moved = true;
                } else if (ch == KEY_LEFT && p.x > 0 && dir != RIGHT) {
                    p.x--;
                    dir = LEFT;
                    lastdir = LEFT;
                    moved = true;
                } else if (ch == KEY_RIGHT && p.x*2 < col-2 && dir != LEFT) {
                    p.x++;
                    dir = RIGHT;
                    lastdir = RIGHT;
                    moved = true;
                }

                if (moved) {
                    if (dontgrow > 0) {
                        dontgrow--;
                    } else {
                        op = array_pop(&points);
                    }

                    if (p.x == fp.x && p.y == fp.y) {
                        dontgrow += 3;
                        while (true) {
                            fp.x = rand_lim((int) (col-1)/2);
                            fp.y = rand_lim(row-1);
                            if (!array_contains(points, fp)) {
                                break;
                            }
                        }
                    }
                    array_push(points, p);
                }

                update(op, points, fp);

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
                    if (dontgrow > 0) {
                        dontgrow--;
                    } else {
                        op = array_pop(&points);
                    }

                    if (p.x == fp.x && p.y == fp.y) {
                        dontgrow += 3;
                        while (true) {
                            fp.x = rand_lim((int) (col-1)/2);
                            fp.y = rand_lim(row-1);
                            if (!array_contains(points, fp)) {
                                break;
                            }
                        }
                    }
                    array_push(points, p);
                }

                update(op, points, fp);
            }
        }

        cleararray(keys, 10);
    }
    
    endwin();
    return 0;
}