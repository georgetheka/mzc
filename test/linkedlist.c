#include <stdio.h>
#include <string.h>
#include "../lib/minunit.h"
#include "../mz/linkedlist.h"
#include "../mz/logger.h"

int tests_run = 0;

static char *it_creates_a_list() {
  mz_LinkedList *list = mz_linkedlist_new();
  mu_assert("error - count != 0", list->count == 0);
  mu_assert("error - first != NULL", list->first == NULL);
  mu_assert("error - last != NULL", list->last == NULL);
  mz_linkedlist_free(list);
  return 0;
}

static char *it_pushes_item_into_empty_list() {
  mz_LinkedList *list = mz_linkedlist_new();
  char *value = "hello";
  mz_linkedlist_push(list, value);
  mu_assert("error - count != 1", list->count == 1);
  mu_assert("error - list->first != list->last", list->first == list->last);
  mu_assert("error - list->first->value != value", list->first->value == value);
  mu_assert("error - list->first->value != list->last->value", list->first->value == list->last->value);
  mz_linkedlist_free(list);
  return 0;
}

static char *it_pushes_item_into_non_empty_list() {
  mz_LinkedList *list = mz_linkedlist_new();
  char *one = "one";
  char *two = "two";
  char *three = "three";
  mz_linkedlist_push(list, one);
  mz_linkedlist_push(list, two);
  mz_linkedlist_push(list, three);
  mu_assert("error - count != 3", list->count == 3);
  mz_linkedlist_free(list);
  return 0;
}

static char *it_handles_removing_item_from_empty_list() {
  mz_LinkedList *list = mz_linkedlist_new();
  mz_LinkedListNode *node = calloc(1, sizeof(mz_LinkedListNode));
  node->value = "hello";
  void *result = mz_linkedlist_remove(list, node);
  mu_assert("error - result != null", result == NULL);
  mu_assert("error - list->first != null", list->first == NULL);
  mu_assert("error - list->last != null", list->last == NULL);
  mu_assert("error - list->count != 0", list->count == 0);
  free(node);
  mz_linkedlist_free(list);
  return 0;
}

static char *it_removes_item_from_single_item_list() {
  mz_LinkedList *list = mz_linkedlist_new();
  char *value = "hello\0";
  mz_linkedlist_push(list, value);
  mz_LinkedListNode *node = list->first;
  void *result = mz_linkedlist_remove(list, node);

  mu_assert("error - result != node->value", strcmp(value, result) == 0);
  mu_assert("error - list->first != null", list->first == NULL);
  mu_assert("error - list->last != null", list->last == NULL);
  mu_assert("error - list->count != 0", list->count == 0);
  mz_linkedlist_free(list);
  return 0;
}

static char *it_removes_first_item_of_multi_item_list() {
  mz_LinkedList *list = mz_linkedlist_new();
  char *one = "one";
  char *two = "two";
  char *three = "three";
  mz_linkedlist_push(list, one);
  mz_linkedlist_push(list, two);
  mz_linkedlist_push(list, three);
  mz_LinkedListNode *node = list->first;
  mz_LinkedListNode *second_item = list->first->next;
  void *result = mz_linkedlist_remove(list, node);
  mu_assert("error - result != node->value", strcmp(node->value, result) == 0);
  mu_assert("error - list->first != second_item", list->first == second_item);
  mu_assert("error - list->count != 2", list->count == 2);
  mz_linkedlist_free(list);
  return 0;
}

static char *it_removes_last_item_from_multi_item_list() {
  mz_LinkedList *list = mz_linkedlist_new();
  char *one = "one";
  char *two = "two";
  char *three = "three";
  mz_linkedlist_push(list, one);
  mz_linkedlist_push(list, two);
  mz_linkedlist_push(list, three);
  mz_LinkedListNode *node = list->last;
  mz_LinkedListNode *second_before_last = list->last->prev;
  void *result = mz_linkedlist_remove(list, node);
  mu_assert("error - result != node->value", result == node->value);
  mu_assert("error - list->last != second_before_last", list->last == second_before_last);
  mu_assert("error - list->count != 2", list->count == 2);
  mz_linkedlist_free(list);
  return 0;
}

