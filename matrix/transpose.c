#include "../common/math.h"

int transpose(float* mat, float* dst, int rows, int cols, int rowTile,
              int colTile) {
            
    if (rows <= 0 || cols <= 0 || rowTile <= 0 || colTile <= 0) {
        return -1; // Invalid input parameters
    }

    for (int i = 0; i < rows; i += rowTile) {
        int iEnd = min(i+rowTile, rows);
        for (int j = 0; j < cols; j += colTile) {
            int jEnd = min(j+colTile, cols);
            for (int ii = i; ii < iEnd; ++ii) {
                for (int jj = j; jj < jEnd; ++jj) {
                    dst[jj*rows + ii] = mat[ii*cols + jj];
                }
            }
        }
    }

    return 0;
}

int main() {
    int rows = 32;
    int cols = 32;

    float mat[rows * cols];
    for (int i = 0; i < rows * cols; ++i) {
        mat[i] = (float)i;
    }
    float dst [rows * cols];

    (void)transpose(mat, dst, rows, cols, 32, 32);
    return 0;
}