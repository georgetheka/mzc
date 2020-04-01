#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include "../lib/minunit.h"
#include "../mz/arraylist.h"
#include "../mz/logger.h"

//int tests_run = 0;

static char *it_creates_and_initializes_an_arraylist() {
  const long BYTE_SIZE = sizeof(char);
  const size_t INITIAL_CAPACITY = 10;
  mz_ArrayList *list = mz_arraylist_new(INITIAL_CAPACITY, BYTE_SIZE);
  mu_assert("error - capacity != INITIAL_CAPACITY", list->capacity == INITIAL_CAPACITY);
  mu_assert("error - size != 0", list->size == 0);
  mu_assert("error - element_size != element_size", list->element_size == BYTE_SIZE);
  mz_arraylist_free(list);
  return 0;
}

static char *it_appends_item_to_arraylist() {
  const long ELEMENT_SIZE = sizeof(void *);
  const size_t INITIAL_CAPACITY = 2;
  mz_ArrayList *list = mz_arraylist_new(INITIAL_CAPACITY, ELEMENT_SIZE);
  char *first = "first";
  char *second = "second";
  bool result1 = mz_arraylist_append(list, first);
  bool result2 = mz_arraylist_append(list, second);
  mu_assert("error - result1 != true", result1 == true);
  mu_assert("error - result2 != true", result2 == true);
  mu_assert("error - capacity != INITIAL_CAPACITY", list->capacity == INITIAL_CAPACITY);
  mu_assert("error - size != 2", list->size == 2);
  mu_assert("error - element_size != element_size", list->element_size == ELEMENT_SIZE);
  mz_arraylist_free(list);
  return 0;
}

static char *it_doubles_capacity_when_appending_items_over_initial_capacity() {
  const long ELEMENT_SIZE = sizeof(void *);
  const size_t INITIAL_CAPACITY = 2;
  mz_ArrayList *list = mz_arraylist_new(INITIAL_CAPACITY, ELEMENT_SIZE);
  char *first = "first";
  char *second = "second";
  char *third = "third";
  bool result1 = mz_arraylist_append(list, first);
  bool result2 = mz_arraylist_append(list, second);
  bool result3 = mz_arraylist_append(list, third);
  mu_assert("error - result1 != true", result1 == true);
  mu_assert("error - result2 != true", result2 == true);
  mu_assert("error - result3 != true", result3 == true);
  mu_assert("error - capacity != 4", list->capacity == 2 * INITIAL_CAPACITY);
  mu_assert("error - size != 3", list->size == 3);
  mu_assert("error - element_size != element_size", list->element_size == ELEMENT_SIZE);
  mz_arraylist_free(list);
  return 0;
}

static char *it_appends_range_of_items_to_list() {
  const long ELEMENT_SIZE = sizeof(void *);
  const size_t INITIAL_CAPACITY = 2;
  mz_ArrayList *list = mz_arraylist_new(INITIAL_CAPACITY, ELEMENT_SIZE);
  char *first = "first";
  char *array_to_append[] = {"second", "third"};
  bool result1 = mz_arraylist_append(list, first);
  bool result2 = mz_arraylist_append_range(list, (void **) array_to_append, 2);
  mu_assert("error - result1 != true", result1 == true);
  mu_assert("error - result2 != true", result2 == true);
  mu_assert("error - capacity != 4", list->capacity == 2 * INITIAL_CAPACITY);
  mu_assert("error - size != 3", list->size == 3);
  mu_assert("error - element_size != element_size", list->element_size == ELEMENT_SIZE);
  mz_arraylist_free(list);
  return 0;
}

static char *it_inserts_item_at_index() {
  const long ELEMENT_SIZE = sizeof(void *);
  const size_t INITIAL_CAPACITY = 2;
  mz_ArrayList *list = mz_arraylist_new(INITIAL_CAPACITY, ELEMENT_SIZE);
  char *first = "first";
  char *second = "second";
  char *third = "third";
  mz_arraylist_append(list, first);
  mz_arraylist_append(list, second);
  mz_arraylist_append(list, third);
  char *item_to_insert = "after first";
  mz_arraylist_insert_at(list, 1, item_to_insert);
  mu_assert("error - capacity != 4", list->capacity == 2 * INITIAL_CAPACITY);
  mu_assert("error - size != 4", list->size == 4);
  mu_assert("error - element_size != element_size", list->element_size == ELEMENT_SIZE);
  mu_assert("error - element at index 0 != expected", list->array[0] == first);
  mu_assert("error - element at index 1 != expected", list->array[1] == item_to_insert);
  mu_assert("error - element at index 2 != expected", list->array[2] == second);
  mu_assert("error - element at index 3 != expected", list->array[3] == third);
  mz_arraylist_free(list);
  return 0;
}

