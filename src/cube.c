#include "cube.h"
#include <math.h>
#include <stdlib.h>
#include <time.h>

// Helper to cycle 4 chunks in clockwise order: a -> b -> c -> d -> a
static void cycle_chunks(cube_t *cube, int a, int b, int c, int d,
                         bool clockwise) {
    if (clockwise) {
        chunk_t temp = cube->chunks[d];
        cube->chunks[d] = cube->chunks[c];
        cube->chunks[c] = cube->chunks[b];
        cube->chunks[b] = cube->chunks[a];
        cube->chunks[a] = temp;
    } else {
        chunk_t temp = cube->chunks[a];
        cube->chunks[a] = cube->chunks[b];
        cube->chunks[b] = cube->chunks[c];
        cube->chunks[c] = cube->chunks[d];
        cube->chunks[d] = temp;
    }
}

// Helper to rotate the face stickers array when it changes physical axes
static void rotate_face_flags(chunk_t *chunk, axis_t axis, bool clockwise) {
    face_t temp[6];
    for (int i = 0; i < 6; i++) {
        temp[i] = chunk->face[i];
    }

    if (axis == AXIS_X) {
        if (clockwise) {
            // CW around +X: TOP (FACE_TOP) -> BACK (FACE_BACK) -> BOTTOM
            // (FACE_BOTTOM) -> FRONT (FACE_FRONT) -> TOP (FACE_TOP)
            chunk->face[FACE_BACK] = temp[FACE_TOP];
            chunk->face[FACE_BOTTOM] = temp[FACE_BACK];
            chunk->face[FACE_FRONT] = temp[FACE_BOTTOM];
            chunk->face[FACE_TOP] = temp[FACE_FRONT];
        } else {
            // CCW around +X: TOP (FACE_TOP) -> FRONT (FACE_FRONT) -> BOTTOM
            // (FACE_BOTTOM) -> BACK (FACE_BACK) -> TOP (FACE_TOP)
            chunk->face[FACE_FRONT] = temp[FACE_TOP];
            chunk->face[FACE_BOTTOM] = temp[FACE_FRONT];
            chunk->face[FACE_BACK] = temp[FACE_BOTTOM];
            chunk->face[FACE_TOP] = temp[FACE_BACK];
        }
    } else if (axis == AXIS_Y) {
        if (clockwise) {
            // CW around +Y: FRONT (FACE_FRONT) -> LEFT (FACE_LEFT) -> BACK
            // (FACE_BACK) -> RIGHT (FACE_RIGHT) -> FRONT (FACE_FRONT)
            chunk->face[FACE_LEFT] = temp[FACE_FRONT];
            chunk->face[FACE_BACK] = temp[FACE_LEFT];
            chunk->face[FACE_RIGHT] = temp[FACE_BACK];
            chunk->face[FACE_FRONT] = temp[FACE_RIGHT];
        } else {
            // CCW around +Y: FRONT (FACE_FRONT) -> RIGHT (FACE_RIGHT) -> BACK
            // (FACE_BACK) -> LEFT (FACE_LEFT) -> FRONT (FACE_FRONT)
            chunk->face[FACE_RIGHT] = temp[FACE_FRONT];
            chunk->face[FACE_BACK] = temp[FACE_RIGHT];
            chunk->face[FACE_LEFT] = temp[FACE_BACK];
            chunk->face[FACE_FRONT] = temp[FACE_LEFT];
        }
    } else if (axis == AXIS_Z) {
        if (clockwise) {
            // CW around +Z: TOP (FACE_TOP) -> RIGHT (FACE_RIGHT) -> BOTTOM
            // (FACE_BOTTOM) -> LEFT (FACE_LEFT) -> TOP (FACE_TOP)
            chunk->face[FACE_RIGHT] = temp[FACE_TOP];
            chunk->face[FACE_BOTTOM] = temp[FACE_RIGHT];
            chunk->face[FACE_LEFT] = temp[FACE_BOTTOM];
            chunk->face[FACE_TOP] = temp[FACE_LEFT];
        } else {
            // CCW around +Z: TOP (FACE_TOP) -> LEFT (FACE_LEFT) -> BOTTOM
            // (FACE_BOTTOM) -> RIGHT (FACE_RIGHT) -> TOP (FACE_TOP)
            chunk->face[FACE_LEFT] = temp[FACE_TOP];
            chunk->face[FACE_BOTTOM] = temp[FACE_LEFT];
            chunk->face[FACE_RIGHT] = temp[FACE_BOTTOM];
            chunk->face[FACE_TOP] = temp[FACE_RIGHT];
        }
    }
}

