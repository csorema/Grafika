#include "circle.h"
#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_CIRCLE_COUNT 10

int main(int argc, char* argv[])
{
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window *window = SDL_CreateWindow("Mouse Hover Circles", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_SHOWN);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    Circle circles[MAX_CIRCLE_COUNT];
    int circle_count = 0;

    int running = 1;
    SDL_Event event;
    int mouse_x, mouse_y;

    while (running)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                running = 0;
            }
            if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT)
            {
                if (circle_count < MAX_CIRCLE_COUNT)
                {
                    SDL_Color random_color = { rand() % 256, rand() % 256, rand() % 256, 255 };
                    set_circle_data(&circles[circle_count], event.button.x, event.button.y, 30 + rand() % 20, random_color);
                    circle_count++;
                }
            }
        }

        SDL_GetMouseState(&mouse_x, &mouse_y);

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        for (int i = 0; i < circle_count; i++)
        {
            draw_circle(renderer, &circles[i]);
            if (is_point_inside_circle(&circles[i], mouse_x, mouse_y))
            {
                draw_marker(renderer, &circles[i], '+');  // Vagy próbálkozhatsz 'x' jellel is
            }
        }

        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
