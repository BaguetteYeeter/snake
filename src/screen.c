#include <ncurses.h>
#include <string.h>

#include "settings.h"
#include "struct.h"

void init_screen() {
    initscr();
    raw();
    noecho();
    cbreak();
    keypad(stdscr, TRUE);
    curs_set(0);
    timeout(DELAY);
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

void draw_box(int y, int x, int w, int h, char *text) {
    mvprintw(y, x, "+");
    for (int i = 0; i < w-2; i++) {
        printw("-");
    }
    printw("+");

    for (int i = 0; i < h-2; i++) {
        mvprintw(y+i+1, x, "|");
        for (int j = 0; j < w-2; j++) {
            printw(" ");
        }
        printw("|");
    }

    mvprintw(y+h-1, x, "+");
    for (int i = 0; i < w-2; i++) {
        printw("-");
    }
    printw("+");

    int row = h/2;
    int col = w/2 - strlen(text)/2;

    mvprintw(y+row, x+col, text);

    refresh();
}