static char *it_inserts_item_at_index_zero_of_empty_array() {
  const long ELEMENT_SIZE = sizeof(void *);
  const size_t INITIAL_CAPACITY = 2;
  mz_ArrayList *list = mz_arraylist_new(INITIAL_CAPACITY, ELEMENT_SIZE);
  char *first = "first";
  mz_arraylist_insert_at(list, 0, first);
  mu_assert("error - capacity != 2", list->capacity == 2);
  mu_assert("error - size != 1", list->size == 1);
  mu_assert("error - element_size != element_size", list->element_size == ELEMENT_SIZE);
  mu_assert("error - element at index 0 != expected", list->array[0] == first);
  mz_arraylist_free(list);
  return 0;
}

static char *it_inserts_item_at_index_minus_one_of_empty_array() {
  const long ELEMENT_SIZE = sizeof(void *);
  const size_t INITIAL_CAPACITY = 2;
  mz_ArrayList *list = mz_arraylist_new(INITIAL_CAPACITY, ELEMENT_SIZE);
  char *first = "first";
  mz_arraylist_insert_at(list, -1, first);
  mu_assert("error - capacity != 2", list->capacity == 2);
  mu_assert("error - size != 1", list->size == 1);
  mu_assert("error - element_size != element_size", list->element_size == ELEMENT_SIZE);
  mu_assert("error - element at index 0 != expected", list->array[0] == first);
  mz_arraylist_free(list);
  return 0;
}

static char *it_inserts_item_with_negative_index_for_non_empty_array() {
  const long ELEMENT_SIZE = sizeof(void *);
  const size_t INITIAL_CAPACITY = 2;
  mz_ArrayList *list = mz_arraylist_new(INITIAL_CAPACITY, ELEMENT_SIZE);
  char *first = "first";
  char *second = "second";
  char *third = "third";
  mz_arraylist_append(list, first);
  mz_arraylist_append(list, second);
  mz_arraylist_append(list, third);
  char *item_to_insert = "item to insert";
  mz_arraylist_insert_at(list, -2, item_to_insert);
  mu_assert("error - capacity != 4", list->capacity == 2 * INITIAL_CAPACITY);
  mu_assert("error - size != 4", list->size == 4);
  mu_assert("error - element_size != element_size", list->element_size == ELEMENT_SIZE);
  mu_assert("error - element at index 0 != expected", list->array[0] == first);
  mu_assert("error - element at index 1 != expected", list->array[1] == item_to_insert);
  mu_assert("error - element at index 2 != expected", list->array[2] == second);
  mu_assert("error - element at index 3 != expected", list->array[3] == third);
  mz_arraylist_free(list);
  return 0;
}

static char *it_fails_to_insert_at_index_out_of_range() {
  const long ELEMENT_SIZE = sizeof(void *);
  const size_t INITIAL_CAPACITY = 2;
  mz_ArrayList *list = mz_arraylist_new(INITIAL_CAPACITY, ELEMENT_SIZE);
  char *first = "first";
  char *second = "second";
  mz_arraylist_append(list, first);
  mz_arraylist_append(list, second);
  char *item_to_insert = "item to insert";
  bool result = mz_arraylist_insert_at(list, 2, item_to_insert);
  mu_assert("error - result != false", result == false);
  mu_assert("error - capacity != 2", list->capacity == INITIAL_CAPACITY);
  mu_assert("error - size != 2", list->size == 2);
  mu_assert("error - element_size != element_size", list->element_size == ELEMENT_SIZE);
  mu_assert("error - element at index 0 != expected", list->array[0] == first);
  mu_assert("error - element at index 1 != expected", list->array[1] == second);
  mz_arraylist_free(list);
  return 0;
}