static void cube_apply_permutation(cube_t *cube, face_t face, bool clockwise) {
    switch (face) {
    case FACE_FRONT: // Rotate Front Layer (18-26) around Z
        cycle_chunks(cube, 24, 26, 20, 18, clockwise);
        cycle_chunks(cube, 25, 23, 19, 21, clockwise);
        for (int i = 18; i < 27; i++) {
            rotate_face_flags(&cube->chunks[i], AXIS_Z, clockwise);
        }
        break;
    case FACE_BACK: // Rotate Back Layer (0-8) around Z
        cycle_chunks(cube, 8, 6, 0, 2, clockwise);
        cycle_chunks(cube, 7, 3, 1, 5, clockwise);
        for (int i = 0; i < 9; i++) {
            rotate_face_flags(&cube->chunks[i], AXIS_Z, !clockwise);
        }
        break;
    case FACE_LEFT: // Rotate Left Layer around X
        cycle_chunks(cube, 6, 24, 18, 0, clockwise);
        cycle_chunks(cube, 15, 21, 9, 3, clockwise);
        for (int i = 0; i < 27; i += 3) {
            rotate_face_flags(&cube->chunks[i], AXIS_X, !clockwise);
        }
        break;
    case FACE_RIGHT: // Rotate Right Layer around X
        cycle_chunks(cube, 26, 8, 2, 20, clockwise);
        cycle_chunks(cube, 17, 5, 11, 23, clockwise);
        for (int i = 2; i < 27; i += 3) {
            rotate_face_flags(&cube->chunks[i], AXIS_X, clockwise);
        }
        break;
    case FACE_TOP: // Rotate Top Layer around Y
        cycle_chunks(cube, 6, 8, 26, 24, clockwise);
        cycle_chunks(cube, 7, 17, 25, 15, clockwise);
        for (int i = 6; i < 9; i++)
            rotate_face_flags(&cube->chunks[i], AXIS_Y, clockwise);
        for (int i = 15; i < 18; i++)
            rotate_face_flags(&cube->chunks[i], AXIS_Y, clockwise);
        for (int i = 24; i < 27; i++)
            rotate_face_flags(&cube->chunks[i], AXIS_Y, clockwise);
        break;
    case FACE_BOTTOM: // Rotate Bottom Layer around Y
        cycle_chunks(cube, 18, 20, 2, 0, clockwise);
        cycle_chunks(cube, 19, 11, 1, 9, clockwise);
        for (int i = 0; i < 3; i++)
            rotate_face_flags(&cube->chunks[i], AXIS_Y, !clockwise);
        for (int i = 9; i < 12; i++)
            rotate_face_flags(&cube->chunks[i], AXIS_Y, !clockwise);
        for (int i = 18; i < 21; i++)
            rotate_face_flags(&cube->chunks[i], AXIS_Y, !clockwise);
        break;
    default:
        break;
    }
}

