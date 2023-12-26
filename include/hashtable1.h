#ifndef INCLUDEGUARD_HASHTABLE1
#define INCLUDEGUARD_HASHTABLE1
#define NS_NAME(x) com_6e5d_hashtable1_##x
#define NS_TYPE(x) Com_6e5dHashtable1##x

typedef struct {
	uint32_t size_key;
	uint32_t size_val;
	uint32_t size_entry;
	size_t elements;
	uint8_t bitshift;
	void* buckets;
} NS_TYPE();

void NS_NAME(init)(NS_TYPE()* result, uint32_t size_key, uint32_t size_val);
void NS_NAME(double)(NS_TYPE()* table);
void NS_NAME(debug)(NS_TYPE()* table);
uint8_t NS_NAME(insert)(NS_TYPE()* table, void* key, void* value);
bool NS_NAME(get)(NS_TYPE()* table, void* key, void** result);
uint8_t NS_NAME(contains)(NS_TYPE()* table, void* key);
uint8_t NS_NAME(remove)(NS_TYPE()* table, void* key);
void NS_NAME(deinit)(NS_TYPE()* result);

#undef NS_NAME
#undef NS_TYPE
#endif
