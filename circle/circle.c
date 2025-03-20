#include "circle.h"

#include <math.h>
#include <stdio.h>
#include <string.h>

#define PI 3.141592653589793

void set_circle_data(Circle* circle, double x, double y, double radius, SDL_Color color)
{
    circle->x = x;
    circle->y = y;
    circle->radius = (radius > 0.0) ? radius : NAN;
    circle->color = color;
}

int is_point_inside_circle(const Circle* circle, int px, int py)
{
    double dx = px - circle->x;
    double dy = py - circle->y;
    return (dx * dx + dy * dy) <= (circle->radius * circle->radius);
}

void draw_circle(SDL_Renderer* renderer, const Circle* circle)
{
    SDL_SetRenderDrawColor(renderer, circle->color.r, circle->color.g, circle->color.b, 255);
    
    int x = circle->radius;
    int y = 0;
    int err = 1 - x;

    while (x >= y)
    {
        SDL_RenderDrawPoint(renderer, circle->x + x, circle->y + y);
        SDL_RenderDrawPoint(renderer, circle->x - x, circle->y + y);
        SDL_RenderDrawPoint(renderer, circle->x + x, circle->y - y);
        SDL_RenderDrawPoint(renderer, circle->x - x, circle->y - y);
        SDL_RenderDrawPoint(renderer, circle->x + y, circle->y + x);
        SDL_RenderDrawPoint(renderer, circle->x - y, circle->y + x);
        SDL_RenderDrawPoint(renderer, circle->x + y, circle->y - x);
        SDL_RenderDrawPoint(renderer, circle->x - y, circle->y - x);

        y++;
        if (err < 0)
        {
            err += 2 * y + 1;
        }
        else
        {
            x--;
            err += 2 * (y - x + 1);
        }
    }
}

void draw_marker(SDL_Renderer* renderer, const Circle* circle, char marker)
{
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // Fehér marker
    int cx = (int)circle->x;
    int cy = (int)circle->y;
    int r = (int)circle->radius / 2;

    if (marker == '+')
    {
        SDL_RenderDrawLine(renderer, cx - r, cy, cx + r, cy);
        SDL_RenderDrawLine(renderer, cx, cy - r, cx, cy + r);
    }
    else if (marker == 'x')
    {
        SDL_RenderDrawLine(renderer, cx - r, cy - r, cx + r, cy + r);
        SDL_RenderDrawLine(renderer, cx + r, cy - r, cx - r, cy + r);
    }
}


/*void set_circle_data(Circle* circle, double x, double y, double radius, SDL_Color color) {
    circle->x = x;
    circle->y = y;
    circle->color = color;
    if (radius > 0.0) {
        circle->radius = radius;
    } else {
        circle->radius = NAN;
    }
	/*strncpy(circle->color, color, sizeof(circle->color) - 1);
    circle->color[sizeof(circle->color) - 1] = '\0'; // Ensure null termination
}

void draw_circle(SDL_Renderer *renderer, const Circle *circle) {
    SDL_SetRenderDrawColor(renderer, circle->color.r, circle->color.g, circle->color.b, circle->color.a);
    
    for (int w = 0; w < circle->radius * 2; w++) {
        for (int h = 0; h < circle->radius * 2; h++) {
            int dx = circle->radius - w; 
            int dy = circle->radius - h; 
            if ((dx * dx + dy * dy) <= (circle->radius * circle->radius)) {
                SDL_RenderDrawPoint(renderer, circle->x + dx, circle->y + dy);
            }
        }
    }
}*/


double calc_circle_area(const Circle* circle)
{
    return circle->radius * circle->radius * PI;
}

/**
 * Approximate the circle using a fixed segment count
 */
void draw_circle_by_segments(const Circle* circle, int segments)
{
    if (segments < 3) {
        printf("Segment count should be at least 3!\n");
        return;
    }

    double angle_step = 2.0 * PI / segments;
    double prev_x = circle->x + circle->radius;
    double prev_y = circle->y;

    for (int i = 1; i <= segments; i++)
    {
        double angle = i * angle_step;
        double new_x = circle->x + cos(angle) * circle->radius;
        double new_y = circle->y + sin(angle) * circle->radius;

        printf("Segment: (%.2f, %.2f) -> (%.2f, %.2f)\n", prev_x, prev_y, new_x, new_y);

        prev_x = new_x;
        prev_y = new_y;
    }
}

/**
 * Approximate the circle using a fixed angle step
 */
void draw_circle_by_angle(const Circle* circle, double angle_step_deg)
{
    if (angle_step_deg <= 0 || angle_step_deg >= 180) {
        printf("Invalid angle step! Should be between 1° and 179°\n");
        return;
    }

    double angle_step = angle_step_deg * (PI / 180.0);
    int segments = (int)(2.0 * PI / angle_step);

    draw_circle_by_segments(circle, segments);
}

/**
 * Approximate the circle using a maximum segment length
 */
void draw_circle_by_max_length(const Circle* circle, double max_length)
{
    if (max_length <= 0) {
        printf("Max length must be positive!\n");
        return;
    }

    // Calculate necessary segment count
    int segments = (int)ceil(2.0 * PI * circle->radius / max_length);
    draw_circle_by_segments(circle, segments);
}