static char *it_fails_to_insert_at_negative_index_out_of_range() {
  const long ELEMENT_SIZE = sizeof(void *);
  const size_t INITIAL_CAPACITY = 2;
  mz_ArrayList *list = mz_arraylist_new(INITIAL_CAPACITY, ELEMENT_SIZE);
  char *first = "first";
  char *second = "second";
  mz_arraylist_append(list, first);
  mz_arraylist_append(list, second);
  char *item_to_insert = "item to insert";
  bool result = mz_arraylist_insert_at(list, -3, item_to_insert);
  mu_assert("error - result != false", result == false);
  mu_assert("error - capacity != 2", list->capacity == INITIAL_CAPACITY);
  mu_assert("error - size != 2", list->size == 2);
  mu_assert("error - element_size != element_size", list->element_size == ELEMENT_SIZE);
  mu_assert("error - element at index 0 != expected", list->array[0] == first);
  mu_assert("error - element at index 1 != expected", list->array[1] == second);

  mz_arraylist_free(list);
  return 0;
}

static char *it_removes_item_at_index_in_range_of_non_empty_list() {
  const long ELEMENT_SIZE = sizeof(void *);
  const size_t INITIAL_CAPACITY = 2;
  mz_ArrayList *list = mz_arraylist_new(INITIAL_CAPACITY, ELEMENT_SIZE);
  char *first = "first";
  char *second = "second";
  char *third = "third";
  mz_arraylist_append(list, first);
  mz_arraylist_append(list, second);
  mz_arraylist_append(list, third);
  bool result = mz_arraylist_remove_at(list, 1);
  mu_assert("error - result != true", result == true);
  mu_assert("error - capacity != 4", list->capacity == 2 * INITIAL_CAPACITY);
  mu_assert("error - size != 2", list->size == 2);
  mu_assert("error - element_size != element_size", list->element_size == ELEMENT_SIZE);
  mu_assert("error - element at index 0 != expected", list->array[0] == first);
  mu_assert("error - element at index 1 != expected", list->array[1] == third);
  mu_assert("error - element at index 2 != NULL", list->array[2] == NULL);

  mz_arraylist_free(list);
  return 0;
}

static char *it_removes_first_item_of_non_empty_list() {
  const long ELEMENT_SIZE = sizeof(void *);
  const size_t INITIAL_CAPACITY = 2;
  mz_ArrayList *list = mz_arraylist_new(INITIAL_CAPACITY, ELEMENT_SIZE);
  char *first = "first";
  char *second = "second";
  char *third = "third";
  mz_arraylist_append(list, first);
  mz_arraylist_append(list, second);
  mz_arraylist_append(list, third);
  bool result = mz_arraylist_remove_at(list, 0);
  mu_assert("error - result != true", result == true);
  mu_assert("error - capacity != 4", list->capacity == 2 * INITIAL_CAPACITY);
  mu_assert("error - size != 2", list->size == 2);
  mu_assert("error - element_size != element_size", list->element_size == ELEMENT_SIZE);
  mu_assert("error - element at index 0 != expected", list->array[0] == second);
  mu_assert("error - element at index 1 != expected", list->array[1] == third);
  mu_assert("error - element at index 2 != NULL", list->array[2] == NULL);

  mz_arraylist_free(list);
  return 0;
}

static char *it_removes_last_item_with_negative_index_of_non_empty_list() {
  const long ELEMENT_SIZE = sizeof(void *);
  const size_t INITIAL_CAPACITY = 2;
  mz_ArrayList *list = mz_arraylist_new(INITIAL_CAPACITY, ELEMENT_SIZE);
  char *first = "first";
  char *second = "second";
  char *third = "third";
  mz_arraylist_append(list, first);
  mz_arraylist_append(list, second);
  mz_arraylist_append(list, third);
  bool result = mz_arraylist_remove_at(list, -1);
  mu_assert("error - result != true", result == true);
  mu_assert("error - capacity != 4", list->capacity == 2 * INITIAL_CAPACITY);
  mu_assert("error - size != 2", list->size == 2);
  mu_assert("error - element_size != element_size", list->element_size == ELEMENT_SIZE);
  mu_assert("error - element at index 0 != expected", list->array[0] == first);
  mu_assert("error - element at index 1 != expected", list->array[1] == second);
  mu_assert("error - element at index 2 != NULL", list->array[2] == NULL);

  mz_arraylist_free(list);
  return 0;
}

