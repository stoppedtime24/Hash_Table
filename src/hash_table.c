#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "hash_table.h"

static ht_pair* ht_new_pair(const char* k, const char* v) {
	ht_pair* i = malloc(sizeof(ht_pair));
	i->key = strdup(k);
	i->value = strdup(v);
	return i;
}

ht_hash_table* ht_new() {
	ht_hash_table* ht = malloc(sizeof(ht_hash_table));
	
	ht->size = 53;
	ht->count = 0;
	ht->pairs = calloc((size_t)ht->size, sizeof(ht_pair*));

	return ht;
}

static void ht_del_pair(ht_pair* i) {
	free(i->key);
	free(i->value);
	free(i);
}

void ht_del_hash_table(ht_hash_table* ht) {
	for (int i = 0; i < ht->size; i++) {
		ht_pair* pair = ht->pairs[i];
		if (pair != NULL) {
			ht_del_pair(pair);
		}
	}
	free(ht->pairs);
	free(ht);
}

static int ht_hash(const char* s, const int a, const int m) {
	long hash = 0;
	const int len_s = strlen(s);
	for (int i = 0; i < len_s; i++) {
		hash += (long)pow(a, len_s - (i+1)) * s[i];
		hash %= m;
	}
	return hash;
}
