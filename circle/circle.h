#ifndef CIRCLE_H
#define CIRCLE_H

#include <SDL2/SDL.h>

/**
 * Data of a circle object in Descartes coordinate system
 */
typedef struct Circle
{
    double x;
    double y;
    double radius;
    SDL_Color color;
} Circle;

/**
 * Set the data of the circle
 */
void set_circle_data(Circle* circle, double x, double y, double radius, SDL_Color color);

/**
 * Calculate the area of the circle.
 */
double calc_circle_area(const Circle* circle);

/**
 * Draw a circle on the renderer
 */
void draw_circle(SDL_Renderer* renderer, const Circle* circle);

/**
 * Check if a point is inside the circle.
 */
int is_point_inside_circle(const Circle* circle, int px, int py);

/**
 * Draw a + or X inside the circle
 */
void draw_marker(SDL_Renderer* renderer, const Circle* circle, char marker);

#endif // CIRCLE_H