static char *it_fails_to_remove_item_at_index_out_of_range_for_non_empty_list() {
  const long ELEMENT_SIZE = sizeof(void *);
  const size_t INITIAL_CAPACITY = 2;
  mz_ArrayList *list = mz_arraylist_new(INITIAL_CAPACITY, ELEMENT_SIZE);
  char *first = "first";
  char *second = "second";
  char *third = "third";
  mz_arraylist_append(list, first);
  mz_arraylist_append(list, second);
  mz_arraylist_append(list, third);
  bool result = mz_arraylist_remove_at(list, 3);
  mu_assert("error - result != false", result == false);
  mu_assert("error - capacity != 4", list->capacity == 2 * INITIAL_CAPACITY);
  mu_assert("error - size != 3", list->size == 3);
  mu_assert("error - element_size != element_size", list->element_size == ELEMENT_SIZE);
  mu_assert("error - element at index 0 != expected", list->array[0] == first);
  mu_assert("error - element at index 1 != expected", list->array[1] == second);
  mu_assert("error - element at index 2 != NULL", list->array[2] == third);
  mz_arraylist_free(list);
  return 0;
}

static char *it_fails_to_remove_item_at_negative_index_out_of_range_for_non_empty_list() {
  const long ELEMENT_SIZE = sizeof(void *);
  const size_t INITIAL_CAPACITY = 2;
  mz_ArrayList *list = mz_arraylist_new(INITIAL_CAPACITY, ELEMENT_SIZE);
  char *first = "first";
  char *second = "second";
  char *third = "third";
  mz_arraylist_append(list, first);
  mz_arraylist_append(list, second);
  mz_arraylist_append(list, third);
  bool result = mz_arraylist_remove_at(list, -4);
  mu_assert("error - result != false", result == false);
  mu_assert("error - capacity != 4", list->capacity == 2 * INITIAL_CAPACITY);
  mu_assert("error - size != 3", list->size == 3);
  mu_assert("error - element_size != element_size", list->element_size == ELEMENT_SIZE);
  mu_assert("error - element at index 0 != expected", list->array[0] == first);
  mu_assert("error - element at index 1 != expected", list->array[1] == second);
  mu_assert("error - element at index 2 != NULL", list->array[2] == third);
  mz_arraylist_free(list);
  return 0;
}

static char *it_returns_expected_result_when_trying_to_remove_from_empty_list() {
  const long ELEMENT_SIZE = sizeof(void *);
  const size_t INITIAL_CAPACITY = 2;
  mz_ArrayList *list = mz_arraylist_new(INITIAL_CAPACITY, ELEMENT_SIZE);
  bool result = mz_arraylist_remove_at(list, 1);
  mu_assert("error - result != false", result == false);
  mz_arraylist_free(list);
  return 0;
}

static char *it_removes_a_subarray_with_range_in_array() {
  const long ELEMENT_SIZE = sizeof(void *);
  const size_t INITIAL_CAPACITY = 10;
  mz_ArrayList *list = mz_arraylist_new(INITIAL_CAPACITY, ELEMENT_SIZE);
  for (int i = 0; i < 10; i++) {
    mz_arraylist_append(list, (void *) (intptr_t) i);
  }
  bool result = mz_arraylist_remove_range(list, 2, 6);
  char *actual = calloc(11, sizeof(char));
  char *expected = "0167890000\0";
  for (int i = 0; i < 10; i++) {
    actual[i] = (char) ((int) (intptr_t) list->array[i] + '0');
  }
  actual[10] = '\0';
  mu_assert("error - initial != expected", strcmp(actual, expected) == 0);
  mu_assert("error - result != true", result == true);
  mu_assert("error - capacity != 10", list->capacity == INITIAL_CAPACITY);
  mu_assert("error - size != 6", list->size == 6);
  free(actual);
  mz_arraylist_free(list);
  return 0;
}

static char *it_removes_a_subarray_with_negative_numbered_range_in_array() {
  const long ELEMENT_SIZE = sizeof(void *);
  const size_t INITIAL_CAPACITY = 10;
  mz_ArrayList *list = mz_arraylist_new(INITIAL_CAPACITY, ELEMENT_SIZE);
  for (int i = 0; i < 10; i++) {
    mz_arraylist_append(list, (void *) (intptr_t) i);
  }
  bool result = mz_arraylist_remove_range(list, -6, -2);
  char *actual = calloc(11, sizeof(char));
  char *expected = "0123890000\0";
  for (int i = 0; i < 10; i++) {
    actual[i] = (char) ((int) (intptr_t) list->array[i] + '0');
  }
  actual[10] = '\0';
  mu_assert("error - initial != expected", strcmp(actual, expected) == 0);
  mu_assert("error - result != true", result == true);
  mu_assert("error - capacity != 10", list->capacity == INITIAL_CAPACITY);
  mu_assert("error - size != 6", list->size == 6);
  free(actual);
  mz_arraylist_free(list);
  return 0;
}

