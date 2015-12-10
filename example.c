#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "range.h"

int main(int argc, char *argv[]) {
	range_cont_t *cont = range_cont_new();
	range_t *range1 = range_new();
	range_t *tmp;
	range_status status;

	range_cont_set_cmp_func(cont, range_cmp_fn_strcmp, NULL);
	range_cont_add_range(cont, range1);

	status = range_cont_insert(cont, "a");
	assert(status == RANGE_OK);
	status = range_cont_insert(cont, "b");
	assert(status == RANGE_OK);
	status = range_cont_insert(cont, "c");
	assert(status == RANGE_OK);

	tmp = range_cont_choose(cont, "a");
	printf("Created range: %p\n", range1);
	printf("Choosen range: %p\n", tmp);
	printf("range1 min: %s\n", (char *)range1->min);
	printf("range1 max: %s\n", (char *)range1->max);

	range_destroy(range1);
	range_cont_destroy(cont);
	return 0;
}