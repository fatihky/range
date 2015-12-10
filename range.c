#include <string.h>
#include <malloc.h>
#include "range.h"

range_t *range_new() {
	range_t *range = calloc(1, sizeof(range_t));
	return range;
}

void range_init(range_t *range) {
	(void)range;
}

void range_deinit(range_t *range) {
	(void)range;
}

void range_destroy(range_t *range) {
	free(range);
}

range_bool range_in_range(range_cont_t *cont, range_t *range, void *k) {
	int tmp;

	if (range->min == NULL || range->max == NULL)
		return RANGE_TRUE;

	tmp = cont->cmp(range->min, k, cont->data);

	if (tmp == 0)
		return RANGE_TRUE;

	if (tmp < 0)
		return RANGE_FALSE;

	tmp = cont->cmp(range->max, k, cont->data);

	if (tmp == 0)
		return RANGE_TRUE;

	if (tmp > 0)
		return RANGE_FALSE;

	return RANGE_TRUE;
}

range_status range_insert(range_cont_t *cont, range_t *range, void *k) {
	int cres_min = 1;
	int cres_max = -1;

	if (range->min != NULL)
		cres_min = cont->cmp(range->min, k, cont->data);

	if (range->max != NULL)
		cres_max = cont->cmp(range->max, k, cont->data);

	// key equal to min or max key of this range
	// do nothing
	if (cres_min == 0 || cres_max == 0)
		return RANGE_OK;

	// key less smaller than minimum key
	if (cres_min > 0) {
		range->min = k;
		return RANGE_OK;
	}

	// cres_min < 0
	// key greater than minimum key of this range

	// key greater than maximum key
	if (cres_max < 0)
		range->max = k;

	return RANGE_OK;
}

range_cont_t *range_cont_new() {
	range_cont_t *cont = calloc(1, sizeof(range_cont_t));
	return cont;
}

void range_cont_init(range_cont_t *cont) {
	(void)cont;
}

void range_cont_deinit(range_cont_t *cont) {
	(void)cont;
}

void range_cont_destroy(range_cont_t *cont) {
	free(cont);
}

void range_cont_set_cmp_func(range_cont_t *cont,
	int (cmp)(void *a, void *b, void *data), void *data) {

	cont->cmp = cmp;
	cont->data = data;
}

range_status range_cont_add_range(range_cont_t *cont, range_t *range) {
	int old_count = cont->num_ranges;
	int new_count = old_count + 1;
	range_t **ranges = calloc(new_count, sizeof(range_t *));

	if (ranges == NULL)
		return RANGE_ALLOC_FAIL;

	if (old_count > 0) {
		memcpy(ranges, cont->ranges, sizeof(range_t *) * old_count);
		free(cont->ranges);
	}

	ranges[old_count] = range;
	cont->ranges = ranges;
	cont->num_ranges = new_count;

	return RANGE_OK;
}

range_t *range_cont_choose(range_cont_t *cont, void *k) {
	int i = 0;
	for (; i < cont->num_ranges; i++) {
		range_t *range = cont->ranges[i];
		range_bool in_range = range_in_range(cont, range, k);

		if (in_range == RANGE_TRUE)
			return range;
	}
	return NULL;
}

range_status range_cont_insert(range_cont_t *cont, void *k) {
	range_t *range = range_cont_choose(cont, k);
	range_t *tmp_range;
	range_status status;

	if (cont->num_ranges == 0)
		return RANGE_NO_RANGE;

	if (range == NULL) {
		tmp_range = cont->ranges[0];

		if (cont->cmp(tmp_range->min, k, cont->data) > 0)
			range = tmp_range;
	}

	if (range == NULL) {
		tmp_range = cont->ranges[cont->num_ranges - 1];

		if (cont->cmp(tmp_range->max, k, cont->data) < 0)
			range = tmp_range;
	}

	// not expected state
	if (range == NULL)
		return RANGE_NO_RANGE;

	status = range_insert(cont, range, k);

	return status;
}

int range_cmp_fn_strcmp(void *a, void *b, void *data) {
	(void)data;
	return strcmp((char *)a, (char *)b);
}