static char *it_set_an_item_at_index() {
  const long ELEMENT_SIZE = sizeof(void *);
  const size_t INITIAL_CAPACITY = 2;
  mz_ArrayList *list = mz_arraylist_new(INITIAL_CAPACITY, ELEMENT_SIZE);
  char *first = "first";
  char *second = "second";
  char *item_to_set = "set item";
  mz_arraylist_append(list, first);
  mz_arraylist_append(list, second);
  bool result = mz_arraylist_set(list, 1, item_to_set);
  mu_assert("error - result != true", result == true);
  mu_assert("error - capacity != 2", list->capacity == INITIAL_CAPACITY);
  mu_assert("error - size != 2", list->size == 2);
  mu_assert("error - element_size != element_size", list->element_size == ELEMENT_SIZE);
  mu_assert("error - element at index 0 != expected", list->array[0] == first);
  mu_assert("error - element at index 1 != set item", list->array[1] == item_to_set);
  mz_arraylist_free(list);
  return 0;
}

static char *it_set_an_item_with_negative_index() {
  const long ELEMENT_SIZE = sizeof(void *);
  const size_t INITIAL_CAPACITY = 2;
  mz_ArrayList *list = mz_arraylist_new(INITIAL_CAPACITY, ELEMENT_SIZE);
  char *first = "first";
  char *second = "second";
  char *item_to_set = "set item";
  mz_arraylist_append(list, first);
  mz_arraylist_append(list, second);
  bool result = mz_arraylist_set(list, -1, item_to_set);
  mu_assert("error - result != true", result == true);
  mu_assert("error - capacity != 2", list->capacity == INITIAL_CAPACITY);
  mu_assert("error - size != 2", list->size == 2);
  mu_assert("error - element_size != element_size", list->element_size == ELEMENT_SIZE);
  mu_assert("error - element at index 0 != expected", list->array[0] == first);
  mu_assert("error - element at index 1 != set item", list->array[1] == item_to_set);
  mz_arraylist_free(list);
  return 0;
}

static char *it_gets_an_item_with_negative_index() {
  const long ELEMENT_SIZE = sizeof(void *);
  const size_t INITIAL_CAPACITY = 2;
  mz_ArrayList *list = mz_arraylist_new(INITIAL_CAPACITY, ELEMENT_SIZE);
  char *first = "first";
  char *second = "second";
  char *expected = second;
  mz_arraylist_append(list, first);
  mz_arraylist_append(list, second);
  char *actual = mz_arraylist_get(list, -1);
  mu_assert("error - actual != expected", actual == expected);
  mu_assert("error - capacity != 2", list->capacity == INITIAL_CAPACITY);
  mu_assert("error - size != 2", list->size == 2);
  mu_assert("error - element_size != element_size", list->element_size == ELEMENT_SIZE);
  mu_assert("error - element at index 0 != expected", list->array[0] == first);
  mu_assert("error - element at index 1 != expected", list->array[1] == second);
  mz_arraylist_free(list);
  return 0;
}

static char *it_gets_range_from_array() {
  const long ELEMENT_SIZE = sizeof(void *);
  const size_t INITIAL_CAPACITY = 2;
  mz_ArrayList *list = mz_arraylist_new(INITIAL_CAPACITY, ELEMENT_SIZE);
  char *first = "first";
  char *second = "second";
  char *third = "third";
  char *fourth = "fourth";
  mz_arraylist_append(list, first);
  mz_arraylist_append(list, second);
  mz_arraylist_append(list, third);
  mz_arraylist_append(list, fourth);
  void **result = mz_arraylist_get_range(list, 1, 3);
  mu_assert("error - range element at index 0 != expected", result[0] == second);
  mu_assert("error - reange element at index 1 != expected", result[1] == third);
  free(result);
  mz_arraylist_free(list);
  return 0;
}

static char *it_gets_list_properties() {
  const long ELEMENT_SIZE = sizeof(void *);
  const size_t INITIAL_CAPACITY = 2;
  mz_ArrayList *list = mz_arraylist_new(INITIAL_CAPACITY, ELEMENT_SIZE);
  char *first = "first";
  char *second = "second";
  mz_arraylist_append(list, first);
  mz_arraylist_append(list, second);

  mu_assert("error - size != expected", mz_arraylist_size(list) == 2);
  mu_assert("error - capacity != expected", mz_arraylist_capacity(list) == 2);
  mu_assert("error - is_empty != false", mz_arraylist_is_empty(list) == false);
  mz_arraylist_free(list);
  return 0;
}

