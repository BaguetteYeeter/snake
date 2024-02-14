#include <stdio.h>
#include <ncurses.h>

typedef struct {
    int x;
    int y;
} pointer;

int main() {
    int row,col;
    int ch;

    pointer p;
    p.x = 0;
    p.y = 0;

    //old pointer, used to clear the space behind
    pointer op;
    op.x = 0;
    op.y = 0;

    initscr();

    getmaxyx(stdscr,row,col);

    raw();
    noecho();
    keypad(stdscr, TRUE);
    cbreak();
    curs_set(0);
    timeout(1000);

    printw("Hello world!\n");
    printw("The terminal is %dx%d", row, col);
    refresh();

    mvchgat(p.y, p.x, 1, A_REVERSE, 0, NULL);

    while (true) {
        ch = getch();
        if (ch != ERR) {
            if (ch == KEY_F(1)) {
                break;
            }

            op.x = p.x;
            op.y = p.y;

            if (ch == KEY_UP) {
                p.y--;
            } else if (ch == KEY_DOWN) {
                p.y++;
            } else if (ch == KEY_LEFT) {
                p.x--;
            } else if (ch == KEY_RIGHT) {
                p.x++;
            }

            mvchgat(op.y, op.x, 1, A_NORMAL, 0, NULL);
            mvchgat(p.y, p.x, 1, A_REVERSE, 0, NULL);

            //printw("%d\n", ch);
        }
    }
    
    endwin();
    return 0;
}