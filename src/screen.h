#ifndef SCREEN_H
#define SCREEN_H

void init_screen();
void update(pointer op, node *points, pointer fp);
void draw_box(int y, int x, int w, int h, char* text);

#endif