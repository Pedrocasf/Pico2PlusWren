#include <stdlib.h>
#include <stdio.h>
#include "gfx.h"
float* interpolate(int i0, float d0, int i1, float d1){
    float* values = calloc((i1-i0)+1, sizeof(float));
    if(i1 == i0){
        values[0] = d0;
        return values;
    }
    float a = (d1-d0) / (i1-i0);
    float d = d0;
    for (int i = i0; i < i1 ; i++)
    {
        values[i] = d;
        d += a;
    }
    return values;
}
void draw_line(Point p0, Point p1, uint16_t color, uint16_t* fb){
    if(abs(p1.x - p0.x)>abs(p1.y - p0.y)){
        if( p0.x > p1.x){
            Point tmp = p1;
            p1 = p0;
            p0 = tmp; 
        }
        float* ys = interpolate(p0.x,p0.y,p1.x,p1.y);
        
        for(int x = p0.x; x<p1.x ; x++){
            printf("%f\n", ys[x - p0.x]);
            int y = ys[x - p0.x];
            fb[x + (y*SCREEN_W)] = color;
        }
    }else{
        if( p0.y > p1.y){
            Point tmp = p1;
            p1 = p0;
            p0 = tmp; 
        }
        float* xs = interpolate(p0.y,p0.x,p1.y,p1.x);

        for(int y = p0.y; y < p1.y ; y++){
            printf("%f\n", xs[y - p0.y]);
            int x = xs[y - p0.y];
            fb[x + (y*SCREEN_W)] = color;
        }
    }
}
void draw_wireframe_tri(Point p0, Point p1, Point p2, uint16_t color, uint16_t* fb){
    draw_line(p0, p1, color, fb);
    draw_line(p1, p2, color, fb);
    draw_line(p2, p0, color, fb);
}