static char *it_returns_null_when_removing_node_not_in_list() {
  mz_LinkedList *list = mz_linkedlist_new();
  char *one = "one";
  char *two = "two";
  char *three = "three";
  mz_linkedlist_push(list, one);
  mz_linkedlist_push(list, two);
  mz_linkedlist_push(list, three);
  mz_LinkedListNode *node = malloc(sizeof(mz_LinkedListNode));
  node->value = "not in list";
  void *result = mz_linkedlist_remove(list, node);
  mu_assert("error - result != null", result == NULL);
  free(node);
  mz_linkedlist_free(list);
  return 0;
}

static char *it_returns_null_when_popping_empty_list() {
  mz_LinkedList *list = mz_linkedlist_new();
  void *result = mz_linkedlist_pop(list);
  mu_assert("error - result != null", result == NULL);
  mz_linkedlist_free(list);
  return 0;
}

static char *it_removes_node_from_list() {
  mz_LinkedList *list = mz_linkedlist_new();
  char *one = "one";
  char *two = "two";
  char *three = "three";
  mz_linkedlist_push(list, one);
  mz_linkedlist_push(list, two);
  mz_linkedlist_push(list, three);
  mz_LinkedListNode *node_two = list->first->next;
  void *result = mz_linkedlist_remove(list, node_two);
  mu_assert("error - result != node_two->value", result == two);
  mu_assert("error - count != 2", list->count == 2);
  mu_assert("error - first->next != last", list->first->next == list->last);
  mu_assert("error - last->prev != first", list->last->prev == list->first);
  mz_linkedlist_free(list);
  return 0;
}

static char *it_pops_item_from_list() {
  mz_LinkedList *list = mz_linkedlist_new();
  char *one = "one";
  char *two = "two";
  char *three = "three";
  mz_linkedlist_push(list, one);
  mz_linkedlist_push(list, two);
  mz_linkedlist_push(list, three);
  void *result = mz_linkedlist_pop(list);
  mu_assert("error - result != last->value", result == three);
  mu_assert("error - count != 2", list->count == 2);
  mu_assert("error - first->next != last", list->first->next == list->last);
  mu_assert("error - last->prev != first", list->last->prev == list->first);
  mz_linkedlist_free(list);
  return 0;
}

static char *it_pops_first_item_from_list() {
  mz_LinkedList *list = mz_linkedlist_new();
  char *one = "one";
  char *two = "two";
  char *three = "three";
  mz_linkedlist_push(list, one);
  mz_linkedlist_push(list, two);
  mz_linkedlist_push(list, three);
  void *result = mz_linkedlist_shift(list);
  mu_assert("error - result != last->value", result == one);
  mu_assert("error - count != 2", list->count == 2);
  mu_assert("error - first->next != last", list->first->next == list->last);
  mu_assert("error - last->prev != first", list->last->prev == list->first);
  mz_linkedlist_free(list);
  return 0;
}

static char *it_returns_null_when_popping_first_item_from_empty_list() {
  mz_LinkedList *list = mz_linkedlist_new();
  void *result = mz_linkedlist_shift(list);
  mu_assert("error - result != null", result == NULL);
  mz_linkedlist_free(list);
  return 0;
}

static char *it_pushes_item_at_list_head() {
  mz_LinkedList *list = mz_linkedlist_new();
  char *one = "one";
  char *two = "two";
  char *three = "three";
  mz_linkedlist_push(list, one);
  mz_linkedlist_push(list, two);
  mz_linkedlist_push(list, three);
  char *value = "zero";
  mz_linkedlist_unshift(list, value);
  mu_assert("error - count != 4", list->count == 4);
  mu_assert("error - first->value != value", list->first->value == value);
  mz_linkedlist_free(list);
  return 0;
}

static char *mz_linkedlist_tests() {
  mu_run_test(it_creates_a_list);
  mu_run_test(it_pushes_item_into_empty_list);
  mu_run_test(it_pushes_item_into_non_empty_list);
  mu_run_test(it_handles_removing_item_from_empty_list);
  mu_run_test(it_removes_item_from_single_item_list);
  mu_run_test(it_removes_first_item_of_multi_item_list);
  mu_run_test(it_removes_last_item_from_multi_item_list);
  mu_run_test(it_returns_null_when_removing_node_not_in_list);
  mu_run_test(it_removes_node_from_list);
  mu_run_test(it_returns_null_when_popping_empty_list);
  mu_run_test(it_pops_item_from_list);
  mu_run_test(it_pops_first_item_from_list);
  mu_run_test(it_returns_null_when_popping_first_item_from_empty_list);
  mu_run_test(it_pushes_item_at_list_head);
  return 0;
}
