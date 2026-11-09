#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "../common/math.h"

#define ROWS 2048
#define COLS 2048
#define TOTAL_SIZE (ROWS * COLS)

int transpose(float* mat, float* dst, int rows, int cols, int rowTile,
              int colTile) {
    if (rows <= 0 || cols <= 0 || rowTile <= 0 || colTile <= 0) {
        return -1;  // Invalid input parameters
    }

    for (int i = 0; i < rows; i += rowTile) {
        int i_end = i + min(rowTile, rows - i);
        for (int j = 0; j < cols; j += colTile) {
            int j_end = j + min(colTile, cols - j);
            for (int ii = i; ii < i_end; ++ii) {
                for (int jj = j; jj < j_end; ++jj) {
                    dst[jj * rows + ii] = mat[ii * cols + jj];
                }
            }
        }
    }

    return 0;
}

static int parse_positive_int(const char* text, int* value) {
    char* end;
    errno = 0;
    long parsed = strtol(text, &end, 10);
    if (errno != 0 || end == text || *end != '\0' || parsed <= 0 ||
        parsed > INT_MAX) {
        return -1;
    }
    *value = (int)parsed;
    return 0;
}

int main(int argc, char** argv) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <tile_size> <repetitions>\n", argv[0]);
        return EXIT_FAILURE;
    }

    int tile_size;
    if (parse_positive_int(argv[1], &tile_size) != 0 || tile_size < 1 ||
        tile_size > min(ROWS, COLS)) {
        fprintf(stderr, "Invalid tile size: %s\n", argv[1]);
        return EXIT_FAILURE;
    }

    int repetitions;
    if (parse_positive_int(argv[2], &repetitions) != 0 || repetitions < 1) {
        fprintf(stderr, "Invalid repetitions: %s\n", argv[2]);
        return EXIT_FAILURE;
    }

    float* mat = malloc(TOTAL_SIZE * sizeof(float));
    float* dst = malloc(TOTAL_SIZE * sizeof(float));
    if (mat == NULL || dst == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        free(mat);
        free(dst);
        return EXIT_FAILURE;
    }

    for (int i = 0; i < TOTAL_SIZE; ++i) {
        mat[i] = (float)i;
    }

    double total_us = 0;
    for (int r = 0; r < repetitions; ++r) {
        clock_t start = clock();
        if (start == (clock_t)-1) {
            fprintf(stderr, "Failed to get start time\n");
            free(mat);
            free(dst);
            return EXIT_FAILURE;
        }

        int result = transpose(mat, dst, ROWS, COLS, tile_size, tile_size);

        clock_t end = clock();
        if (result != 0 || end == (clock_t)-1) {
            fprintf(stderr, "Failed to get end time\n");
            free(mat);
            free(dst);
            return EXIT_FAILURE;
        }

        for (int i = 0; i < ROWS; ++i) {
            for (int j = 0; j < COLS; ++j) {
                if (dst[j * ROWS + i] != mat[i * COLS + j]) {
                    fprintf(stderr, "Transpose verification failed at (%d, %d)\n",
                            i, j);
                    free(mat);
                    free(dst);
                    return EXIT_FAILURE;
                }
            }
        }

        total_us += (double)(end - start);
    }

    double avg_us = total_us / repetitions;
    printf("%dx%d: %d us\n", tile_size, tile_size, (int)avg_us);

    free(mat);
    free(dst);
    return 0;
}