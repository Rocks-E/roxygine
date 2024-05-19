#include <stdlib.h>
#include <string.h>
#include "../util.h"
#include "array_list.h"

array_list_t *array_list_new(size_t elem_size, size_t init_cap) {
	
	array_list_t *new_array = (array_list_t *)malloc(sizeof(array_list_t));
	
	if(!new_array)
		ERROR_RETURN(NULL, "Could not allocate memory for array list struct\n");
	
	new_array->length = 0;
	
	new_array->capacity = init_cap;
	new_array->element_size = elem_size;
	
	new_array->data = malloc(elem_size * init_cap);
	
	if(!new_array->data)
		ERROR_RETURN(NULL, "Could not allocate memory for array list data\n");
	
	return new_array;
	
}

void array_list_free(array_list_t *old_array) {
	
	if(old_array) {
		if(old_array->data)
			free(old_array->data);
		
		free(old_array);
		
	}
}

u8 array_list_resize(array_list_t *array, size_t new_size) {
	
	// If the current size is greater than the specified capacity, just truncate the array to that size
	// No reason to actually shrink the amount of allocated memory so we can hopefully avoid excessive reallocs in the future
	if(new_size < array->length)
		array->length = new_size;
	// If we are attempting to increase the array's capacity, realloc and update the capacity
	else if(new_size > array->capacity) {
		
		array->capacity = new_size;
		void *new_data = realloc(array->data, new_size * new_size);
		
		// If realloc failed, return 1
		if(!new_data)
			ERROR_RETURN(1, "Could not allocate additional memory for array list\n");
		
		array->data = new_data;
		
	}
	
	return 0;
	
}

// Shift part of an array list left or right
// Assume any necessary elements have been extracted prior or will be copied after calling this
u8 array_list_shift(array_list_t *array, size_t position, size_t count, s8 shift_by) {
	
	// Check the position, count, and array bounds to ensure we can shift
	if(position >= array->length)
		ERROR_RETURN(1, "Array list index out of bounds for shifting\n");
	if((position + shift_by) < 0 || (position + shift_by + count) > array->capacity)
		ERROR_RETURN(2, "Shift exceeds bounds of array list\n");
	
	// Adjust the count if it will exceed the bounds of the array
	if((position + count) >= array->length)
		count = array->length - position;
	
	// Destination: start of the array + the position offset + the amount we want to shift by. Negative shift_by will cause this to move left, positive right
	// Source: start of the array + position offset
	// Size: Passed in or determined count
	memmove((u8 *)array->data + array->element_size * (position + shift_by), (u8 *)array->data + array->element_size * position, array->element_size * count);
	
	return 0;
	
}

size_t array_list_insert(array_list_t *array, size_t position, void *element) {
	
	// If we are already at capacity, double the capacity
	if(array->length == array->capacity)
		if(array_list_resize(array, (array->capacity > 0 ? array->capacity << 1 : 1)))
			return -1;
	
	if(position > array->length)
		position = array->length;
	
	// If we are inserting into already set data, move everything right of the insert point one position further to make room for the new element
	if(position < array->length)
		array_list_shift(array, position, array->length - position - 1, 1);
		//memmove((u8 *)array->data + array->element_size * (position + 1), (u8 *)array->data + array->element_size * position, array->element_size * (array->length - position - 1));
	
	// Copy the new element into the open position in the array
	memcpy((u8 *)array->data + array->element_size * position, element, array->element_size);
	
	array->length++;
	
	return position;
	
}

size_t array_list_push(array_list_t *array, void *element) {
	return array_list_insert(array, array->length + 1, element);
}

void *array_list_get(array_list_t *array, size_t position) {
	
	if(position >= array->length)
		ERROR_RETURN(NULL, "Array list index out of bounds\n");
	
	return (u8 *)array->data + (array->element_size * position);
	
}

u8 array_list_remove(array_list_t *array, size_t position) {
	
	if(position >= array->length)
		ERROR_RETURN(1, "Array list index out of bounds\n");
	
	if(array_list_shift(array, position, array->length - position - 1, -1))
		ERROR_RETURN(2, "Unable to shift array\n");
	
	array->length--;
	
	return 0;
	
}

u8 array_list_pop(array_list_t *array) {
	
	if(array->length)
		array->length--;
	else
		ERROR_RETURN(1, "Array list is empty\n");
	
	return 0;
	
}