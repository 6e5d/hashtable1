#ifndef INCLUDEGUARD_HASHTABLE1
#define INCLUDEGUARD_HASHTABLE1

typedef struct {
	uint32_t size_key;
	uint32_t size_val;
	uint32_t size_entry;
	size_t elements;
	uint8_t bitshift;
	void* buckets;
} Hashtable1();

void hashtable1(init)(Hashtable1()* result, uint32_t size_key, uint32_t size_val);
void hashtable1(double)(Hashtable1()* table);
void hashtable1(debug)(Hashtable1()* table);
uint8_t hashtable1(insert)(Hashtable1()* table, void* key, void* value);
bool hashtable1(get)(Hashtable1()* table, void* key, void** result);
uint8_t hashtable1(contains)(Hashtable1()* table, void* key);
uint8_t hashtable1(remove)(Hashtable1()* table, void* key);
void hashtable1(deinit)(Hashtable1()* result);

#endif
