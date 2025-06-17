#ifndef GFX_H
#define GFX_H
#include <stdint.h>
#define SCREEN_H 120
#define SCREEN_W 160
typedef struct Point
{
    int32_t x,y;
}Point;
float* interpolate(int i0, float d0, int i1, float d1);
void draw_line(Point p0, Point p1, uint16_t color, uint16_t* fb);
void draw_wireframe_tri(Point p0, Point p1, Point p2, uint16_t color, uint16_t* fb);
void draw_full_tri(Point p0,Point p1,Point p2, uint16_t color, uint16_t* fb);
#endif