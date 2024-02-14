#include <stdio.h>
#include <ncurses.h>

int main() {
    int row,col;
    int ch;

    initscr();

    getmaxyx(stdscr,row,col);

    raw();
    noecho();
    keypad(stdscr, TRUE);
    cbreak();
    timeout(1000);

    printw("Hello world!\n");
    printw("The terminal is %dx%d", row, col);
    refresh();

    while (true) {
        ch = getch();
        if (ch != ERR) {
            if (ch == KEY_F(1)) {
                break;
            }
            printw("%d\n", ch);
        }
    }
    
    endwin();
    return 0;
}