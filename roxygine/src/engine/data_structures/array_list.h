#ifndef RG_ARRAY_LIST_H
#define RG_ARRAY_LIST_H

#include "../types.h"

typedef struct array_list_d {
	size_t length;
	size_t capacity;
	size_t element_size;
	void *data;
} array_list_t;

array_list_t *array_list_new(size_t elem_size, size_t init_cap);
void array_list_free(array_list_t *old_array);
u8 array_list_resize(array_list_t *array, size_t new_size);
u8 array_list_shift(array_list_t *array, size_t position, size_t count, s8 shift_by);
size_t array_list_insert(array_list_t *array, size_t position, void *element);
size_t array_list_push(array_list_t *array, void *element);
void *array_list_get(array_list_t *array, size_t position);
u8 array_list_remove(array_list_t *array, size_t position);
u8 array_list_pop(array_list_t *array);

char *array_list_to_string(array_list_t *array);

#endif