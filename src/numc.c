#include "../include/numc.h"

#include <stdio.h>

#define NUM_OF_ARRAYS 2
#define NDIM 3

int main() {
    printf("NumC: A NumPy-like numerical computing library in C.\n");

    size_t shapes[NUM_OF_ARRAYS][NDIM] = {{10, 1920, 1080}, {8, 16, 32}};
    dtype_t dtypes[NUM_OF_ARRAYS] = {nc_int, nc_float};

    ndarray_t *arrays[NUM_OF_ARRAYS] = {0};
    for (int i = 0; i < NUM_OF_ARRAYS; ++i) {
        arrays[i] = nc_create(shapes[i], NDIM, dtypes[i]);
        nc_display(arrays[i], false);
    }

    // nc_free(&arrays[1]);
    float value = 42.0f;
    size_t indices1[] = {7, 15, 31};
    size_t indices1_retrival[] = {7, 15, 31};
    nc_set(arrays[1], indices1, &value);

    void *retrieved = nc_get(arrays[1], indices1_retrival);
    if (retrieved) {
        printf("nc_get -> %f\n", *(float *)retrieved);
    } else {
        printf("nc_get failed: index out of bounds or invalid access.\n");
    }

    // ARANGE INT
    {
        ndarray_t *fucking_trying_arange = nc_arange(0, 7000, 69, nc_int);
        nc_display(fucking_trying_arange, true);
        nc_free(&fucking_trying_arange);
    }

    // ARANGE FLOAT
    {
        ndarray_t *fucking_trying_arange_float =
            nc_arange(0, 7000, 69.69, nc_float);
        nc_display(fucking_trying_arange_float, false);
        nc_free(&fucking_trying_arange_float);
    }

    // RESHAPE THIS SHIT
    {
        /*
         * TODO: FIX THIS SHIT ASAP
         * ik this example is shit and is doing those mem leaks cause of that
         * inline nc_arange passed in hehe_1, not sure how I'll be handling that
         * case now but will figure it out maybe a flag for inline or maybe a
         * different function for inplace and shit.
         */
        size_t shape[] = {3, 4};
        ndarray_t *fucking_trying_arange_int = nc_arange(0, 12, 1, nc_int);
        nc_display(fucking_trying_arange_int, true);
        ndarray_t *hehe_1 = nc_reshape(nc_arange(0, 12, 1, nc_int), shape, 2);
        ndarray_t *hehe_2 = nc_reshape(fucking_trying_arange_int, shape, 2);
        nc_display(hehe_1, true);
        nc_display(hehe_2, true);
        nc_free(&hehe_1);
        nc_free(&hehe_2);
        nc_free(&fucking_trying_arange_int);
    }

    for (int i = 0; i < NUM_OF_ARRAYS; ++i) {
        nc_free(&arrays[i]);
    }

    return 0;
}
