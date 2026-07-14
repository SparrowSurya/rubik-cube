#include <raylib.h>
#include <rlgl.h>
#include "app.h"


void DrawCubePro(
    Vector3 pos,
    Vector3 rotation,
    float size,
    Color front,
    Color back,
    Color left,
    Color right,
    Color top,
    Color bottom,
    Color outline,
    float width
) {
    float h = size * 0.5f;
    float hi = h - width;
    float epsilon = 0.005f;

    rlPushMatrix();

    rlRotatef(rotation.x, 1.0f, 0.0f, 0.0f);
    rlRotatef(rotation.y, 0.0f, 1.0f, 0.0f);
    rlRotatef(rotation.z, 0.0f, 0.0f, 1.0f);

    rlTranslatef(pos.x, pos.y, pos.z);


    rlBegin(RL_QUADS);

    // ==========================================
    // Front (+Z)
    // ==========================================
    // Outer Outline
    rlColor4ub(outline.r, outline.g, outline.b, outline.a);
    rlNormal3f(0.0f, 0.0f, 1.0f);
    rlVertex3f(-h, -h,  h);
    rlVertex3f( h, -h,  h);
    rlVertex3f( h,  h,  h);
    rlVertex3f(-h,  h,  h);

    // Inner Sticker
    rlColor4ub(front.r, front.g, front.b, front.a);
    rlNormal3f(0.0f, 0.0f, 1.0f);
    rlVertex3f(-hi, -hi,  h + epsilon);
    rlVertex3f( hi, -hi,  h + epsilon);
    rlVertex3f( hi,  hi,  h + epsilon);
    rlVertex3f(-hi,  hi,  h + epsilon);

    // ==========================================
    // Back (-Z)
    // ==========================================
    // Outer Outline
    rlColor4ub(outline.r, outline.g, outline.b, outline.a);
    rlNormal3f(0.0f, 0.0f, -1.0f);
    rlVertex3f( h, -h, -h);
    rlVertex3f(-h, -h, -h);
    rlVertex3f(-h,  h, -h);
    rlVertex3f( h,  h, -h);

    // Inner Sticker
    rlColor4ub(back.r, back.g, back.b, back.a);
    rlNormal3f(0.0f, 0.0f, -1.0f);
    rlVertex3f( hi, -hi, -h - epsilon);
    rlVertex3f(-hi, -hi, -h - epsilon);
    rlVertex3f(-hi,  hi, -h - epsilon);
    rlVertex3f( hi,  hi, -h - epsilon);

    // ==========================================
    // Left (-X)
    // ==========================================
    // Outer Outline
    rlColor4ub(outline.r, outline.g, outline.b, outline.a);
    rlNormal3f(-1.0f, 0.0f, 0.0f);
    rlVertex3f(-h, -h, -h);
    rlVertex3f(-h, -h,  h);
    rlVertex3f(-h,  h,  h);
    rlVertex3f(-h,  h, -h);

    // Inner Sticker
    rlColor4ub(left.r, left.g, left.b, left.a);
    rlNormal3f(-1.0f, 0.0f, 0.0f);
    rlVertex3f(-h - epsilon, -hi, -hi);
    rlVertex3f(-h - epsilon, -hi,  hi);
    rlVertex3f(-h - epsilon,  hi,  hi);
    rlVertex3f(-h - epsilon,  hi, -hi);

    // ==========================================
    // Right (+X)
    // ==========================================
    // Outer Outline
    rlColor4ub(outline.r, outline.g, outline.b, outline.a);
    rlNormal3f(1.0f, 0.0f, 0.0f);
    rlVertex3f( h, -h,  h);
    rlVertex3f( h, -h, -h);
    rlVertex3f( h,  h, -h);
    rlVertex3f( h,  h,  h);

    // Inner Sticker
    rlColor4ub(right.r, right.g, right.b, right.a);
    rlNormal3f(1.0f, 0.0f, 0.0f);
    rlVertex3f( h + epsilon, -hi,  hi);
    rlVertex3f( h + epsilon, -hi, -hi);
    rlVertex3f( h + epsilon,  hi, -hi);
    rlVertex3f( h + epsilon,  hi,  hi);

    // ==========================================
    // Top (+Y)
    // ==========================================
    // Outer Outline
    rlColor4ub(outline.r, outline.g, outline.b, outline.a);
    rlNormal3f(0.0f, 1.0f, 0.0f);
    rlVertex3f(-h,  h,  h);
    rlVertex3f( h,  h,  h);
    rlVertex3f( h,  h, -h);
    rlVertex3f(-h,  h, -h);

    // Inner Sticker
    rlColor4ub(top.r, top.g, top.b, top.a);
    rlNormal3f(0.0f, 1.0f, 0.0f);
    rlVertex3f(-hi,  h + epsilon,  hi);
    rlVertex3f( hi,  h + epsilon,  hi);
    rlVertex3f( hi,  h + epsilon, -hi);
    rlVertex3f(-hi,  h + epsilon, -hi);

    // ==========================================
    // Bottom (-Y)
    // ==========================================
    // Outer Outline
    rlColor4ub(outline.r, outline.g, outline.b, outline.a);
    rlNormal3f(0.0f, -1.0f, 0.0f);
    rlVertex3f(-h, -h, -h);
    rlVertex3f( h, -h, -h);
    rlVertex3f( h, -h,  h);
    rlVertex3f(-h, -h,  h);

    // Inner Sticker
    rlColor4ub(bottom.r, bottom.g, bottom.b, bottom.a);
    rlNormal3f(0.0f, -1.0f, 0.0f);
    rlVertex3f(-hi, -h - epsilon, -hi);
    rlVertex3f( hi, -h - epsilon, -hi);
    rlVertex3f( hi, -h - epsilon,  hi);
    rlVertex3f(-hi, -h - epsilon,  hi);

    rlEnd();
    rlPopMatrix();
}