static char *it_inserts_first_item() {
  const long ELEMENT_SIZE = sizeof(void *);
  const size_t INITIAL_CAPACITY = 2;
  mz_ArrayList *list = mz_arraylist_new(INITIAL_CAPACITY, ELEMENT_SIZE);
  char *first = "first";
  char *second = "second";
  char *item_to_insert = "third";
  mz_arraylist_append(list, first);
  mz_arraylist_append(list, second);
  bool result = mz_arraylist_insert_first(list, item_to_insert);

  mu_assert("error - result != true", result == true);
  mu_assert("error - size != expected", list->size == 3);
  mu_assert("error - capacity != expected", list->capacity == 4);
  mu_assert("error - element at index 0 != expected", list->array[0] == item_to_insert);
  mu_assert("error - element at index 1 != expected", list->array[1] == first);
  mu_assert("error - element at index 2 != expected", list->array[2] == second);
  mz_arraylist_free(list);
  return 0;
}

static char *it_removes_last_item() {
  const long ELEMENT_SIZE = sizeof(void *);
  const size_t INITIAL_CAPACITY = 2;
  mz_ArrayList *list = mz_arraylist_new(INITIAL_CAPACITY, ELEMENT_SIZE);
  char *first = "first";
  char *second = "second";
  char *third = "third";
  mz_arraylist_append(list, first);
  mz_arraylist_append(list, second);
  mz_arraylist_append(list, third);
  bool result = mz_arraylist_remove_last(list);
  mu_assert("error - result != true", result == true);
  mu_assert("error - size != expected", list->size == 2);
  mu_assert("error - capacity != expected", list->capacity == 4);
  mu_assert("error - element at index 0 != expected", list->array[0] == first);
  mu_assert("error - element at index 1 != expected", list->array[1] == second);
  mz_arraylist_free(list);
  return 0;
}

static char *it_enumerates_through_list_using_macro_definition() {
  const long ELEMENT_SIZE = sizeof(void *);
  const size_t INITIAL_CAPACITY = 2;
  mz_ArrayList *list = mz_arraylist_new(INITIAL_CAPACITY, ELEMENT_SIZE);
  char *first = "first";
  char *second = "second";
  char *third = "third";
  mz_arraylist_append(list, first);
  mz_arraylist_append(list, second);
  mz_arraylist_append(list, third);
  mzm_arraylist_foreach(list, element, index) {
    mu_assert("error - array[%d] != macro(%d)", element == list->array[index]);
  }
  mz_arraylist_free(list);
  return 0;
}

void *map_fn(const void *element) {
  size_t size = 3;
  char *result = (char *) calloc(size, sizeof(void *));
  return memcpy(result, element, 3);
}

static char *it_maps_arraylist_to_fn() {
  const long ELEMENT_SIZE = sizeof(void *);
  const size_t INITIAL_CAPACITY = 2;
  mz_ArrayList *list = mz_arraylist_new(INITIAL_CAPACITY, ELEMENT_SIZE);
  char *first = "first";
  char *second = "second";
  char *third = "third";
  mz_arraylist_append(list, first);
  mz_arraylist_append(list, second);
  mz_arraylist_append(list, third);
  mz_ArrayList *result = mz_arraylist_map(list, *map_fn);
  mzm_arraylist_foreach(result, element, index) {
    mu_assert("error - result element length != 3", strlen(element) == 3);
  }
  mz_arraylist_free(result);
  mz_arraylist_free(list);
  return 0;
}

bool filter_fn(const void *element) {
  return strlen(element) <= 3;
}

static char *it_filters_arraylist_based_on_filter_fn() {
  const long ELEMENT_SIZE = sizeof(void *);
  const size_t INITIAL_CAPACITY = 2;
  mz_ArrayList *list = mz_arraylist_new(INITIAL_CAPACITY, ELEMENT_SIZE);
  char *first = "one";
  char *second = "two";
  char *third = "three";
  mz_arraylist_append(list, first);
  mz_arraylist_append(list, second);
  mz_arraylist_append(list, third);
  mz_ArrayList *result = mz_arraylist_filter(list, *filter_fn);
  mu_assert("error - result size != 2", result->size == 2);
  mzm_arraylist_foreach(result, element, index) {
    mu_assert("error - result element length != 3", strlen(element) == 3);
  }
  mz_arraylist_free(result);
  mz_arraylist_free(list);
  return 0;
}

