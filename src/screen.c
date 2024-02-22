#include <ncurses.h>

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