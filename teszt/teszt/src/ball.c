#include "ball.h"
#include "load.h"
#include "draw.h"

#include <GL/gl.h>
#include <math.h>




#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif


void init_ball(Ball* ball, float x, float y) {
    ball->x = x;
    ball->y = y;
    ball->radius = 30;
    ball->speed_x = 300;
    ball->speed_y = 300;
    ball->angle = 0.0f;
    ball->rotation_speed = INITIAL_ROTATION_SPEED;
    ball->texture_id = 0;

    init_model(&ball->model);

    if (!load_model(&ball->model, "assets/models/cube.obj")) {
        printf("ERROR: Failed to load ball model 'assets/models/ball.obj'\n");
    }
}

void update_ball(Ball* ball, double time)
{
    ball->x += ball->speed_x * time;
    ball->y += ball->speed_y * time;

    ball->angle += ball->rotation_speed * time;
    
    ball->angle = fmodf(ball->angle, 360.0f);
    if (ball->angle < 0.0f) {
        ball->angle += 360.0f;
    }
}

void render_ball(Ball* ball) {
    glPushMatrix();

    glTranslatef(ball->x, ball->y, -0.99f); 

    glDisable(GL_LIGHTING);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glColor4f(0.0f, 0.0f, 0.0f, 0.4f);

    float shadow_radius = ball->radius * 0.9f;
    int num_segments = 24;

    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(0.0f, 0.0f);
    for (int i = 0; i <= num_segments; i++) {
        float angle_rad = (float)i / (float)num_segments * 2.0f * M_PI;
        glVertex2f(cosf(angle_rad) * shadow_radius, sinf(angle_rad) * shadow_radius);
    }
    glEnd();

    glDisable(GL_BLEND);
    glEnable(GL_LIGHTING);

    glPopMatrix();

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, ball->texture_id);

    glPushMatrix();
    glTranslatef(ball->x, ball->y, 0.0f);

    glRotatef(ball->angle, 0.0f, 1.0f, 0.0f); 
    glScalef(ball->radius, ball->radius, ball->radius);
    glColor3f(1.0f, 1.0f, 1.0f); 

    draw_model(&ball->model);

    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}

/*void render_ball(Ball* ball)
{
    double angle_rad;
    double x_vtx;
    double y_vtx;

    glPushMatrix();
    glTranslatef(ball->x, ball->y, 0.0);
    glRotatef(ball->angle, 0.0, 0.0, 1.0);
    glBegin(GL_TRIANGLE_FAN);
    glColor3f(0.9f, 0.9f, 0.9f);
    glVertex2f(0, 0);
    
    int num_segments = 24;
    for (int i = 0; i < num_segments; i++) {
        angle_rad = (double)i / (double)num_segments * 2.0 * M_PI;
        x_vtx = cos(angle_rad) * ball->radius;
        y_vtx = sin(angle_rad) * ball->radius;
        
        float r = 0.8f + 0.2f * (float)cos(angle_rad + M_PI / 4.0);
        float g = 0.7f + 0.2f * (float)sin(angle_rad + M_PI / 4.0);
        float b = 0.6f;
        glColor3f(r, g, b);
        glVertex2f(x_vtx, y_vtx);
    }
    glEnd();

    glPopMatrix();
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, ball->texture_id);

    glPushMatrix();
    glTranslatef(ball->x, ball->y, 0.0f);
    glRotatef(ball->angle, 0.0f, 0.1f, 1.0f);
    glScalef(ball->radius, ball->radius, ball->radius);
    glColor3f(1.0f, 1.0f, 1.0f);

    draw_model(&ball->model); 

    

    



    glPopMatrix();
    glDisable(GL_TEXTURE_2D); 
} */