#include "hash_table.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>

#define HT_PRIME1 151
#define HT_PRIME2 157

static ht_item HT_DELETED_ITEM = {NULL, NULL};

static ht_item *ht_new_item(const char *key, const char *value) {
  ht_item *i = malloc(sizeof(ht_item));
  i->key = strdup(key);
  i->value = strdup(value);
  return i;
}

ht_hash_table *ht_new() {
  ht_hash_table *ht = malloc(sizeof(ht_hash_table));

  ht->size = 53;
  ht->count = 0;
  ht->items = calloc((size_t)ht->size, sizeof(ht_item *));
  return ht;
}

static void ht_del_item(ht_item *i) {
  free(i->key);
  free(i->value);
  free(i);
}

void ht_del_hash_table(ht_hash_table *ht) {
  for (int i = 0; i < ht->size; i++) {
    ht_item *item = ht->items[i];
    if (item != NULL) {
      ht_del_item(item);
    }
  }

  free(ht->items);
  free(ht);
}

static int ht_hash(const char *s, const int a, const int m) {
  int hash = 0;
  size_t slen = strlen(s);
  for (int i = 0; i < slen; i++) {
    hash += ((int)pow(a, slen - i - 1)) * s[i];
  }
  hash %= m;
  return hash;
}

static int ht_get_hash(const char *s, const int num_buckets,
                       const int attempt) {
  const int hash_a = ht_hash(s, HT_PRIME1, num_buckets);
  int hash_b = ht_hash(s, HT_PRIME2, num_buckets);

  if (hash_b % num_buckets == 0)
    hash_b = 1;

  return (hash_a + (attempt * (hash_b + 1))) % num_buckets;
}

void ht_insert(ht_hash_table *ht, const char *key, const char *value) {
  ht_item *item = ht_new_item(key, value);

  int hash = ht_get_hash(item->key, ht->size, 0);
  ht_item *cur_item = ht->items[hash];

  int i = 1;
  while (cur_item != NULL && cur_item != &HT_DELETED_ITEM) {
    hash = ht_get_hash(item->key, ht->size, i++);
    cur_item = ht->items[hash];
  }

  ht->items[hash] = item;
  ht->count++;
}

char *ht_search(ht_hash_table *ht, const char *key) {
  int hash = ht_get_hash(key, ht->size, 0);
  int i = 1;
  ht_item *cur_item = ht->items[hash];

  while (cur_item != NULL) {
    if (cur_item->key != NULL && strcmp(key, cur_item->key) == 0) {
      return cur_item->value;
    }

    hash = ht_get_hash(key, ht->size, i++);
    cur_item = ht->items[hash];
  }

  return NULL;
}

void ht_delete(ht_hash_table *ht, const char *key) {
  int hash = ht_get_hash(key, ht->size, 0);
  int i = 1;
  ht_item *cur_item = ht->items[hash];

  while (cur_item != NULL) {
    if (strcmp(key, cur_item->key) == 0) {
      ht->items[hash] = &HT_DELETED_ITEM;
      ht->count--;
      ht_del_item(cur_item);
      return;
    }

    hash = ht_get_hash(key, ht->size, i++);
    cur_item = ht->items[hash];
  }

  return;
}
