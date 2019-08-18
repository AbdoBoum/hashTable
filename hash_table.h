/**
 * Author: BOUMAHDI ABDERRAHIM
 */

 #ifndef HASH_TABLE_H
 #define HASH_TABLE_H

int HT_PRIME_1 = 153;
int HT_PRIME_2 = 163;
int HT_INITIAL_BASE_SIZE = 53;

 typedef struct {
   char* key;
   char* value;
 } ht_item;


typedef struct {
  int base_size;
  int size;
  int count;
  ht_item** items;
} ht_hash_table;

ht_hash_table* ht_new(void);
void ht_del_hash_table(ht_hash_table*);

void ht_insert(ht_hash_table*, const char*, const char*); //
void ht_insert_item(ht_hash_table*, ht_item*); //
char* ht_get(ht_hash_table*, const char*); //
void ht_delete(ht_hash_table*, const char*); //
bool ht_isEmpty(ht_hash_table*); //
bool ht_containsKey(ht_hash_table*, const char*);
int ht_size(ht_hash_table*); //
void ht_replace(ht_hash_table*, const char*, const char*);
#endif
