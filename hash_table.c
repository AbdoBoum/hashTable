#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Math.h>
#include <stdbool.h>
#include "hash_table.h"
#include "prime.h"
#define debug 0
static ht_item HT_DELETED_ITEM = {NULL, NULL};

static int hash(const char* s, const int a, const int m) {
  long hash = 0;
  const int len_s = strlen(s);
  for (int i = 0; i < len_s; i++) {
    hash += (long)pow(a, len_s-(i+1))*s[i];
    hash = hash % m;
  }
  return (int)hash;
}

static int get_hash(const char*s, const int num_buckets, const int attempt) {
  const int hash_a = hash(s, HT_PRIME_1, num_buckets);
  const int hash_b = hash(s, HT_PRIME_2, num_buckets);
  return hash_a + (attempt * (hash_b + 1)) % num_buckets;
}

static ht_item* ht_new_item(const char* k, const char* v) {
  ht_item* item = (ht_item*)malloc(sizeof(ht_item));
  item->key = strdup(k);
  item->value = strdup(v);
  return item;
}

static void ht_del_item(ht_item* item) {
  free(item->key);
  free(item->value);
  free(item);
}

/*
* creating hash tables of a certain size
*/
static ht_hash_table* ht_new_sized(const int base_size) {
  ht_hash_table* ht = (ht_hash_table*)malloc(sizeof(ht_hash_table));
  ht->base_size = base_size;
  ht->size = next_prime(ht->base_size);
  ht->count = 0;
  ht->items = calloc((size_t)ht->size, sizeof(ht_item*));
}

ht_hash_table* ht_new() {
  return ht_new_sized(HT_INITIAL_BASE_SIZE);
}

void ht_del_hash_table(ht_hash_table* ht) {
  for (int i = 0; i < ht->size; i++) {
    ht_item* item = ht->items[i];
    if (item != NULL) {
      ht_del_item(item);
    }
  }
  free(ht->items);
  free(ht);
}

static void ht_resize(ht_hash_table* ht, const int base_size) {
  if (base_size < HT_INITIAL_BASE_SIZE) {
    return;
  }
  ht_hash_table* new_resized = ht_new_sized(base_size);
  for (int i = 0; i < ht->size; i++) {
    ht_item* item = ht->items[i];
    if (item != NULL && item != &HT_DELETED_ITEM) {
      ht_insert_item(new_resized, item);
    }
  }
  ht->base_size = new_resized->base_size;
  ht->count = new_resized->base_size;

  const int tmp_size = ht->size;
  ht->size = new_resized->size;
  new_resized->size = tmp_size;
  ht_item** tmp_items = ht->items;
  ht->items = new_resized->items;
  new_resized->items = tmp_items;
  ht_del_hash_table(new_resized);
}

static void ht_resize_up(ht_hash_table* ht) {
    const int new_size = ht->base_size * 2;
    ht_resize(ht, new_size);
}


static void ht_resize_down(ht_hash_table* ht) {
    const int new_size = ht->base_size / 2;
    ht_resize(ht, new_size);
}

void ht_insert(ht_hash_table* ht, const char* key, const char* value) {
  ht_item* item = ht_new_item(key, value);
  ht_insert_item(ht, item);
}

void ht_delete(ht_hash_table* ht, const char* key) {
  if (debug) {printf("In ht_delete\n");}
  const int load = ht->count * 100 / ht->size;
  if (load < 10) {
    ht_resize_down(ht);
  }
  int index = get_hash(key, ht->size, 0);
  ht_item* current_item = ht->items[index];
  int i = 1;
  while (current_item != NULL) {
    if (current_item != &HT_DELETED_ITEM) {
      if (strcmp(key, current_item->key) == 0) {
        ht_del_item(current_item);
        current_item = &HT_DELETED_ITEM;
        break;
      }
    }
    index = get_hash(key, ht->size, i++);
    current_item = ht->items[index];
  }
  ht->count--;
  if (debug) {printf("Out ht_delete\n");}
  return;
}

void ht_insert_item (ht_hash_table* ht, ht_item* item) {
  if (debug) {printf("In ht_insert_item\n");}
  const int load = ht->count * 100 / ht->size;
  if (load > 70) {
    ht_resize_up(ht);
  }
  int index = get_hash(item->key, ht->size, 0);
  ht_item* current_item = ht->items[index];
  int i = 1;
  while (current_item != NULL) {
    if (current_item != &HT_DELETED_ITEM) {
      if (strcmp(current_item->key, item->key) == 0) {
        ht_delete(ht, current_item->key);
        ht->items[index] = item;
        return;
      }
    }
    index = get_hash(item->key, ht->size, i++);
    current_item = ht->items[index];
  }
  ht->items[index] = item;
  ht->count++;
  if (debug) {printf("out ht_insert_item\n");}
  return;
}

char* ht_get(ht_hash_table* ht, const char* key) {
  int index = get_hash(key, ht->size, 0);
  ht_item* current_item = ht->items[index];
  int i = 1;
  while(current_item != NULL) {
    if (current_item != &HT_DELETED_ITEM) {
      if (strcmp(current_item->key, key) == 0) {
        return current_item->value;
      } else {
        return NULL;
      }
    }
    index = get_hash(key, ht->size, i++);
    current_item = ht->items[index];
  }
  return NULL;
}

bool ht_isEmpty(ht_hash_table* ht) {
  return ht->size == 0 ? true : false;
}

int ht_size(ht_hash_table* ht) {
  return ht->size;
}

int main() {
  ht_hash_table* table = ht_new();
  ht_insert(table, "cat", "animal");
  ht_insert(table, "dog", "hanibal");
  ht_insert(table, "lion", "tanibal");
  printf("%s\n", ht_get(table, "cat"));
  ht_delete(table, "cat");
  printf("%s\n", ht_get(table, "cat"));
  printf("%d\n", table->count);
  return 0;
}