void cube_create(cube_t *cube) {
    cube->animating = false;

    // Reset all pieces to blank (FACE_NONE)
    for (int i = 0; i < 27; i++) {
        cube->chunks[i].type = CHUNK_CORE;
        for (int j = 0; j < 6; j++) {
            cube->chunks[i].face[j] = FACE_NONE;
        }
    }

    // Layer 1: z = 0 (BACK)
    cube->chunks[0].type = CHUNK_CORNER;
    cube->chunks[0].face[FACE_BACK] = FACE_BACK;
    cube->chunks[0].face[FACE_BOTTOM] = FACE_BOTTOM;
    cube->chunks[0].face[FACE_LEFT] = FACE_LEFT;

    cube->chunks[1].type = CHUNK_EDGE;
    cube->chunks[1].face[FACE_BACK] = FACE_BACK;
    cube->chunks[1].face[FACE_BOTTOM] = FACE_BOTTOM;

    cube->chunks[2].type = CHUNK_CORNER;
    cube->chunks[2].face[FACE_BACK] = FACE_BACK;
    cube->chunks[2].face[FACE_BOTTOM] = FACE_BOTTOM;
    cube->chunks[2].face[FACE_RIGHT] = FACE_RIGHT;

    cube->chunks[3].type = CHUNK_EDGE;
    cube->chunks[3].face[FACE_BACK] = FACE_BACK;
    cube->chunks[3].face[FACE_LEFT] = FACE_LEFT;

    cube->chunks[4].type = CHUNK_CENTER;
    cube->chunks[4].face[FACE_BACK] = FACE_BACK;

    cube->chunks[5].type = CHUNK_EDGE;
    cube->chunks[5].face[FACE_BACK] = FACE_BACK;
    cube->chunks[5].face[FACE_RIGHT] = FACE_RIGHT;

    cube->chunks[6].type = CHUNK_CORNER;
    cube->chunks[6].face[FACE_BACK] = FACE_BACK;
    cube->chunks[6].face[FACE_TOP] = FACE_TOP;
    cube->chunks[6].face[FACE_LEFT] = FACE_LEFT;

    cube->chunks[7].type = CHUNK_EDGE;
    cube->chunks[7].face[FACE_BACK] = FACE_BACK;
    cube->chunks[7].face[FACE_TOP] = FACE_TOP;

    cube->chunks[8].type = CHUNK_CORNER;
    cube->chunks[8].face[FACE_BACK] = FACE_BACK;
    cube->chunks[8].face[FACE_TOP] = FACE_TOP;
    cube->chunks[8].face[FACE_RIGHT] = FACE_RIGHT;

    // Layer 2: z = 1 (Middle layer)
    cube->chunks[9].type = CHUNK_EDGE;
    cube->chunks[9].face[FACE_LEFT] = FACE_LEFT;
    cube->chunks[9].face[FACE_BOTTOM] = FACE_BOTTOM;

    cube->chunks[10].type = CHUNK_CENTER;
    cube->chunks[10].face[FACE_BOTTOM] = FACE_BOTTOM;

    cube->chunks[11].type = CHUNK_EDGE;
    cube->chunks[11].face[FACE_BOTTOM] = FACE_BOTTOM;
    cube->chunks[11].face[FACE_RIGHT] = FACE_RIGHT;

    cube->chunks[12].type = CHUNK_CENTER;
    cube->chunks[12].face[FACE_LEFT] = FACE_LEFT;

    cube->chunks[13].type = CHUNK_CORE;

    cube->chunks[14].type = CHUNK_CENTER;
    cube->chunks[14].face[FACE_RIGHT] = FACE_RIGHT;

    cube->chunks[15].type = CHUNK_EDGE;
    cube->chunks[15].face[FACE_LEFT] = FACE_LEFT;
    cube->chunks[15].face[FACE_TOP] = FACE_TOP;

    cube->chunks[16].type = CHUNK_CENTER;
    cube->chunks[16].face[FACE_TOP] = FACE_TOP;

    cube->chunks[17].type = CHUNK_EDGE;
    cube->chunks[17].face[FACE_RIGHT] = FACE_RIGHT;
    cube->chunks[17].face[FACE_TOP] = FACE_TOP;

    // Layer 3: z = 2 (FRONT)
    cube->chunks[18].type = CHUNK_CORNER;
    cube->chunks[18].face[FACE_FRONT] = FACE_FRONT;
    cube->chunks[18].face[FACE_BOTTOM] = FACE_BOTTOM;
    cube->chunks[18].face[FACE_LEFT] = FACE_LEFT;

    cube->chunks[19].type = CHUNK_EDGE;
    cube->chunks[19].face[FACE_FRONT] = FACE_FRONT;
    cube->chunks[19].face[FACE_BOTTOM] = FACE_BOTTOM;

    cube->chunks[20].type = CHUNK_CORNER;
    cube->chunks[20].face[FACE_FRONT] = FACE_FRONT;
    cube->chunks[20].face[FACE_BOTTOM] = FACE_BOTTOM;
    cube->chunks[20].face[FACE_RIGHT] = FACE_RIGHT;

    cube->chunks[21].type = CHUNK_EDGE;
    cube->chunks[21].face[FACE_FRONT] = FACE_FRONT;
    cube->chunks[21].face[FACE_LEFT] = FACE_LEFT;

    cube->chunks[22].type = CHUNK_CENTER;
    cube->chunks[22].face[FACE_FRONT] = FACE_FRONT;

    cube->chunks[23].type = CHUNK_EDGE;
    cube->chunks[23].face[FACE_FRONT] = FACE_FRONT;
    cube->chunks[23].face[FACE_RIGHT] = FACE_RIGHT;

    cube->chunks[24].type = CHUNK_CORNER;
    cube->chunks[24].face[FACE_FRONT] = FACE_FRONT;
    cube->chunks[24].face[FACE_TOP] = FACE_TOP;
    cube->chunks[24].face[FACE_LEFT] = FACE_LEFT;

    cube->chunks[25].type = CHUNK_EDGE;
    cube->chunks[25].face[FACE_FRONT] = FACE_FRONT;
    cube->chunks[25].face[FACE_TOP] = FACE_TOP;

    cube->chunks[26].type = CHUNK_CORNER;
    cube->chunks[26].face[FACE_FRONT] = FACE_FRONT;
    cube->chunks[26].face[FACE_TOP] = FACE_TOP;
    cube->chunks[26].face[FACE_RIGHT] = FACE_RIGHT;

    for (int i = 0; i < 27; i++) {
        orient_t *orient = &cube->chunks[i].orient;
        orient->animating = false;
        orient->total = 10;
        orient->delta = 0;
        orient->rotation = (Vector3){0.0f, 0.0f, 0.0f};
    }
}

