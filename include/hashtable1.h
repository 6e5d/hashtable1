#ifndef INCLUDEGUARD_HASHTABLE1
#define INCLUDEGUARD_HASHTABLE1

typedef struct {
	uint32_t size_key;
	uint32_t size_val;
	uint32_t size_entry;
	size_t elements;
	uint8_t bitshift;
	void* buckets;
} Hashtable1;

void hashtable1_init(Hashtable1* result, uint32_t size_key, uint32_t size_val);
void hashtable1_double(Hashtable1* table);
uint8_t hashtable1_insert(Hashtable1* table, void* key, void* value);
bool hashtable1_get(Hashtable1* table, void* key, void** result);
uint8_t hashtable1_contains(Hashtable1* table, void* key);
uint8_t hashtable1_remove(Hashtable1* table, void* key);
void hashtable1_deinit(Hashtable1* result);

#endif
