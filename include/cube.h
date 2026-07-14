#ifndef CUBE_H
#define CUBE_H

#define CUBE_SIZE 3

typedef enum face_t face_t;
enum face_t {
    TOP    = (1 << 0), // +y
    BOTTOM = (1 << 1), // -y
    LEFT   = (1 << 2), // -x
    RIGHT  = (1 << 3), // +x
    FRONT  = (1 << 4), // +z
    BACK   = (1 << 5), // -z
};

typedef enum chunk_type_t chunk_type_t;
enum chunk_type_t {
    CORE   =  0,
    CENTER =  1,
    EDGE   =  2,
    CORNER = 3,
};

typedef struct chunk_t chunk_t;
struct chunk_t {
    chunk_type_t type;
    face_t faces;
};

/*
Cube Structure:

Layers (front -> back): [1, 2, 3]

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
    chunk_t chunks[27];
};


void cube_create(cube_t* cube);
void cube_rotate_clockwise(cube_t* cube, face_t face);
void cube_rotate_anticlockwise(cube_t* cube, face_t face);
int cube_complete(cube_t* cube);

#endif // CUBE_H