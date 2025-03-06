#include "matrix.h"
#include <math.h>
#include <stdio.h>

void init_zero_matrix(float matrix[3][3])
{
    int i;
    int j;

    for (i = 0; i < 3; ++i) {
        for (j = 0; j < 3; ++j) {
            matrix[i][j] = 0.0;
        }
    }
}

void print_matrix(const float matrix[3][3])
{
    int i;
    int j;

    for (i = 0; i < 3; ++i) {
        for (j = 0; j < 3; ++j) {
            printf("%4.4f ", matrix[i][j]);
        }
        printf("\n");
    }
}

void add_matrices(const float a[3][3], const float b[3][3], float c[3][3])
{
    int i;
    int j;

    for (i = 0; i < 3; ++i) {
        for (j = 0; j < 3; ++j) {
            c[i][j] = a[i][j] + b[i][j];
        }
    }
}

void init_identity_matrix(float matrix[3][3])
{
    int i;
    int j;

    for (i = 0; i < 3; ++i) {
        for (j = 0; j < 3; ++j) {
            matrix[i][j] = (i == j) ? 1.0f : 0.0f;
        }
    }
}

void multiply_matrix_by_scalar(float matrix[3][3], float scalar)
{
    int i;
    int j;

    for (i = 0; i < 3; ++i) {
        for (j = 0; j < 3; ++j) {
            matrix[i][j] *= scalar;
        }
    }
}

void transform_point(float matrix[3][3], float point[3], float result[3]) {
    int i, j;
    for (i = 0; i < 3; ++i) {
        result[i] = 0;
        for (j = 0; j < 3; ++j) {
            result[i] += matrix[i][j] * point[j];
        }
    }
}

void scale(float matrix[3][3], float sx, float sy) {
    float scale_matrix[3][3] = {
        { sx,  0,  0 },
        {  0, sy,  0 },
        {  0,  0,  1 }
    };

    float temp[3][3] = {0};
    add_matrices(matrix, scale_matrix, temp);

    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            matrix[i][j] = temp[i][j];
}

void shift(float matrix[3][3], float tx, float ty) {
    float shift_matrix[3][3] = {
        { 1,  0, tx },
        { 0,  1, ty },
        { 0,  0,  1 }
    };

    float temp[3][3] = {0};
    add_matrices(matrix, shift_matrix, temp);

    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            matrix[i][j] = temp[i][j];
}

void rotate(float matrix[3][3], float angle) {
    float cosA = cos(angle);
    float sinA = sin(angle);

    float rotate_matrix[3][3] = {
        { cosA, -sinA, 0 },
        { sinA,  cosA, 0 },
        {    0,     0, 1 }
    };

    float temp[3][3] = {0};
    add_matrices(matrix, rotate_matrix, temp);

    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            matrix[i][j] = temp[i][j];
}
