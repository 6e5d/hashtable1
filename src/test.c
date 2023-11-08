#include <assert.h>
#include <stdio.h>

#include "../include/hashtable1.h"

int main() {
	Hashtable1 table;
	hashtable1_new(&table, sizeof(int), sizeof(long));
	int k1 = 5; long v1 = 101;
	hashtable1_insert(&table, &k1, &v1);
	int k2 = 7; long v2 = 102;
	hashtable1_insert(&table, &k2, &v2);
	int k3 = 6; long v3 = 103;
	hashtable1_insert(&table, &k3, &v3);
	int k4 = 7; long v4 = 104;
	hashtable1_insert(&table, &k4, &v4);
	void* v;
	assert(0 == hashtable1_get(&table, &k2, &v));
	printf("%ld\n", *(long*)v);
	assert(0 == hashtable1_remove(&table, &k2));
	assert(1 == hashtable1_remove(&table, &k2));
	hashtable1_destroy(&table);
}
