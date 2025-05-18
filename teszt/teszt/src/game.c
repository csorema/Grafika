#include "game.h"
#include "pong.h"

#include <GL/gl.h>
#include <GL/glu.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <math.h>
#include <unistd.h>

bool show_controls = false;
GLuint controls_texture = 0; 

void init_game(Game* game, int width, int height)
{
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        printf("Current working directory: %s\n", cwd);
    } else {
        perror("getcwd() error");
    }

    game->is_running = false;
    game->width = width;
    game->height = height;
    if (init_sdl(game) == false) {
        return;
    }
    init_opengl(game);
    init_pong(&(game->pong), width, height);
    update_score_title(game);

    // load the controls texture
    controls_texture = load_texture("src/text.png");
    if (controls_texture == 0) {
        printf("[ERROR] Failed to load controls texture.\n");
    }

    printf("\n--- Pong Controls ---\n");
    printf("Player 1 (Left Paddle): W (Up), S (Down)\n");
    printf("Player 2 (Right Paddle): Mouse Y-axis\n");
    printf("Ball Size: UP Arrow (Increase), DOWN Arrow (Decrease)\n");
    printf("Move Ball: Left Mouse Click\n");
    printf("Quit: ESC\n");
    printf("----------------------\n\n");

    game->last_update_time = (double)SDL_GetTicks() / 1000;
    game->is_running = true;
}

void destroy_game(Game* game)
{
    // free the controls texture
    if (controls_texture != 0) {
        glDeleteTextures(1, &controls_texture);
        controls_texture = 0;
    }

    if (game->gl_context != NULL) {
        SDL_GL_DeleteContext(game->gl_context);
    }

    if (game->window != NULL) {
        SDL_DestroyWindow(game->window);
    }

    // quit SDL_image
    IMG_Quit();

    SDL_Quit();
}

void render_controls(Game* game)
{
    if (controls_texture == 0) return; // ensure the texture is loaded

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, controls_texture);

    // render the texture as a quad
    glColor3f(1.0f, 1.0f, 1.0f); // set color to white
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex2f(50.0f, 50.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex2f(game->width - 50.0f, 50.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex2f(game->width - 50.0f, game->height - 50.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex2f(50.0f, game->height - 50.0f);
    glEnd();

    glDisable(GL_TEXTURE_2D);
}

void handle_game_events(Game* game)
{
    SDL_Event event;
    //int x, y;

    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_KEYDOWN:
            if (event.key.keysym.scancode == SDL_SCANCODE_F1) {
                show_controls = !show_controls;
                break;
            }

            if (show_controls)
                break;

            switch (event.key.keysym.scancode) {
            case SDL_SCANCODE_ESCAPE:
                game->is_running = false;
                break;
            case SDL_SCANCODE_W:
                set_left_pad_speed(&(game->pong), +300);
                break;
            case SDL_SCANCODE_S:
                set_left_pad_speed(&(game->pong), -300);
                break;
            case SDL_SCANCODE_UP:
                game->pong.ball.radius -= 2.0f;
                if (game->pong.ball.radius > MAX_BALL_RADIUS) {
                    game->pong.ball.radius = MAX_BALL_RADIUS;
                }
                printf("Ball radius increased: %.1f\n", game->pong.ball.radius);
                break;
            case SDL_SCANCODE_DOWN:
                game->pong.ball.radius += 2.0f;
                if (game->pong.ball.radius < MIN_BALL_RADIUS) {
                    game->pong.ball.radius = MIN_BALL_RADIUS;
                }
                printf("Ball radius decreased: %.1f\n", game->pong.ball.radius);
                break;
            default:
                break;
            }
            break;

        case SDL_KEYUP:
            if (!show_controls) {
                switch (event.key.keysym.scancode) {
                case SDL_SCANCODE_W:
                case SDL_SCANCODE_S:
                    set_left_pad_speed(&(game->pong), 0);
                    break;
                default:
                    break;
                }
            }
            break;

            case SDL_MOUSEMOTION:
            if (!show_controls) {
                int mouse_sdl_x, mouse_sdl_y;
                SDL_GetMouseState(&mouse_sdl_x, &mouse_sdl_y);
                
                float world_y_for_mouse = (float)game->height - (float)mouse_sdl_y;
                
                set_right_pad_position(&(game->pong), world_y_for_mouse - game->pong.right_pad.height / 2.0f);
            }
            break;

        case SDL_MOUSEBUTTONDOWN:
            if (!show_controls && event.button.button == SDL_BUTTON_LEFT) {
                game->pong.ball.x = (float)event.button.x;
                game->pong.ball.y = (float)event.button.y;
                printf("Ball position set to (%d, %d)\n", event.button.x, event.button.y);
            }
            break;

        case SDL_QUIT:
            game->is_running = false;
            break;

        default:
            break;
        }
    }
}


