#include <stdio.h>
#include <ncurses.h>

int main() {
    int row,col;

    initscr();

    getmaxyx(stdscr,row,col);

    raw();
    noecho();
    keypad(stdscr, TRUE);

    printw("Hello world!\n");
    printw("The terminal is %dx%d", row, col);
    refresh();
    getch();
    
    endwin();
    return 0;
}