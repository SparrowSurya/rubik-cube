#ifndef CUBE_H
#define CUBE_H

#define CUBE_SIZE 3

#include <raylib.h>
#include <stdbool.h>

typedef enum axis_t axis_t;
enum axis_t {
    AXIS_X,
    AXIS_Y,
    AXIS_Z,
};

typedef struct orient_t orient_t;
struct orient_t {
    bool animating;   // Animation indicator
    int total;        // Total animation frames
    int frame;        // Current animation frame
    axis_t axis;      // Axis of rotation
    float delta;      // Target value to be added
    Vector3 rotation; // Value before animation is set. anim state = rotation +
                      // (frame/total)*delta
};

typedef enum face_t face_t;
enum face_t {
    FACE_TOP = 0,    // +y
    FACE_BOTTOM = 1, // -y
    FACE_LEFT = 2,   // -x
    FACE_RIGHT = 3,  // +x
    FACE_FRONT = 4,  // +z
    FACE_BACK = 5,   // -z
    FACE_NONE = 6,
};

typedef enum chunk_type_t chunk_type_t;
enum chunk_type_t {
    CHUNK_CORE = 0,
    CHUNK_CENTER = 1,
    CHUNK_EDGE = 2,
    CHUNK_CORNER = 3,
};

typedef struct chunk_t chunk_t;
struct chunk_t {
    chunk_type_t type; // TYpe of chunk
    face_t face[6];    // ALl six faces of a chunk
    orient_t orient;   // Orientation state of chunk
};

/*
Cube Structure:

Layers (back -> front): [1, 2, 3]

Layer 1:
```
[ 6,  7,  8],
[ 3,  4,  5],
[ 0,  1,  2],
```

Layer 2:
```
[15, 16, 17],
[12, 13, 14],
[ 9, 10, 11],
```

Layer 3:
```
[24, 25, 26],
[21, 22, 23],
[18, 19, 20],
```
 */
typedef struct cube_t cube_t;
struct cube_t {
    chunk_t chunks[27];      // Physical chunks
    bool animating;          // animation state
    face_t rotating_face;    // rotating face
    bool rotating_clockwise; // rotation direction
};

void cube_create(cube_t *cube);
void cube_shuffle(cube_t *cube, int steps);
void cube_update(cube_t *cube);
void cube_rotate_clockwise(cube_t *cube, face_t face);
void cube_rotate_anticlockwise(cube_t *cube, face_t face);
bool cube_complete(cube_t *cube);

#endif // CUBE_H