void update_game(Game* game)
{
    if (show_controls) {
        return;
    }

    double current_time;
    double elapsed_time;

    current_time = (double)SDL_GetTicks() / 1000;
    elapsed_time = current_time - game->last_update_time;

    game->last_update_time = current_time;

    update_pong(&(game->pong), elapsed_time, game);
}

void render_game(Game* game) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDisable(GL_LIGHTING);
    glEnable(GL_TEXTURE_2D);
    GLuint background_texture = load_texture("src/pongtable.jpg");
    glBindTexture(GL_TEXTURE_2D, background_texture);

    glPushMatrix();
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(0.0f, 0.0f, -1.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f((float)game->width, 0.0f, -1.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f((float)game->width, (float)game->height, -1.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(0.0f, (float)game->height, -1.0f);
    glEnd();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
    glEnable(GL_LIGHTING);

    render_pong(&(game->pong));


    if (show_controls) {
        render_controls(game);
    }

    SDL_GL_SwapWindow(game->window);
}

bool init_sdl(Game* game)
{
    int error_code;

    error_code = SDL_Init(SDL_INIT_EVERYTHING);
    if (error_code != 0) {
        printf("[ERROR] SDL initialization error: %s\n", SDL_GetError());
        return false;
    }

    // initialize SDL_image
    int img_flags = IMG_INIT_PNG | IMG_INIT_JPG;
    if (!(IMG_Init(img_flags) & img_flags)) {
        printf("[ERROR] SDL_image initialization error: %s\n", IMG_GetError());
        return false;
    }

    game->window = SDL_CreateWindow(
        "Pong!",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        game->width, game->height,
        SDL_WINDOW_OPENGL);
    if (game->window == NULL) {
        printf("[ERROR] Unable to create the application window!\n");
        return false;
    }

    game->gl_context = SDL_GL_CreateContext(game->window);
    if (game->gl_context == NULL) {
        printf("[ERROR] Unable to create the OpenGL context!\n");
        return false;
    }

    return true;
}

void init_opengl(Game* game) {
    glShadeModel(GL_SMOOTH);
    glClearColor(0.1f, 0.1f, 0.15f, 1.0f);
    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);

    GLfloat light_ambient[] = {0.3f, 0.3f, 0.3f, 1.0f};
    GLfloat light_diffuse[] = {0.8f, 0.8f, 0.8f, 1.0f};
    GLfloat light_specular[] = {0.5f, 0.5f, 0.5f, 1.0f};

    GLfloat light_position[] = {(float)game->width / 2.0f, (float)game->height, 500.0f, 1.0f}; // Positional light above center

    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

    GLfloat mat_specular[] = { 0.8f, 0.8f, 0.8f, 1.0f };
    GLfloat mat_shininess[] = { 50.0f };
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);


    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    float aspect = (float)game->width / (float)game->height;
    gluPerspective(45.0f, aspect, 10.0f, game->width * 2.0f);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt(
        game->width / 2.0f, game->height / 2.0f - game->height * 0.3f, (double)game->height * 1.2,
        game->width / 2.0f, game->height / 2.0f, 0.0f,
        0.0f, 1.0f, 0.0f
    );

    glViewport(0, 0, game->width, game->height);
}
GLuint load_texture(const char* file_path)
{
    SDL_Surface* surface = IMG_Load(file_path); // use the provided file path
    if (!surface) {
        printf("Failed to load texture '%s': %s\n", file_path, IMG_GetError());

        // create a fallback texture (e.g., a white square)
        GLuint texture_id;
        glGenTextures(1, &texture_id);
        glBindTexture(GL_TEXTURE_2D, texture_id);

        unsigned char fallback_texture[4] = {255, 255, 255, 255}; // White pixel
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, fallback_texture);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        return texture_id;
    }

    GLuint texture_id;
    glGenTextures(1, &texture_id);
    glBindTexture(GL_TEXTURE_2D, texture_id);

    GLenum format = GL_RGB;
    if (surface->format->BytesPerPixel == 4) {
        format = GL_RGBA; // handle images with alpha channel
    }

    glTexImage2D(GL_TEXTURE_2D, 0, format, surface->w, surface->h, 0, format, GL_UNSIGNED_BYTE, surface->pixels);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    SDL_FreeSurface(surface);
    return texture_id;
}