#include <stdio.h>
#include "omp.h"

#include "CycleTimer.h"

extern void mandelbrotSerial(
    float x0, float y0, float x1, float y1,
    int width, int height,
    int startRow, int numRows,
    int startCol, int totalColumns,
    int maxIterations,
    int output[]);

//
// MandelbrotThread --
//
// Multi-threaded implementation of mandelbrot set image generation.
// Multi-threading performed via pthreads.
void mandelbrotOmp(
    int numThreads,
    float x0, float y0, float x1, float y1,
    int width, int height,
    int maxIterations, int output[])
{
    const static int MAX_THREADS = 32;

    if (numThreads > MAX_THREADS)
    {
        fprintf(stderr, "Error: Max allowed threads is %d\n", MAX_THREADS);
        exit(1);
    }

#pragma omp parallel num_threads(numThreads)
{
    int threadId = omp_get_thread_num();
    double startTime = CycleTimer::currentSeconds();

    int rows = 0, columns = 0;
    int chunk = 5;
    for (int i = 0; i < height; i += chunk) {
        rows =  height - i < chunk ? height - i : chunk;
        for (int j = chunk*threadId; j < width; j += chunk*numThreads) {
            columns =  width - j < chunk ? width - j : chunk;
            mandelbrotSerial(x0, y0, x1, y1, width, height,i, rows, j, columns, maxIterations, output);
        }
    }

    double endTime = CycleTimer::currentSeconds();
    printf("[Thread# %d]:\t\t[%.3f] ms\n",threadId , (endTime-startTime) * 1000);
}
}   