Color face_color(face_t face) {
    switch (face) {
        case FACE_FRONT: return CUBE_FRONT;
        case FACE_BACK: return CUBE_BACK;
        case FACE_TOP: return CUBE_TOP;
        case FACE_BOTTOM: return CUBE_BOTTOM;
        case FACE_LEFT: return CUBE_LEFT;
        case FACE_RIGHT: return CUBE_RIGHT;
        default: return ColorAlpha(BLACK, 1.0f);
    }
}

void draw_cube(cube_t* cube, Vector3 center, float size) {
    Vector3 shift = (Vector3) {
        center.x - size,
        center.y - size,
        center.z - size
    };

    for (int z=0; z<3; z++) {
        for (int y=0; y<3; y++) {
            for (int x=0; x<3; x++) {

                int index = x + y * 3 + z * 9;
                chunk_t* chunk = &cube->chunks[index];

                Vector3 offset = (Vector3) {
                    shift.x + x * size,
                    shift.y + y * size,
                    shift.z + z * size,
                };

                Vector3 rotation = chunk->orient.rotation;
                if (chunk->orient.animating)
                {
                    float t = (float)chunk->orient.frame / (float)chunk->orient.total;
                    float delta = chunk->orient.delta * t;

                    switch (chunk->orient.axis)
                    {
                        case AXIS_X: rotation.x += delta; break;
                        case AXIS_Y: rotation.y += delta; break;
                        case AXIS_Z: rotation.z += delta; break;
                    }
                }

                DrawCubePro(
                    offset,
                    rotation,
                    size,
                    face_color(chunk->face[FACE_FRONT]),
                    face_color(chunk->face[FACE_BACK]),
                    face_color(chunk->face[FACE_LEFT]),
                    face_color(chunk->face[FACE_RIGHT]),
                    face_color(chunk->face[FACE_TOP]),
                    face_color(chunk->face[FACE_BOTTOM]),
                    CUBE_OUTLINE,
                    0.05f
                );
            }
        }
    }
}