bool index_of_fn(const void *element) {
  return strcmp(element, "two") == 0;
}

static char *it_finds_the_first_index_of_element() {
  const long ELEMENT_SIZE = sizeof(void *);
  const size_t INITIAL_CAPACITY = 2;
  mz_ArrayList *list = mz_arraylist_new(INITIAL_CAPACITY, ELEMENT_SIZE);
  char *first = "one";
  char *second = "two";
  char *third = "three";
  mz_arraylist_append(list, first);
  mz_arraylist_append(list, second);
  mz_arraylist_append(list, third);
  size_t result = mz_arraylist_index_of(list, *index_of_fn);
  mu_assert("error - result index != 1", result == 1);
  mz_arraylist_free(list);
  return 0;
}

void *arraylist_reduce_fn(const void *a, const void *b) {
  return (void *) ((long) a + (long) b);
};

static char *it_reduces_list() {
  const long ELEMENT_SIZE = sizeof(void *);
  const size_t INITIAL_CAPACITY = 2;
  mz_ArrayList *list = mz_arraylist_new(INITIAL_CAPACITY, ELEMENT_SIZE);
  void *first = (void *) (long) 1;
  void *second = (void *) (long) 3;
  void *third = (void *) (long) 7;
  mz_arraylist_append(list, first);
  mz_arraylist_append(list, second);
  mz_arraylist_append(list, third);
  int result = (int) (long) mz_arraylist_reduce(list, *arraylist_reduce_fn);
  mu_assert("error - result accumulator != 11", result == 11);
  mz_arraylist_free(list);
  return 0;
}

static char *it_returns_null_when_reducing_a_null_list() {
  void *result = mz_arraylist_reduce(NULL, *arraylist_reduce_fn);
  mu_assert("error - result accumulator != NULL", result == NULL);
  return 0;
}

static char *it_returns_value_when_reducing_a_single_item_list() {
  const long ELEMENT_SIZE = sizeof(void *);
  const size_t INITIAL_CAPACITY = 2;
  mz_ArrayList *list = mz_arraylist_new(INITIAL_CAPACITY, ELEMENT_SIZE);
  void *first = (void *) (long) 1;
  mz_arraylist_append(list, first);
  int result = (int) (long) mz_arraylist_reduce(list, *arraylist_reduce_fn);
  mu_assert("error - result accumulator != 1", result == 1);
  mz_arraylist_free(list);
  return 0;
}

int arraylist_comparator_fn(const void *first, const void *second) {
  int a = *((int *) first);
  int b = *((int *) second);
  return a - b;
};

static char *it_finds_item_in_sorted_array_using_binary_search() {
  const long ELEMENT_SIZE = sizeof(void *);
  const size_t INITIAL_CAPACITY = 100;
  mz_ArrayList *list = mz_arraylist_new(INITIAL_CAPACITY, ELEMENT_SIZE);
  for (int i = 0; i < 1000; i++) {
    void *item = (void *) (long) (i + 1);
    mz_arraylist_append(list, item);
  }
  void *item_to_find = (void *) (long) 857;
  size_t result = mz_arraylist_binary_search(list, item_to_find, arraylist_comparator_fn);
  mu_assert("error - index of 857 != 856", result == 856);
  mu_assert("error - value of index 856 != 857", (int) (long) list->array[result] == 857);
  mz_arraylist_free(list);
  return 0;
}

static char *it_sorts_using_mergesort() {
  const long ELEMENT_SIZE = sizeof(void *);
  const size_t INITIAL_CAPACITY = 100;
  mz_ArrayList *list = mz_arraylist_new(INITIAL_CAPACITY, ELEMENT_SIZE);
  for (int i = 0; i < 100; i++) {
    void *item = (void *) (long) (int) (rand() % 100);
    mz_arraylist_append(list, item);
  }
  bool result = mz_arraylist_sort(list, mz_ArrayListSortOptionMerge, arraylist_comparator_fn);
  mu_assert("error - mergesort result != true", result == true);
  int prev = -1;
  for (int i = 0; i < 100; i++) {
    int current = (int) (long) mz_arraylist_get(list, i);
    if (current >= prev) {
      prev = current;
    } else {
      mu_assert("error - mergesort item not sorted", current >= prev);
    }
  }
  mz_arraylist_free(list);
  return 0;
}