void cube_shuffle(cube_t *cube, int steps) {
    // Prevent shuffling if the cube is currently animating a rotation
    if (cube->animating)
        return;

    face_t faces[] = {FACE_TOP,   FACE_BOTTOM, FACE_LEFT,
                      FACE_RIGHT, FACE_FRONT,  FACE_BACK};

    for (int i = 0; i < steps; i++) {
        int random_face_index = rand() % 6;
        face_t face = faces[random_face_index];
        bool clockwise = rand() % 2;

        // Apply the physical index and sticker permutation instantly
        cube_apply_permutation(cube, face, clockwise);
    }
}

void cube_update(cube_t *cube) {
    if (!cube->animating)
        return;

    bool still_animating = false;
    for (int i = 0; i < 27; i++) {
        orient_t *orient = &cube->chunks[i].orient;
        if (!orient->animating)
            continue;

        orient->frame += 1;
        if (orient->frame < orient->total) {
            still_animating = true;
        } else {
            orient->animating = false;
        }
    }

    if (!still_animating) {
        cube_apply_permutation(cube, cube->rotating_face,
                               cube->rotating_clockwise);

        for (int i = 0; i < 27; i++) {
            orient_t *orient = &cube->chunks[i].orient;
            orient->rotation = (Vector3){0.0f, 0.0f, 0.0f};
            orient->frame = 0;
            orient->delta = 0.0f;
        }
        cube->animating = false;
    }
}

