#include "../include/hashtable1.h"

static uint64_t hashtable1_fnv1a(uint8_t* p, size_t size) {
	const uint64_t prime = 0xcbf29ce484222325;
	uint64_t hash = 0;
	for (size_t i = 0; i < size; p += 1, i += 1) {
		hash ^= *(uint8_t*)p;
		hash *= prime;
	}
	if (hash == 0) {hash = 1;}
	return hash;
}

// bucket_item virtual struct
// type: [uint64_t] [size_key] [size_val]
// content: [hash] [key data] [val data]

void hashtable1_init(Hashtable1* result, uint32_t size_key, uint32_t size_val) {
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

static uint64_t hashtable1_getidx(Hashtable1* table, uint64_t v) {
	uint64_t idx;
	if (table->bitshift == 0) {
		idx = 0;
	} else {
		idx = v >> (64 - table->bitshift);
	}
	return idx;
}

// 0=found, 1=not found, 2+=error
static bool hashtable1_get_entry(Hashtable1* table, void* key, uint64_t idx, void** result) {
	while (1) {
		uint8_t* p_ent = (uint8_t*)table->buckets + table->size_entry * idx;
		uint64_t hbin;
		memcpy(&hbin, p_ent, sizeof(uint64_t));
		if (hbin == 0) {
			*result = p_ent;
			return false;
		}
		if (memcmp(key, p_ent + 8, table->size_key) == 0) {
			*result = p_ent;
			return true;
		}
		// printf("collision!\n");
		idx = (idx + 1) % (1 << table->bitshift);
	}
}

bool hashtable1_get(Hashtable1* table, void* key, void** result) {
	uint8_t **p = (uint8_t**)result;
	uint64_t v = hashtable1_fnv1a(key, table->size_key);
	uint64_t idx = hashtable1_getidx(table, v);
	bool ret = hashtable1_get_entry(table, key, idx, result);
	*p += 8 + table->size_key;
	return ret;
}

uint8_t hashtable1_contains(Hashtable1* table, void* key) {
	void *p;
	bool ret = hashtable1_get(table, key, &p);
	return ret;
}

// insert without doubling
static uint8_t hashtable1_insert2(Hashtable1* table, void* key, void* value) {
	uint8_t* p;
	uint64_t v = hashtable1_fnv1a(key, table->size_key);
	uint64_t idx = hashtable1_getidx(table, v);
	uint8_t ret = hashtable1_get_entry(table, key, idx, (void **)&p);
	memcpy(p, &v, 8);
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
	uint8_t* p = (uint8_t*)old_buckets;
	for (size_t i = 0; i < old_len; i += 1, p += table->size_entry) {
		hashtable1_insert2(table, p + 8, p + table->size_key + 8);
	}
	free(old_buckets);
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
	uint64_t v =hashtable1_fnv1a(key, table->size_key);
	uint64_t idx = hashtable1_getidx(table, v);
	uint8_t ret = hashtable1_get_entry(table, key, idx, &p);
	if (ret == 1) {
		return 1;
	}
	table->elements -= 1;
	memset(p, 0, table->size_entry);
	return 0;
}

void hashtable1_deinit(Hashtable1* result) {
	free(result->buckets);
}
