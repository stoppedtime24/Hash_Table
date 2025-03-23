#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "hash_table.h"

const int HT_PRIME_1 = 151;
const int HT_PRIME_2 = 157;

static ht_pair HT_DELETED_ITEM = {NULL, NULL};

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

static int ht_get_hash(const char* s, const int num_buckets, const int attempt) {
	const int hash_a = ht_hash(s, HT_PRIME_1, num_buckets);
	const int hash_b = ht_hash(s, HT_PRIME_2, num_buckets);
	return (hash_a + (attempt * (hash_b + 1))) % num_buckets;
}

void ht_insert(ht_hash_table *ht, const char *key, const char *value) {
	ht_pair* pair = ht_new_pair(key, value);
	int index = ht_get_hash(pair->key, ht->size, 0);
	ht_pair* cur_pair = ht->pairs[index];
	int i = 1;
	while (cur_pair != NULL) {
		if (cur_pair != &HT_DELETED_ITEM) {
			if (strcmp(cur_pair->key, key) == 0) {
				ht_del_pair(cur_pair);
				ht->pairs[index] = pair;
				return;
			}
		}
		index = ht_get_hash(pair->key, ht->size, i);
		cur_pair = ht->pairs[index];
		i++;
	}
	ht->pairs[index] = pair;
	ht->count++;
}

char* ht_search(ht_hash_table* ht, const char* key) {
	int index = ht_get_hash(key, ht->size, 0);
	ht_pair* pair = ht->pairs[index];
	int i = 1;
	while (pair != NULL) {
		if (pair != &HT_DELETED_ITEM) {
			if (strcmp(pair->key, key) == 0) {
				return pair->value;
			}
		}
		index = ht_get_hash(key, ht->size, i);
		pair = ht->pairs[index];
		i++;
	}
	return NULL;
}

void ht_delete(ht_hash_table* ht, const char* key) {
	int index = ht_get_hash(key, ht->size, 0);
	ht_pair* pair = ht->pairs[index];
	int i = 1;
	while (pair != NULL) {
		if (pair != &HT_DELETED_ITEM) {
			if (strcmp(pair->key, key) == 0) {
				ht_del_pair(pair);
				ht->pairs[index] = &HT_DELETED_ITEM;
			}
		}
		index = ht_get_hash(key, ht->size, i);
		pair = ht->pairs[index];
		i++;
	}
	ht->count--;
}
