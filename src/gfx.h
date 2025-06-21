#ifndef GFX_H
#define GFX_H
#include <stdint.h>
#define SCREEN_H 120
#define SCREEN_W 160
typedef struct Point
{
    int32_t x,y,z,h;
}Point;
typedef struct TriIdxs
{
    uint16_t id0,id1,id2,c;
}TriIdxs;

typedef struct Vertex{
    float x,y,z;
}Vertex;
typedef uint16_t Color;
typedef struct Model
{
    const uint16_t vl;
    const uint16_t tl;
    uint32_t uuid;
    Vertex* const vs;
    TriIdxs *const ts;
}Model;
typedef struct Instance{
    Model * const m;
    Vertex pos;
};
float* interpolate(int i0, float d0, int i1, float d1);
void draw_line(Vertex p0, Vertex p1, Color color, uint16_t* fb);
void draw_wireframe_tri(Vertex p0, Vertex p1, Vertex p2, Color color, uint16_t* fb);
void draw_full_tri(Vertex p0,Vertex p1,Vertex p2, Color color, uint16_t* fb);
void draw_shaded_tri(Vertex p0,Vertex p1,Vertex p2, Color color, uint16_t* fb);
void draw_wireframe_cube(Vertex vs[8],uint16_t * fb );
void render_object(Vertex *v, int32_t vtxs_len, TriIdxs* t, int32_t idxs_len, uint16_t* fb);
#endif