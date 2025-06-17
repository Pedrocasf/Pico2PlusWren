#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include "pico/scanvideo/composable_scanline.h"
#include "pico/scanvideo.h"
#include "gfx.h"
float* interpolate(int i0, float d0, int i1, float d1){
    float* values;
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
void draw_line(const Point point0,const Point point1, Color c, uint16_t* fb){
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
            fb[x + (y*SCREEN_W)] = PICO_SCANVIDEO_PIXEL_FROM_RGB8(c.r,c.g,c.b);
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
            fb[x + (y*SCREEN_W)] = PICO_SCANVIDEO_PIXEL_FROM_RGB8(c.r,c.g,c.b);
        }
        free(xs);
    }
}
void draw_wireframe_tri(const Point p0,const Point p1,const Point p2, Color c, uint16_t* fb){
    draw_line(p0, p1, c, fb);
    draw_line(p1, p2, c, fb);
    draw_line(p2, p0, c, fb);
}
void draw_full_tri(Point p0,Point p1,Point p2, Color c, uint16_t* fb){
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
    size_t x01_size = p1.y-p0.y;
    float* x12 = interpolate(p1.y,p1.x,p2.y,p2.x);
    size_t x12_size = p2.y-p1.y;
    
    float* x02 = interpolate(p0.y,p0.x,p2.y,p2.x);
    size_t x02_size = p2.y-p0.y;
    
    float* x012 = calloc(x01_size+x12_size-1, sizeof(float));
    memcpy(x012, x01, sizeof(float)*(x01_size-1));
    memcpy(x012+(x01_size-1),x12, sizeof(float)*x12_size);
    int32_t x012_size = x01_size + x12_size-1;    

    int32_t m = x012_size>>1;
    float *x_left; 
    float *x_right;
    if (x02[m] < x012[m]){
        x_left = x02;
        x_right = x012;
    }else{
        x_left = x012;
        x_right = x02;
    }
    for(int y = p0.y; y < p2.y; y++){
        int counter = 0;
        for(int x = x_left[y-p0.y]; x<x_right[y-p0.y];x++){
            fb[x + (y*SCREEN_W)] = PICO_SCANVIDEO_PIXEL_FROM_RGB8(c.r,c.g,c.b);
            counter++;
        }
    }
}

void draw_shaded_tri(Point p0,Point p1,Point p2, Color c, uint16_t* fb){
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
    float* h01 = interpolate(p0.y,p0.h,p1.y,p1.h);
    size_t e01_size = p1.y-p0.y;
    
    float* x12 = interpolate(p1.y,p1.x,p2.y,p2.x);
    float* h12 = interpolate(p1.y,p1.h,p2.y,p2.h);
    size_t e12_size = p2.y-p1.y;
    
    float* x02 = interpolate(p0.y,p0.x,p2.y,p2.x);
    float* h02 = interpolate(p0.y,p0.h,p2.y,p2.h);
    size_t e02_size = p2.y-p0.y;
    
    float* x012 = calloc(e01_size+e12_size-1, sizeof(float));
    memcpy(x012, x01, sizeof(float)*(e01_size-1));
    memcpy(x012+(e01_size-1),x12, sizeof(float)*e12_size);
 
    float* h012 = calloc(e01_size+e12_size-1, sizeof(float));
    memcpy(h012, h01, sizeof(float)*(e01_size-1));
    memcpy(h012+(e01_size-1),h12, sizeof(float)*e12_size);
    int32_t x012_size = e01_size + e12_size-1;    

    int32_t m = x012_size>>1;
    float *x_left; 
    float *x_right;
    float *h_left;
    float *h_right;
    if (x02[m] < x012[m]){
        x_left = x02;
        h_left = h02;
        x_right = x012;
        h_right = h012;
    }else{
        x_left = x012;
        h_left = h012;
        x_right = x02;
        h_right = h02;
    }

    for(int y = p0.y; y < p2.y; y++){
        int32_t x_l = x_left[y - p0.y];
        int32_t x_r = x_right[y -p0.y];
        float *h_segment = interpolate(x_l, h_left[y - p0.y], x_r, h_right[y-p0.y]);
        for(int x = x_l; x<x_r ;x++){
            float h = h_segment[x - x_l];
            Color shaded_c = {
                c.r * h,
                c.g * h,
                c.b * h
            };
            fb[x + (y*SCREEN_W)] = PICO_SCANVIDEO_PIXEL_FROM_RGB8(c.r,c.g,c.b);
        }
        free(h_segment);
    }
    free(x_left);
    free(h_left);
    free(x_right);
    free(h_right);
    free(x01);
    free(h01);
    free(x12);
    free(h12);
    free(x02);
    free(h02);
    free(x012);
    free(h012);
}

