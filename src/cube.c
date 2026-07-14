#include "cube.h"


void cube_create(cube_t* cube) {
    cube->chunks[ 0].type = CORNER;
    cube->chunks[ 0].faces = BACK | BOTTOM | LEFT;

    cube->chunks[ 1].type = EDGE;
    cube->chunks[ 1].faces = BACK | BOTTOM;

    cube->chunks[ 2].type = CORNER;
    cube->chunks[ 2].faces = BACK | BOTTOM | RIGHT;

    cube->chunks[ 3].type = EDGE;
    cube->chunks[ 3].faces = BACK | LEFT;

    cube->chunks[ 4].type = CENTER;
    cube->chunks[ 4].faces = BACK;

    cube->chunks[ 5].type = EDGE;
    cube->chunks[ 5].faces = BACK | RIGHT;

    cube->chunks[ 6].type = CORNER;
    cube->chunks[ 6].faces = BACK | TOP | LEFT;

    cube->chunks[ 7].type = EDGE;
    cube->chunks[ 7].faces = BACK | TOP;

    cube->chunks[ 8].type = CORNER;
    cube->chunks[ 8].faces = BACK | TOP | RIGHT;

    cube->chunks[ 9].type = EDGE;
    cube->chunks[ 9].faces = LEFT | BOTTOM;

    cube->chunks[10].type = CENTER;
    cube->chunks[10].faces = BOTTOM;

    cube->chunks[11].type = EDGE;
    cube->chunks[11].faces = BOTTOM | RIGHT;

    cube->chunks[12].type = CENTER;
    cube->chunks[12].faces = LEFT;

    cube->chunks[13].type = CORE;
    cube->chunks[13].faces = 0;

    cube->chunks[14].type = CENTER;
    cube->chunks[14].faces = RIGHT;

    cube->chunks[15].type = EDGE;
    cube->chunks[15].faces = LEFT | TOP;

    cube->chunks[16].type = CENTER;
    cube->chunks[16].faces = TOP;

    cube->chunks[17].type = EDGE;
    cube->chunks[17].faces = RIGHT | TOP;

    cube->chunks[18].type = CORNER;
    cube->chunks[18].faces = FRONT | BOTTOM | LEFT;

    cube->chunks[19].type = EDGE;
    cube->chunks[19].faces = FRONT | BOTTOM;

    cube->chunks[20].type = CORNER;
    cube->chunks[20].faces = FRONT | BOTTOM | RIGHT;

    cube->chunks[21].type = EDGE;
    cube->chunks[21].faces = FRONT | LEFT;

    cube->chunks[22].type = CENTER;
    cube->chunks[22].faces = FRONT;

    cube->chunks[23].type = EDGE;
    cube->chunks[23].faces = FRONT | RIGHT;

    cube->chunks[24].type = CORNER;
    cube->chunks[24].faces = FRONT | TOP | LEFT;

    cube->chunks[25].type = EDGE;
    cube->chunks[25].faces = FRONT | TOP;

    cube->chunks[26].type = CORNER;
    cube->chunks[26].faces = FRONT | TOP | RIGHT;
}

void cube_rotate_clockwise(cube_t* cube, face_t face) {
}

void cube_rotate_anticlockwise(cube_t* cube, face_t face) {}

int cube_complete(cube_t* cube) {
    return 1;
}
