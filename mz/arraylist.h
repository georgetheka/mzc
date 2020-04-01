#ifndef __mz_arraylist__
#define __mz_arraylist__

#include <stdio.h>
#include <stdlib.h>
#include "type.h"

typedef struct mz_ArrayList {
  size_t initial_capacity;
  size_t element_size;
  int size;
  int capacity;
  void **array;
} mz_ArrayList;

typedef enum mz_ArrayListSortOption {
  mz_ArrayListSortOptionMerge,
  mz_ArrayListSortOptionHeap,
  mz_ArrayListSortOptionQuick
} mz_ArrayListSortOption;

void *(*mz_arraylist_fn)(const void *);

bool (*mz_arraylist_filter_fn)(const void *);

void *(*mz_arraylist_reduce_fn)(const void *, const void *);

int (*mz_arraylist_comparator_fn)(const void *, const void *);

mz_ArrayList *mz_arraylist_new(size_t initial_capacity, size_t element_size);

void mz_arraylist_free(mz_ArrayList *list);

bool mz_arraylist_append(mz_ArrayList *list, void *element);

bool mz_arraylist_append_range(mz_ArrayList *list, void **elements, unsigned int len);

bool mz_arraylist_insert_at(mz_ArrayList *list, int index, void *element);

bool mz_arraylist_remove_at(mz_ArrayList *list, int index);

bool mz_arraylist_remove_range(mz_ArrayList *list, int from_index, int to_index);

bool mz_arraylist_set(mz_ArrayList *list, int index, void *element);

void *mz_arraylist_get(mz_ArrayList *list, int index);

void **mz_arraylist_get_range(mz_ArrayList *list, int from_index, int to_index);

mz_ArrayList *mz_arraylist_map(mz_ArrayList *list, void *(*mz_arraylist_fn)(const void *));

mz_ArrayList *mz_arraylist_filter(mz_ArrayList *list, bool (*mz_arraylist_filter_fn)(const void *));

size_t mz_arraylist_index_of(mz_ArrayList *list, bool (*mz_arraylist_filter_fn)(const void *));

void *mz_arraylist_reduce(mz_ArrayList *list, void *(*mz_arraylist_reduce_fn)(const void *, const void *));

size_t mz_arraylist_binary_search(mz_ArrayList *list, void *element,
                                  int (*mz_arraylist_comparator_fn)(const void *, const void *));

bool mz_arraylist_sort(mz_ArrayList *list, mz_ArrayListSortOption sort_option,
                       int (*mz_arraylist_comparator_fn)(const void *, const void *));

static inline bool mz_arraylist_insert_first(mz_ArrayList *list, void *element) {
  return mz_arraylist_insert_at(list, 0, element);
}

static inline bool mz_arraylist_remove_last(mz_ArrayList *list) {
  return mz_arraylist_remove_at(list, -1);
}

static inline int mz_arraylist_size(mz_ArrayList *list) {
  return list->size;
}

static inline size_t mz_arraylist_capacity(mz_ArrayList *list) {
  return list->capacity;
}

static inline bool mz_arraylist_is_empty(mz_ArrayList *list) {
  return list->size == 0;
}

#define mzm_arraylist_foreach(L, E, I) void * E = L != NULL && L->size > 0 ? L->array[0] : NULL;\
                                        int I = 0;\
                                        for (; I < L->size; E = L->array[++I])


#endif