void cube_rotate_clockwise(cube_t *cube, face_t face) {
    if (cube->animating)
        return;

    cube->animating = true;
    cube->rotating_face = face;
    cube->rotating_clockwise = true;

    switch (face) {
    case FACE_FRONT:
        for (int i = 18; i < 27; i++) {
            cube->chunks[i].orient.animating = true;
            cube->chunks[i].orient.axis = AXIS_Z;
            cube->chunks[i].orient.delta = -90.0f;
            cube->chunks[i].orient.frame = 0;
        }
        break;
    case FACE_BACK:
        for (int i = 0; i < 9; i++) {
            cube->chunks[i].orient.animating = true;
            cube->chunks[i].orient.axis = AXIS_Z;
            cube->chunks[i].orient.delta = 90.0f;
            cube->chunks[i].orient.frame = 0;
        }
        break;
    case FACE_LEFT:
        for (int i = 0; i < 27; i += 3) {
            cube->chunks[i].orient.animating = true;
            cube->chunks[i].orient.axis = AXIS_X;
            cube->chunks[i].orient.delta = 90.0f;
            cube->chunks[i].orient.frame = 0;
        }
        break;
    case FACE_RIGHT:
        for (int i = 2; i < 27; i += 3) {
            cube->chunks[i].orient.animating = true;
            cube->chunks[i].orient.axis = AXIS_X;
            cube->chunks[i].orient.delta = -90.0f;
            cube->chunks[i].orient.frame = 0;
        }
        break;
    case FACE_TOP:
        for (int i = 6; i < 9; i++) {
            cube->chunks[i].orient.animating = true;
            cube->chunks[i].orient.axis = AXIS_Y;
            cube->chunks[i].orient.delta = -90.0f;
            cube->chunks[i].orient.frame = 0;
        }
        for (int i = 15; i < 18; i++) {
            cube->chunks[i].orient.animating = true;
            cube->chunks[i].orient.axis = AXIS_Y;
            cube->chunks[i].orient.delta = -90.0f;
            cube->chunks[i].orient.frame = 0;
        }
        for (int i = 24; i < 27; i++) {
            cube->chunks[i].orient.animating = true;
            cube->chunks[i].orient.axis = AXIS_Y;
            cube->chunks[i].orient.delta = -90.0f;
            cube->chunks[i].orient.frame = 0;
        }
        break;
    case FACE_BOTTOM:
        for (int i = 0; i < 3; i++) {
            cube->chunks[i].orient.animating = true;
            cube->chunks[i].orient.axis = AXIS_Y;
            cube->chunks[i].orient.delta = 90.0f;
            cube->chunks[i].orient.frame = 0;
        }
        for (int i = 9; i < 12; i++) {
            cube->chunks[i].orient.animating = true;
            cube->chunks[i].orient.axis = AXIS_Y;
            cube->chunks[i].orient.delta = 90.0f;
            cube->chunks[i].orient.frame = 0;
        }
        for (int i = 18; i < 21; i++) {
            cube->chunks[i].orient.animating = true;
            cube->chunks[i].orient.axis = AXIS_Y;
            cube->chunks[i].orient.delta = 90.0f;
            cube->chunks[i].orient.frame = 0;
        }
        break;
    default:
        break;
    }
}

