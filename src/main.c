#include <raylib.h>
#include <rlgl.h>

#include "app.h"
#include "cube.h"


int main(void) {
    cube_t cube = {0};
    cube_create(&cube);

    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE);

    Camera3D camera = {0};
    camera.position = (Vector3){ 10.0f, 10.0f, 10.0f };
    camera.target = (Vector3){ 0.0f, 0.0f, 0.0f };
    camera.fovy = 45.0f;
    camera.projection = CAMERA_PERSPECTIVE;
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };

    Vector3 center = (Vector3){ 0.0f, 0.0f, 0.0f };
    float size = 1.0f;

    SetTargetFPS(APP_FPS);
    DisableCursor();
    rlEnableDepthTest();

    while (!WindowShouldClose()) {

        UpdateCamera(&camera, CAMERA_THIRD_PERSON);

        BeginDrawing();
            ClearBackground(APP_BG);

            BeginMode3D(camera);
                draw_cube(&cube, center, size);
                DrawGrid(10, 1.0f);

                DrawLine3D((Vector3){0, 0, 0}, (Vector3){5, 0, 0}, RED);
                DrawLine3D((Vector3){0, 0, 0}, (Vector3){0, 5, 0}, GREEN);
                DrawLine3D((Vector3){0, 0, 0}, (Vector3){0, 0, 5}, BLUE);
            EndMode3D();

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
