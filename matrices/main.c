#include "matrix.h"
#include <math.h>
#include <stdio.h>

int main(int argc, char* argv[])
{
    float a[3][3] = {
        { 1.0f, -2.0f,  3.0f},
        { 5.0f, -3.0f,  0.0f},
        {-2.0f,  1.0f, -4.0f}
    };
    float b[3][3];
    float c[3][3];
    float egysegmatrix[3][3];
    float scalar = 2.5f;
    float scmatrix[3][3] = {
        { 1.0f, -2.0f,  3.0f},
        { 5.0f, -3.0f,  0.0f},
        {-2.0f,  1.0f, -4.0f}
    };

    init_zero_matrix(b);
    b[1][1] =  8.0f;
    b[2][0] = -3.0f;
    b[2][2] =  5.0f;

    printf("Matrix A:\n");
    print_matrix(a);
    printf("Matrix B:\n");
    print_matrix(b);

    add_matrices(a, b, c);
    printf("Matrix A + B:\n");
    print_matrix(c);

    init_identity_matrix(egysegmatrix);
    printf("Egyszegmatrix:\n");
    print_matrix(egysegmatrix);

    multiply_matrix_by_scalar(scmatrix, scalar);
    printf("Skalarral szorzott matrix:\n");
    print_matrix(scmatrix);

    float point[3] = {2.0f, 3.0f, 1.0f};
    float transformed_point[3];

    transform_point(a, point, transformed_point);
    printf("Transzformalt pont: (%.2f, %.2f)\n", transformed_point[0], transformed_point[1]);

    float transform[3][3];
    init_identity_matrix(transform);
    printf("Eredeti matrix:\n");
    print_matrix(transform);

    scale(transform, 2.0f, 0.5f);
    printf("Skalazott matrix:\n");
    print_matrix(transform);
    transform_point(transform, point, transformed_point);
    printf("Skalazott pont: (%.2f, %.2f)\n", transformed_point[0], transformed_point[1]);

    shift(transform, 3.0f, -2.0f);
    printf("Eltolt matrix:\n");
    print_matrix(transform);
    transform_point(transform, point, transformed_point);
    printf("Eltolt pont: (%.2f, %.2f)\n", transformed_point[0], transformed_point[1]);

    rotate(transform, M_PI / 4);
    printf("Forgatott matrix:\n");
    print_matrix(transform);
    transform_point(transform, point, transformed_point);
    printf("Forgatott pont: (%.2f, %.2f)\n", transformed_point[0], transformed_point[1]);

    return 0;
}