void cube_rotate_anticlockwise(cube_t *cube, face_t face) {
    if (cube->animating)
        return;

    cube->animating = true;
    cube->rotating_face = face;
    cube->rotating_clockwise = false;

    switch (face) {
    case FACE_FRONT:
        for (int i = 18; i < 27; i++) {
            cube->chunks[i].orient.animating = true;
            cube->chunks[i].orient.axis = AXIS_Z;
            cube->chunks[i].orient.delta = 90.0f;
            cube->chunks[i].orient.frame = 0;
        }
        break;
    case FACE_BACK:
        for (int i = 0; i < 9; i++) {
            cube->chunks[i].orient.animating = true;
            cube->chunks[i].orient.axis = AXIS_Z;
            cube->chunks[i].orient.delta = -90.0f;
            cube->chunks[i].orient.frame = 0;
        }
        break;
    case FACE_LEFT:
        for (int i = 0; i < 27; i += 3) {
            cube->chunks[i].orient.animating = true;
            cube->chunks[i].orient.axis = AXIS_X;
            cube->chunks[i].orient.delta = -90.0f;
            cube->chunks[i].orient.frame = 0;
        }
        break;
    case FACE_RIGHT:
        for (int i = 2; i < 27; i += 3) {
            cube->chunks[i].orient.animating = true;
            cube->chunks[i].orient.axis = AXIS_X;
            cube->chunks[i].orient.delta = 90.0f;
            cube->chunks[i].orient.frame = 0;
        }
        break;
    case FACE_TOP:
        for (int i = 6; i < 9; i++) {
            cube->chunks[i].orient.animating = true;
            cube->chunks[i].orient.axis = AXIS_Y;
            cube->chunks[i].orient.delta = 90.0f;
            cube->chunks[i].orient.frame = 0;
        }
        for (int i = 15; i < 18; i++) {
            cube->chunks[i].orient.animating = true;
            cube->chunks[i].orient.axis = AXIS_Y;
            cube->chunks[i].orient.delta = 90.0f;
            cube->chunks[i].orient.frame = 0;
        }
        for (int i = 24; i < 27; i++) {
            cube->chunks[i].orient.animating = true;
            cube->chunks[i].orient.axis = AXIS_Y;
            cube->chunks[i].orient.delta = 90.0f;
            cube->chunks[i].orient.frame = 0;
        }
        break;
    case FACE_BOTTOM:
        for (int i = 0; i < 3; i++) {
            cube->chunks[i].orient.animating = true;
            cube->chunks[i].orient.axis = AXIS_Y;
            cube->chunks[i].orient.delta = -90.0f;
            cube->chunks[i].orient.frame = 0;
        }
        for (int i = 9; i < 12; i++) {
            cube->chunks[i].orient.animating = true;
            cube->chunks[i].orient.axis = AXIS_Y;
            cube->chunks[i].orient.delta = -90.0f;
            cube->chunks[i].orient.frame = 0;
        }
        for (int i = 18; i < 21; i++) {
            cube->chunks[i].orient.animating = true;
            cube->chunks[i].orient.axis = AXIS_Y;
            cube->chunks[i].orient.delta = -90.0f;
            cube->chunks[i].orient.frame = 0;
        }
        break;
    default:
        break;
    }
}

bool cube_complete(cube_t *cube) {
    // 1. Check Top Face (Center is chunk 16)
    face_t top_color = cube->chunks[16].face[FACE_TOP];
    int top_indices[] = {6, 7, 8, 15, 16, 17, 24, 25, 26};
    for (int i = 0; i < 9; i++) {
        if (cube->chunks[top_indices[i]].face[FACE_TOP] != top_color)
            return false;
    }

    // 2. Check Bottom Face (Center is chunk 10)
    face_t bottom_color = cube->chunks[10].face[FACE_BOTTOM];
    int bottom_indices[] = {0, 1, 2, 9, 10, 11, 18, 19, 20};
    for (int i = 0; i < 9; i++) {
        if (cube->chunks[bottom_indices[i]].face[FACE_BOTTOM] != bottom_color)
            return false;
    }

    // 3. Check Left Face (Center is chunk 12)
    face_t left_color = cube->chunks[12].face[FACE_LEFT];
    int left_indices[] = {0, 3, 6, 9, 12, 15, 18, 21, 24};
    for (int i = 0; i < 9; i++) {
        if (cube->chunks[left_indices[i]].face[FACE_LEFT] != left_color)
            return false;
    }

    // 4. Check Right Face (Center is chunk 14)
    face_t right_color = cube->chunks[14].face[FACE_RIGHT];
    int right_indices[] = {2, 5, 8, 11, 14, 17, 20, 23, 26};
    for (int i = 0; i < 9; i++) {
        if (cube->chunks[right_indices[i]].face[FACE_RIGHT] != right_color)
            return false;
    }

    // 5. Check Front Face (Center is chunk 22)
    face_t front_color = cube->chunks[22].face[FACE_FRONT];
    for (int i = 18; i < 27; i++) {
        if (cube->chunks[i].face[FACE_FRONT] != front_color)
            return false;
    }

    // 6. Check Back Face (Center is chunk 4)
    face_t back_color = cube->chunks[4].face[FACE_BACK];
    for (int i = 0; i < 9; i++) {
        if (cube->chunks[i].face[FACE_BACK] != back_color)
            return false;
    }

    return true;
}
