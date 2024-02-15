#include <stdio.h>
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

void update(pointer op, pointer p) {
    mvchgat(op.y, op.x*2, 1, A_NORMAL, 0, NULL);
    mvchgat(op.y, op.x*2+1, 1, A_NORMAL, 0, NULL);
    mvchgat(p.y, p.x*2, 1, A_REVERSE, 0, NULL);
    mvchgat(p.y, p.x*2+1, 1, A_REVERSE, 0, NULL);
}

int main() {
    int row,col;
    int ch;
    enum Directions dir = RIGHT;

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
    timeout(500);

    printw("Hello world!\n");
    printw("The terminal is %dx%d", row, col);
    refresh();

    mvchgat(p.y, p.x, 1, A_REVERSE, 0, NULL);
    mvchgat(p.y, p.x*2+1, 1, A_REVERSE, 0, NULL);

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
                dir = UP;
            } else if (ch == KEY_DOWN) {
                p.y++;
                dir = DOWN;
            } else if (ch == KEY_LEFT) {
                p.x--;
                dir = LEFT;
            } else if (ch == KEY_RIGHT) {
                p.x++;
                dir = RIGHT;
            }

            update(op,p);

            //printw("%d\n", ch);
        } else {
            if (dir == UP) {
                op.x = p.x;
                op.y = p.y;
                p.y--;
                update(op, p);
            } else if (dir == DOWN) {
                op.x = p.x;
                op.y = p.y;
                p.y++;
                update(op, p);
            } else if (dir == LEFT) {
                op.x = p.x;
                op.y = p.y;
                p.x--;
                update(op, p);
            } else if (dir == RIGHT) {
                op.x = p.x;
                op.y = p.y;
                p.x++;
                update(op, p);
            }
        }
    }
    
    endwin();
    return 0;
}