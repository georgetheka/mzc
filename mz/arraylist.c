#include <stdio.h>
#include <stdlib.h>
#include "arraylist.h"
#include "logger.h"
#include "type.h"

bool _mz_arraylist_optimize_capacity(mz_ArrayList *list, int new_size) {
  bool result = true;
  size_t new_capacity = list->initial_capacity;
  while (new_capacity <= new_size) {
    new_capacity *= 2;
  }
  void *array = realloc(list->array, new_capacity * sizeof(void *));
  if (!array) {
    ERROR("could not reallocate memory for arraylist->array");
    result = false;
  } else {
    list->capacity = new_capacity;
    list->array = array;
  }
  return true;
}

int _mz_arraylist_normalize_index(int size, int index) {
  if (index < 0) {
    //if the list is not empty, count from the back of the array
    //otherwise, set the index to zero
    index = size > 0 ? size + index : 0;
  }
  return index;
}

bool _mz_arraylist_is_index_within_range(int size, int index) {
  return 0 <= index && index < size;
}

mz_ArrayList *mz_arraylist_new(size_t initial_capacity, size_t element_size) {
  mz_ArrayList *list = NULL;
  if (initial_capacity < 1) {
    ERROR("invalid initial_capacity for arraylist. initial_capacity must be greater than 1");
  } else {
    list = calloc(1, sizeof(mz_ArrayList));
    if (!list) {
      ERROR("could not allocate memory for arraylist");
    } else {
      list->initial_capacity = initial_capacity;
      list->element_size = element_size;
      list->size = 0;
      list->capacity = initial_capacity;
      list->array = calloc(list->capacity, sizeof(void *));
      if (!list->array) {
        ERROR("could not allocate memory for arraylist->array");
        free(list);
      }
    }
  }
  return list;
}

void mz_arraylist_free(mz_ArrayList *list) {
  if (list) {
    if (list->array) {
      free(list->array);
    }
    free(list);
  }
}

bool mz_arraylist_append(mz_ArrayList *list, void *element) {
  bool result = true;
  if (!_mz_arraylist_optimize_capacity(list, list->size)) {
    ERROR("could not optimize array capacity");
    result = false;
  } else {
    list->array[list->size++] = element;
  }
  return result;
}

bool mz_arraylist_append_range(mz_ArrayList *list, void **elements, unsigned int len) {
  bool result = true;
  if (!_mz_arraylist_optimize_capacity(list, list->size + len)) {
    ERROR("could not optimize array capacity");
    result = false;
  } else {
    for (int i = 0; i < len; i++) {
      list->array[list->size++] = elements[i];
    }
  }
  return result;
}

bool mz_arraylist_insert_at(mz_ArrayList *list, int index, void *element) {
  bool result = false;
  //negative to positive conversion:
  index = _mz_arraylist_normalize_index(list->size, index);
  //if index is out of range or the following is also not true -
  //the list is empty and the index is either 0 or -1
  if (!_mz_arraylist_is_index_within_range(list->size, index) &&
      !(list->size == 0 && (index == 0 || index == -1))) {
    ERROR("index is out of range - %d", index);
  } else {
    if (!_mz_arraylist_optimize_capacity(list, list->size)) {
      ERROR("could not optimize capacity");
    } else {
      for (int i = list->size - 1; i >= index; i--) {
        list->array[i + 1] = list->array[i];
      }
      list->size += 1;
      list->array[index] = element;
      result = true;
    }
  }
  return result;
}

bool mz_arraylist_remove_at(mz_ArrayList *list, int index) {
  bool result = false;
  //negative to positive conversion:
  index = _mz_arraylist_normalize_index(list->size, index);
  if (!_mz_arraylist_is_index_within_range(list->size, index)) {
    ERROR("index is out of range - %d", index);
  } else {
    for (int i = index; i < list->size - 1; i++) {
      list->array[i] = list->array[i + 1];
    }
    list->array[list->size - 1] = NULL;
    list->size -= 1;
    if (!_mz_arraylist_optimize_capacity(list, list->size)) {
      ERROR("could not optimize capacity");
    } else {
      result = true;
    }
  }
  return result;
}

bool mz_arraylist_remove_range(mz_ArrayList *list, int from_index, int to_index) {
  bool result = false;
  //negative to positive conversion:
  from_index = _mz_arraylist_normalize_index(list->size, from_index);
  to_index = _mz_arraylist_normalize_index(list->size, to_index);
  if (!_mz_arraylist_is_index_within_range(list->size, from_index)) {
    ERROR("'from' index is out of range - %d", from_index);
  } else if (!_mz_arraylist_is_index_within_range(list->size, to_index)) {
    ERROR("'to' index is out of range - %d", to_index);
  } else if (from_index > to_index) {
    ERROR("'from_index' is larger than the 'to_index' - %d > %d", from_index, to_index);
  } else {
    //remove 3,6 from 01234567890, must result in = 012|67890 where 345 is removed
    int range = to_index - from_index;
    for (int i = to_index; i < list->size; i++) {
      list->array[i - range] = list->array[i];
    }
    //clear end of the array
    for (int i = list->size - 1; i >= list->size - range; i--) {
      list->array[i] = NULL;
    }
    list->size -= range;
    //optimize capacity
    if (!_mz_arraylist_optimize_capacity(list, list->size)) {
      ERROR("could not optimize capacity");
    } else {
      result = true;
    }
  }
  return result;
}

bool mz_arraylist_set(mz_ArrayList *list, int index, void *element) {
  bool result = false;
  index = _mz_arraylist_normalize_index(list->size, index);
  if (!_mz_arraylist_is_index_within_range(list->size, index)) {
    ERROR("index is out of range - %d", index);
  } else {
    list->array[index] = element;
    result = true;
  }
  return result;
}

