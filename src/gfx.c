#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include "gfx.h"
float* interpolate(int i0, float d0, int i1, float d1){
    float* values;
    printf("i0 %i\n", i0);
    printf("i1 %i\n", i1);
    printf("d0 %f\n", d0);
    printf("d1 %f\n", d1);
    if(i1 == i0){
        values = calloc(1, sizeof(float));
        *values = d0;
        return values;
    }
    values = calloc((i1-i0), sizeof(float));
    float a = (d1-d0) / abs(i1-i0);
    //printf("a %f\n",a);
    float d = d0;
    for (int i = 0; i < (i1-i0) ; i++)
    {
        values[i] = d;
        d += a;
    }
    return values;
}
void draw_line(const Point point0,const Point point1, uint16_t color, uint16_t* fb){
    Point p0 = point0;
    Point p1 = point1;
    if(abs(p1.x - p0.x)>abs(p1.y - p0.y)){
        if( p0.x > p1.x){
            Point tmp = p1;
            p1 = p0;
            p0 = tmp; 
        }
        float* ys = interpolate(p0.x,(float)p0.y,p1.x,(float)p1.y);
        for(int32_t x = p0.x; x<p1.x ; x++){
            //printf("ys %f\n", ys[x - p0.x]);
            int y = ys[x - p0.x];
            fb[x + (y*SCREEN_W)] = color;
        }
        free(ys);
    }else{
        if( p0.y > p1.y){
            Point tmp = p1;
            p1 = p0;
            p0 = tmp; 
        }
        float* xs = interpolate(p0.y,(float)p0.x,p1.y,(float)p1.x);
        for(int y = p0.y; y < p1.y ; y++){
            //printf("xs %f\n", xs[y - p0.y]);
            int x = xs[y - p0.y];
            fb[x + (y*SCREEN_W)] = color;
        }
        free(xs);
    }
}
void draw_wireframe_tri(const Point p0,const Point p1,const Point p2, uint16_t color, uint16_t* fb){
    draw_line(p0, p1, color, fb);
    draw_line(p1, p2, color, fb);
    draw_line(p2, p0, color, fb);
}
void draw_full_tri(Point p0,Point p1,Point p2, uint16_t color, uint16_t* fb){
    if(p1.y < p0.y){
        Point tmp = p1;
        p1 = p0;
        p0 = tmp; 
    };
    if(p2.y < p0.y){
        Point tmp = p2;
        p2 = p0;
        p0 = tmp; 
    };
    if(p2.y < p1.y){
        Point tmp = p1;
        p1 = p2;
        p2 = tmp; 
    };
    float* x01 = interpolate(p0.y,p0.x,p1.y,p1.x);
    int32_t x01_size = p1.y-p0.y;
    float* x12 = interpolate(p1.y,p1.x,p2.y,p2.x);
    int32_t x12_size = p2.y-p1.y;
    float* x02 = interpolate(p0.y,p0.x,p2.y,p2.x);
    int32_t x02_size = p2.y-p0.y;

    float* x012 = calloc(x01_size+x12_size, sizeof(float));
    memcpy(x012, x01, sizeof(float)*x01_size-1);
    memcpy(x012+(sizeof(float)*(x01_size-1)),x12, sizeof(float)*x12_size);
}

