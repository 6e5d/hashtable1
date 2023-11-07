#include <assert.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "../include/hashtable1.h"
#include "../include/hash.h"

// bucket_item virtual struct
// type: [uint64_t] [size_key] [size_val]
// content: [hash] [key data] [val data]

void hashtable1_new(uint32_t size_key, uint32_t size_val, Hashtable1* result) {
	uint32_t size_entry = size_key + size_val + 8;
	void* buckets = calloc(1, size_entry);
	assert(buckets != NULL);
	*result = (Hashtable1) {
		.size_key = size_key,
		.size_val = size_val,
		.size_entry = size_entry,
		.elements = 0,
		.bitshift = 0,
		.buckets = buckets,
	};
}

uint64_t hashtable1_getidx(Hashtable1* table, uint64_t v) {
	uint64_t idx;
	if (table->bitshift == 0) {
		idx = 0;
	} else {
		idx = v >> (64 - table->bitshift);
	}
	return idx;
}

// 0=found, 1=not found, 2+=error
uint8_t hashtable1_get_entry(Hashtable1* table, void* key, uint64_t idx, void** result) {
	while (1) {
		void* p_ent = table->buckets + table->size_entry * idx;
		uint64_t hbin = *(uint64_t*)p_ent;
		if (hbin == 0) {
			*result = p_ent;
			return 1;
		}
		if (memcmp(key, p_ent + 8, table->size_key) == 0) {
			*result = p_ent;
			return 0;
		}
		// printf("collision!\n");
		idx = (idx + 1) % (1 << table->bitshift);
	}
}

uint8_t hashtable1_get(Hashtable1* table, void* key, void** result) {
	uint64_t v = fnv1a(key, table->size_key);
	uint64_t idx = hashtable1_getidx(table, v);
	uint8_t ret = hashtable1_get_entry(table, key, idx, result);
	*result += 8 + table->size_key;
	return ret;
}

// insert without doubling
static uint8_t hashtable1_insert2(Hashtable1* table, void* key, void* value) {
	void* p;
	uint64_t v = fnv1a(key, table->size_key);
	uint64_t idx = hashtable1_getidx(table, v);
	uint8_t ret = hashtable1_get_entry(table, key, idx, &p);
	memcpy(p, &v, 8);
	*(uint64_t*)p = v;
	memcpy(p + 8, key, table->size_key);
	memcpy(p + 8 + table->size_key, value, table->size_val);
	table->elements += 1;
	return 1 - ret;
}

void hashtable1_double(Hashtable1* table) {
	size_t old_len = 1 << table->bitshift;
	table->bitshift += 1;
	size_t new_len = 1 << table->bitshift;
	// printf("doubling %zu -> %zu\n", old_len, new_len);
	// replace buckets
	void* old_buckets = table->buckets;
	void* new_buckets = calloc(new_len, table->size_entry);
	assert(new_buckets != NULL);
	table->buckets = new_buckets;
	table->elements = 0;
	void* p = old_buckets;
	for (size_t i = 0; i < old_len; i += 1, p += table->size_entry) {
		hashtable1_insert2(table, p + 8, p + table->size_key + 8);
	}
}

// 0=ok, 1=overwrite
uint8_t hashtable1_insert(Hashtable1* table, void* key, void* value) {
	size_t len = 1 << table->bitshift;
	if (table->elements * 2 >= len) {
		hashtable1_double(table);
	}
	hashtable1_insert2(table, key, value);
	return 0;
}

uint8_t hashtable1_remove(Hashtable1* table, void* key) {
	void* p;
	uint64_t v = fnv1a(key, table->size_key);
	uint64_t idx = hashtable1_getidx(table, v);
	uint8_t ret = hashtable1_get_entry(table, key, idx, &p);
	if (ret == 1) {
		return 1;
	}
	table->elements -= 1;
	memset(p, 0, table->size_entry);
	return 0;
}

void hashtable1_destroy(Hashtable1* result) {
	free(result->buckets);
}