static char *it_sorts_using_heapsort() {
  const long ELEMENT_SIZE = sizeof(void *);
  const size_t INITIAL_CAPACITY = 100;
  mz_ArrayList *list = mz_arraylist_new(INITIAL_CAPACITY, ELEMENT_SIZE);
  for (int i = 0; i < 100; i++) {
    void *item = (void *) (long) (int) (rand() % 100);
    mz_arraylist_append(list, item);
  }
  bool result = mz_arraylist_sort(list, mz_ArrayListSortOptionHeap, arraylist_comparator_fn);
  mu_assert("error - heapsort result != true", result == true);
  int prev = -1;
  for (int i = 0; i < 100; i++) {
    int current = (int) (long) mz_arraylist_get(list, i);
    if (current >= prev) {
      prev = current;
    } else {
      mu_assert("error - heapsort item not sorted", current >= prev);
    }
  }
  mz_arraylist_free(list);
  return 0;
}

static char *it_sorts_using_quicksort() {
  const long ELEMENT_SIZE = sizeof(void *);
  const size_t INITIAL_CAPACITY = 100;
  mz_ArrayList *list = mz_arraylist_new(INITIAL_CAPACITY, ELEMENT_SIZE);
  for (int i = 0; i < 100; i++) {
    void *item = (void *) (long) (int) (rand() % 100);
    mz_arraylist_append(list, item);
  }
  bool result = mz_arraylist_sort(list, mz_ArrayListSortOptionQuick, arraylist_comparator_fn);
  mu_assert("error - quicksort result != true", result == true);
  int prev = -1;
  for (int i = 0; i < 100; i++) {
    int current = (int) (long) mz_arraylist_get(list, i);
    if (current >= prev) {
      prev = current;
    } else {
      mu_assert("error - quicksort item not sorted", current >= prev);
    }
  }
  mz_arraylist_free(list);
  return 0;
}

static char *mz_arraylist_tests() {
  mu_run_test(it_creates_and_initializes_an_arraylist);
  mu_run_test(it_appends_item_to_arraylist);
  mu_run_test(it_doubles_capacity_when_appending_items_over_initial_capacity);
  mu_run_test(it_appends_range_of_items_to_list);
  mu_run_test(it_inserts_item_at_index);
  mu_run_test(it_inserts_item_at_index_zero_of_empty_array);
  mu_run_test(it_inserts_item_at_index_minus_one_of_empty_array);
  mu_run_test(it_inserts_item_with_negative_index_for_non_empty_array);
  mu_run_test(it_fails_to_insert_at_index_out_of_range);
  mu_run_test(it_fails_to_insert_at_negative_index_out_of_range);
  mu_run_test(it_removes_item_at_index_in_range_of_non_empty_list);
  mu_run_test(it_removes_first_item_of_non_empty_list);
  mu_run_test(it_removes_last_item_with_negative_index_of_non_empty_list);
  mu_run_test(it_fails_to_remove_item_at_index_out_of_range_for_non_empty_list);
  mu_run_test(it_fails_to_remove_item_at_negative_index_out_of_range_for_non_empty_list);
  mu_run_test(it_returns_expected_result_when_trying_to_remove_from_empty_list);
  mu_run_test(it_removes_a_subarray_with_range_in_array);
  mu_run_test(it_removes_a_subarray_with_negative_numbered_range_in_array);
  mu_run_test(it_set_an_item_at_index);
  mu_run_test(it_set_an_item_with_negative_index);
  mu_run_test(it_gets_an_item_with_negative_index);
  mu_run_test(it_gets_range_from_array);
  mu_run_test(it_gets_list_properties);
  mu_run_test(it_inserts_first_item);
  mu_run_test(it_removes_last_item);
  mu_run_test(it_enumerates_through_list_using_macro_definition);
  mu_run_test(it_maps_arraylist_to_fn);
  mu_run_test(it_filters_arraylist_based_on_filter_fn);
  mu_run_test(it_finds_the_first_index_of_element);
  mu_run_test(it_reduces_list);
  mu_run_test(it_returns_null_when_reducing_a_null_list);
  mu_run_test(it_returns_value_when_reducing_a_single_item_list);
  mu_run_test(it_finds_item_in_sorted_array_using_binary_search);
  mu_run_test(it_sorts_using_mergesort);
  mu_run_test(it_sorts_using_quicksort);
  mu_run_test(it_sorts_using_heapsort);
  return 0;
}
