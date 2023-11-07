#include <stdint.h>
#include <stddef.h>

uint64_t fnv1a(void* p, size_t size) {
	const uint64_t prime = 0xcbf29ce484222325;
	uint64_t hash = 0;
	for (size_t i = 0; i < size; p+=1, i+=1) {
		hash ^= *(uint8_t*)p;
		hash *= prime;
	}
	if (hash == 0) {hash = 1;}
	return hash;
}