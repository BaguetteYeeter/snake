#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <time.h>

#include "struct.h"
#include "arrays.h"
#include "settings.h"
#include "screen.h"

int rand_lim(int limit) {
    int divisor = RAND_MAX/(limit+1);
    int retval;

    do { 
        retval = rand() / divisor;
    } while (retval > limit);

    return retval;
}

pointer point(int x, int y) {
    pointer retval;
    retval.x = x;
    retval.y = y;
    return retval;
}

void update_food(pointer * fp, int row, int col, node * points) {
    while (true) {
        fp->x = rand_lim((int) (col-1)/2);
        fp->y = rand_lim(row-1);
        if (!array_contains(points, *fp)) {
            break;
        }
    }
}

int main() {
    //define some basic vars
    int row,col;
    int ch;
    enum Directions dir = RIGHT;
    bool moved, automoved;
    int dontgrow = 0;

    struct timespec time1, time2;
    double timetaken;
    int keys[10];
    cleararray(keys, 10);

    //set the random seed
    srand(time(NULL));
    
    //main pointer (obviously)
    pointer p = point(0, 0);

    //old pointer, used to clear the space behind
    pointer op = point(0, 0);

    //beginning of points linked list
    node * points = NULL;
    points = (node *) malloc(sizeof(node));
    if (points == NULL) {
        return 1;
    }

    //first value in linked list
    pointer head = point(0, 0);
    points->val = head;
    points->next = NULL;

    //add however many nodes to make the initial length
    pointer p1;
    p1.y = 0;
    for (int i = 0; i < INITIAL_SNAKE_LENGTH-1; i++) {
        p1.x = i+1;
        array_push(points, p1);
    }
    
    //start ncurses
    init_screen();

    getmaxyx(stdscr,row,col);

    printw("Hello world!\n");
    printw("The terminal is %dx%d", row, col);
    refresh();

    //food pointer, where the food is
    pointer fp;
    fp.x = rand_lim((int) (col-1)/2);
    fp.y = rand_lim(row-1);

    update(op, points, fp);

    //time between ticks
    timetaken = 0;

    while (true) {
        moved = false;
        automoved = false;

        //get the time it took to run get character
        clock_gettime(CLOCK_MONOTONIC, &time1);
        ch = getch();
        clock_gettime(CLOCK_MONOTONIC, &time2);

        //dont allow duplicate keys
        if (!valueinarray(ch, keys, 10) && ch != ERR) {
            addtoarray(ch, keys, 10);
        }

        //get the time it took
        double elapsed = (time2.tv_sec - time1.tv_sec) + (time2.tv_nsec - time1.tv_nsec) / 1e9;

        timetaken += elapsed;

        //if it took too short, run it all again
        if (timetaken * 1000 < DELAY * DELAY_MULTIPLIER) {
            continue;
        } else {
            timetaken = 0;
        }

        //for every key
        for (int i = 0; i < 10; i++) {
            ch = keys[i];

            if (ch != ERR) {
                //exit key, might change tho
                if (ch == KEY_F(1)) {
                    break;
                } else if (ch == KEY_RESIZE) {
                    getmaxyx(stdscr,row,col);
                    mvprintw(1, 0, "The terminal is %dx%d", row, col);

                    //make new food pointer as it might be outside the screen
                    mvchgat(fp.y, fp.x*2, 2, A_NORMAL, 0, NULL);
                    update_food(&fp, row, col, points);
                }

                //head of the snake is the last point
                p = array_last(points);

                //this code is too obvious for a comment
                if (ch == KEY_UP && p.y > 0 && dir != DOWN) {
                    p.y--;
                    dir = UP;
                    moved = true;
                } else if (ch == KEY_DOWN && p.y < row-1 && dir != UP) {
                    p.y++;
                    dir = DOWN;
                    moved = true;
                } else if (ch == KEY_LEFT && p.x > 0 && dir != RIGHT) {
                    p.x--;
                    dir = LEFT;
                    moved = true;
                } else if (ch == KEY_RIGHT && p.x*2 < col-2 && dir != LEFT) {
                    p.x++;
                    dir = RIGHT;
                    moved = true;
                }

                if (moved) {
                    //dontgrow is for when food is eaten
                    if (dontgrow > 0) {
                        dontgrow--;
                    } else {
                        op = array_pop(&points);
                    }

                    //if our head is at a food piece
                    if (p.x == fp.x && p.y == fp.y) {
                        dontgrow += 3;
                        update_food(&fp, row, col, points);
                    }
                    array_push(points, p);
                }

                update(op, points, fp);

            } else if (!automoved && !moved) {
                //all of this is basically the same
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
                        update_food(&fp, row, col, points);
                    }
                    array_push(points, p);
                }

                update(op, points, fp);
            } //end of else if
        } // end of for loop

        cleararray(keys, 10);
        
    } // end of main while loop
    
    endwin();
    return 0;
}