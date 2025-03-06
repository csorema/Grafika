#ifndef MATRIX_H
#define MATRIX_H

/**
 * Initializes all elements of the matrix to zero.
 */
void init_zero_matrix(float matrix[3][3]);

/**
 * Print the elements of the matrix.
 */
void print_matrix(const float matrix[3][3]);

/**
 * Add matrices.
 */
void add_matrices(const float a[3][3], const float b[3][3], float c[3][3]);

/**
 * Add identity matrix.
 */
void init_identity_matrix(float matrix[3][3]);

/**
 * Add multiply matrix by scalar.
 */
void multiply_matrix_by_scalar(float matrix[3][3], float scalar);

/*
 Add tranform point.
*/
void transform_point(float matrix[3][3], float point[3], float result[3]);

/**
 * Scales the transformation matrix by sx and sy factors.
 */
void scale(float matrix[3][3], float sx, float sy);

/**
 * Shifts the transformation matrix by tx and ty values.
 */
void shift(float matrix[3][3], float tx, float ty);

/**
 * Rotates the transformation matrix by an angle around the origin.
 */
void rotate(float matrix[3][3], float angle);

#endif // MATRIX_H