void *mz_arraylist_get(mz_ArrayList *list, int index) {
  index = _mz_arraylist_normalize_index(list->size, index);
  if (!_mz_arraylist_is_index_within_range(list->size, index)) {
    ERROR("index is out of range - %d", index);
    return NULL;
  } else {
    return list->array[index];
  }
}

void **mz_arraylist_get_range(mz_ArrayList *list, int from_index, int to_index) {
  void **result = NULL;
  from_index = _mz_arraylist_normalize_index(list->size, from_index);
  to_index = _mz_arraylist_normalize_index(list->size, to_index);
  if (!_mz_arraylist_is_index_within_range(list->size, from_index)) {
    ERROR("'from' index is out of range - %d", from_index);
  } else if (!_mz_arraylist_is_index_within_range(list->size, to_index)) {
    ERROR("'to' index is out of range - %d", to_index);
  } else if (from_index > to_index) {
    ERROR("'from_index' is larger than the 'to_index' - %d > %d", from_index, to_index);
  } else {
    int range = to_index - from_index;
    result = calloc(range, sizeof(void *));
    for (int i = 0; i < range; i++) {
      result[i] = list->array[i + from_index];
    }
  }
  return result;
}

mz_ArrayList *mz_arraylist_map(mz_ArrayList *list, void *(*mz_arraylist_fn)(const void *)) {
  mz_ArrayList *result = NULL;
  if (!list) {
    ERROR("list is null");
  } else {
    result = mz_arraylist_new(list->initial_capacity, sizeof(void *));
    for (int i = 0; i < list->size; i++) {
      void *r = (*mz_arraylist_fn)(list->array[i]);
      mz_arraylist_append(result, r);
      free(r);
    }
  }
  return result;
}

mz_ArrayList *mz_arraylist_filter(mz_ArrayList *list, bool (*mz_arraylist_filter_fn)(const void *)) {
  mz_ArrayList *result = NULL;
  if (!list) {
    ERROR("list is null");
  } else {
    result = mz_arraylist_new(list->initial_capacity, sizeof(void *));
    for (int i = 0; i < list->size; i++) {
      if ((*mz_arraylist_filter_fn)(list->array[i])) {
        mz_arraylist_append(result, list->array[i]);
      }
    }
  }
  return result;
}

size_t mz_arraylist_index_of(mz_ArrayList *list, bool (*mz_arraylist_filter_fn)(const void *)) {
  size_t result = -1;
  if (!list) {
    ERROR("list is null");
  } else {
    for (int i = 0; i < list->size; i++) {
      if ((*mz_arraylist_filter_fn)(list->array[i])) {
        result = i;
        break;
      }
    }
  }
  return result;
}


void *_mz_arraylist_reduce_recur(mz_ArrayList *list, void *accl, size_t idx,
                                 void *(*mz_arraylist_reduce_fn)(const void *, const void *)) {
  if (idx == list->size - 1) {
    return accl;
  } else {
    accl = (*mz_arraylist_reduce_fn)(accl, list->array[++idx]);
    return _mz_arraylist_reduce_recur(list, accl, idx, (*mz_arraylist_reduce_fn));
  }
}

void *mz_arraylist_reduce(mz_ArrayList *list, void *(*mz_arraylist_reduce_fn)(const void *, const void *)) {
  void *result = NULL;
  if (!list) {
    ERROR("list is null");
  } else if (list->size == 1) {
    result = list->array[0];
  } else {
    void *accl = list->array[0];
    result = _mz_arraylist_reduce_recur(list, accl, 0, (*mz_arraylist_reduce_fn));
  }
  return result;
}

size_t _mz_arraylist_binary_search_recur(mz_ArrayList *list, void *element,
                                         int (*mz_arraylist_comparator_fn)(const void *, const void *), size_t start,
                                         size_t end) {
  size_t len = end - start;
  size_t mid = (int) (len / 2) + start;
  void *current_element = list->array[mid];
  int comparison = (*mz_arraylist_comparator_fn)(&element, &current_element);
  if (comparison < 0) { //element < list[mid]
    return _mz_arraylist_binary_search_recur(list, element, (*mz_arraylist_comparator_fn), start, mid);
  } else if (comparison > 0) { // element > list[mid]
    return _mz_arraylist_binary_search_recur(list, element, (*mz_arraylist_comparator_fn), mid, end);
  } else {
    return mid;
  }
}

size_t mz_arraylist_binary_search(mz_ArrayList *list, void *element,
                                  int (*mz_arraylist_comparator_fn)(const void *, const void *)) {
  int result = -1;
  if (!list) {
    ERROR("list is null");
  } else {
    result = _mz_arraylist_binary_search_recur(list, element, (*mz_arraylist_comparator_fn), 0, list->size);
  }
  return result;
}

bool mz_arraylist_sort(mz_ArrayList *list, mz_ArrayListSortOption sort_option,
                       int (*mz_arraylist_comparator_fn)(const void *, const void *)) {
  bool result = false;
  if (!list) {
    ERROR("list is null");
  } else if (sort_option == mz_ArrayListSortOptionMerge &&
             mergesort(list->array, list->size, sizeof(void *), (*mz_arraylist_comparator_fn)) == -1) {
    ERROR("merge sort failed");
  } else if (sort_option == mz_ArrayListSortOptionHeap &&
             heapsort(list->array, list->size, sizeof(void *), (*mz_arraylist_comparator_fn)) == -1) {
    ERROR("heap sort failed");
  } else if (sort_option == mz_ArrayListSortOptionQuick) {
    qsort(list->array, list->size, sizeof(void *), (*mz_arraylist_comparator_fn));
    result = true;
  } else {
    result = true;
  }
  return result;
}
