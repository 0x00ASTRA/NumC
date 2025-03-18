#include <stdlib.h>
#include <string.h>

#include "../include/numc.h"

/*******************************************************************************
                               PRIVATE FUNCTIONS
*******************************************************************************/
static inline size_t _dtype_size(dtype_t dtype) {
    switch (dtype) {
        case INT32:
            return sizeof(int);
        case FLOAT64:
            return sizeof(float);
        case DOUBLE:
            return sizeof(double);
        default:
            fprintf(stderr, "Invalid dtype\n");
            return 0;
    }
}

/*******************************************************************************
                                PUBLIC FUNCTIONS
*******************************************************************************/
ndarray_t *nc_create(size_t *shape, int ndim, dtype_t dtype) {
    if (ndim <= 0) {
        fprintf(stderr, "can't initialize ndarray of dimention %d", ndim);
        return NULL;
    }
    size_t dtype_size = _dtype_size(dtype);
    _check_zero(dtype_size);

    ndarray_t *array = calloc(1, sizeof(ndarray_t));
    _check_alloc(array);

    array->shape = malloc(ndim * sizeof(size_t));
    array->strides = malloc(ndim * sizeof(size_t));
    _check_alloc(array->shape);
    _check_alloc(array->strides);

    memcpy(array->shape, shape, ndim * sizeof(size_t));

    array->dtype = dtype;
    array->item_size = dtype_size;
    array->ndim = ndim;
    array->total_size = 1;
    array->strides[ndim - 1] = dtype_size;

    for (int i = ndim - 1; i >= 0; --i) {
        array->total_size *= shape[i];
        if (i > 0) {
            array->strides[i - 1] = array->strides[i] * shape[i];
        }
    }

    array->data = calloc(array->total_size, dtype_size);
    _check_alloc(array->data);

    return array;

defer:
    if (array) {
        free(array->data);
        free(array->shape);
        free(array->strides);
        free(array);
    }
    return NULL;
}

void nc_free(ndarray_t **array) {
    if (!array || !(*array)) return;

    free((*array)->data);
    free((*array)->shape);
    free((*array)->strides);
    free((*array));
    *array = NULL;
}

/*
 * We don't need this btw it's just for development purposes but idk might leave
 * this here.
 * */
void nc_display(ndarray_t *array) {
    if (!array) {
        printf("(null array)\n");
        return;
    }
    printf("ndarray_t:\n");
    printf("  ndim: %d\n", array->ndim);
    printf("  shape: [");
    for (int i = 0; i < array->ndim; i++)
        printf("%zu%s", array->shape[i], (i < array->ndim - 1) ? ", " : "");
    printf("]\n");
    printf("  strides: [");
    for (int i = 0; i < array->ndim; i++)
        printf("%zu%s", array->strides[i], (i < array->ndim - 1) ? ", " : "");
    printf("]\n");
    printf("  dtype: %d\n", array->dtype);
    printf("  total_size: %zu\n", array->total_size);
    printf("  item_size: %zu\n", array->item_size);
    printf("  data: [ WE DON'T NEED TO SEE THAT ]\n\n");
}
