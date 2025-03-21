typedef struct {
	char* key;
	char* value;
} ht_pair;

typedef struct {
	int size;
	int count;
	ht_pair** pairs;
} ht_hash_table;

void ht_del_hash_table(ht_hash_table* ht);  
ht_hash_table* ht_new();
