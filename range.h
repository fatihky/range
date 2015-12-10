#ifndef _RANGE_H_
#define _RANGE_H_

/**
 * Range
 */

typedef struct {
	// minimum key
	void *min;
	// maximum key
	void *max;
	// elements
	void *elems;
	// element count
	int elem_count;
	// custom data
	void *data;
} range_t;

/**
 * Comparator function
 */

typedef int (*range_cmp_fn)(void *a, void *b, void *data);

/**
 * Range container
 * Contains ranges
 */

typedef struct {
	range_t **ranges;
	int num_ranges;

	range_cmp_fn cmp;
	void *cmp_data;
} range_cont_t;

typedef enum {
	RANGE_OK,
	RANGE_ALLOC_FAIL,
	RANGE_NO_RANGE
} range_status;

typedef enum {
	RANGE_TRUE,
	RANGE_FALSE
} range_bool;

range_t *range_new();
void range_init(range_t *range);
void range_deinit(range_t *range);
void range_destroy(range_t *range);

range_bool range_in_range(range_cont_t *cont, range_t *range, void *k);
range_status range_insert(range_cont_t *cont, range_t *range, void *k);

range_cont_t *range_cont_new();
void range_cont_init(range_cont_t *range);
void range_cont_deinit(range_cont_t *range);
void range_cont_destroy(range_cont_t *range);

void range_cont_set_cmp_func(range_cont_t *cont,
	int (cmp)(void *a, void *b, void *data), void *data);
range_status range_cont_add_range(range_cont_t *cont, range_t *range);
range_t *range_cont_choose(range_cont_t *cont, void *k);

int range_cmp_fn_strcmp(void *a, void *b, void *data);

#endif