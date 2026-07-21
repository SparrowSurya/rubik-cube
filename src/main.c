#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <raylib.h>
#include <raymath.h>
#include <rlgl.h>

#include "app.h"
#include "cube.h"

struct shuffle_t {
    bool shuffling;
    int steps;
    int step;
};


int main(void) {
    srand(time(NULL));

    Vector3 center = (Vector3){0.0f, 0.0f, 0.0f};
    float size = 2.0f;

    cube_t cube = {0};
    cube_create(&cube);

    struct shuffle_t shuffle = {0};
    shuffle.steps = 20;
    shuffle.shuffling = false;

    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE);

    Camera3D camera = {0};
    camera.position = (Vector3){size * 5.0f, size * 5.0f, size * 5.0f};
    camera.target = center;
    camera.fovy = 45.0f;
    camera.projection = CAMERA_PERSPECTIVE;
    camera.up = (Vector3){0.0f, 1.0f, 0.0f};

    SetTargetFPS(APP_FPS);
    DisableCursor();
    rlEnableDepthTest();

    char textBuf[1024];

    face_t prev_face = FACE_NONE;

    while (!WindowShouldClose()) {

        cube_update(&cube);

        if (shuffle.shuffling && !cube.animating) {
            face_t rand_face = FACE_NONE;
            while (rand_face == prev_face) {
                rand_face = rand() % 6;
            }
            prev_face = rand_face;
            bool clockwise = rand() % 2;

            if (clockwise) {
                cube_rotate_clockwise(&cube, rand_face);
            } else {
                cube_rotate_anticlockwise(&cube, rand_face);
            }

            shuffle.step += 1;
            if (shuffle.step >= shuffle.steps) {
                shuffle.shuffling = false;
            }
        }

        Vector3 view = Vector3Normalize(Vector3Subtract(camera.target, camera.position));
        float ax = fabsf(view.x);
        float ay = fabsf(view.y);
        float az = fabsf(view.z);

        face_t face;

        if (ax > ay && ax > az) {
            face = view.x >= 0.0f ? FACE_LEFT : FACE_RIGHT;
        } else if (ay > az) {
            face = view.y >= 0.0f ? FACE_BOTTOM : FACE_TOP;
        } else {
            face = view.z >= 0.0f ? FACE_BACK : FACE_FRONT;
        }

        UpdateCamera(&camera, CAMERA_THIRD_PERSON);

        BeginDrawing();
        ClearBackground(APP_BG);

        BeginMode3D(camera);
        draw_cube(&cube, center, size);
        EndMode3D();

        bool complete = cube_complete(&cube);
        DrawText(complete ? "DONE" : "WIP", 0, 0, 16, WHITE);

        EndDrawing();

        if (IsKeyPressed(KEY_R) && !shuffle.shuffling) {
            if (IsKeyDown(KEY_LEFT_SHIFT) | IsKeyDown(KEY_RIGHT_SHIFT)) {
                cube_rotate_anticlockwise(&cube, face);
            } else {
                cube_rotate_clockwise(&cube, face);
            }
        }

        if (IsKeyPressed(KEY_X) && !shuffle.shuffling) {
            shuffle.shuffling = true;
            shuffle.step = 0;
        }
    }

    CloseWindow();
    return 0;
}
