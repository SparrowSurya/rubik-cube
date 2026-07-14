#ifndef APP_H
#define APP_H

#include "colors.h"
#include "cube.h"

#define WINDOW_HEIGHT 600
#define WINDOW_WIDTH  600
#define WINDOW_TITLE  "Cube"

#define APP_FPS 60
#define APP_BG  APP_BASE

#define CUBE_FRONT    APP_TEXT
#define CUBE_TOP      APP_RED
#define CUBE_BOTTOM   APP_PEACH
#define CUBE_BACK     APP_YELLOW
#define CUBE_LEFT     APP_BLUE
#define CUBE_RIGHT    APP_GREEN
#define CUBE_OUTLINE  APP_MANTLE

void DrawCubePro(Vector3 pos, Vector3 rotation, float size, Color front, Color back, Color left, Color right, Color top, Color bottom, Color outline, float width);
void draw_cube(cube_t* cube, Vector3 center, float size);

#endif // APP_H
