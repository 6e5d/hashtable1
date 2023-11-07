#include <stddef.h>
#include <stdint.h>

typedef struct {
	uint32_t size_key;
	uint32_t size_val;
	uint32_t size_entry;
	size_t elements;
	uint8_t bitshift;
	void* buckets;
} Hashtable1;

void hashtable1_new(uint32_t size_key, uint32_t size_val, Hashtable1* result);
void hashtable1_double(Hashtable1* table);
uint8_t hashtable1_insert(Hashtable1* table, void* key, void* value);
uint8_t hashtable1_get(Hashtable1* table, void* key, void** result);
uint8_t hashtable1_remove(Hashtable1* table, void* key);
void hashtable1_destroy(Hashtable1* result);
