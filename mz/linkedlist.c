#include <string.h>
#include "../mz/linkedlist.h"
#include "../mz/logger.h"

mz_LinkedList *mz_linkedlist_new() {
  mz_LinkedList *list = calloc(1, sizeof(mz_LinkedList));
  if (!list) {
    ERROR("could not allocate memory for list");
    return NULL;
  } else {
    return list;
  }
}

void mz_linkedlist_free(mz_LinkedList *list) {
  mz_LinkedListNode *current = NULL;
  for (current = list->first; current != NULL; current = current->next) {
    if (current->prev) {
      free(current->prev);
    }
  }
  free(list->last);
  free(list);
}

void mz_linkedlist_push(mz_LinkedList *list, void *value) {
  if (!list) {
    ERROR("list is null");
  } else {
    mz_LinkedListNode *node = calloc(1, sizeof(mz_LinkedListNode));
    if (!node) {
      ERROR("could not alloc memory for node");
    } else {
      node->value = value;
      if (list->count == 0) {
        list->first = node;
        list->last = node;
      } else {
        list->last->next = node;
        node->prev = list->last;
        list->last = node;
      }
      list->count += 1;
    }
  }
}

void *mz_linkedlist_remove(mz_LinkedList *list, mz_LinkedListNode *node) {
  void *result = NULL;
  if (!list->first || !list->last) {
    ERROR("list is empty");
  } else if (!node) {
    ERROR("node is null");
  } else {
    if (node == list->first && node == list->last) {
      list->first = NULL;
      list->last = NULL;
      list->count -= 1;
      result = node->value;
      free(node);
    } else if (node == list->first) {
      list->first = list->first->next;
      list->first->prev = NULL;
      list->count -= 1;
      result = node->value;
      free(node);
    } else if (node == list->last) {
      list->last = list->last->prev;
      list->last->next = NULL;
      list->count -= 1;
      result = node->value;
      free(node);
    } else {
      mz_LinkedListNode *current = NULL;
      int is_node_in_list = 0;
      for (current = list->first->next; current != list->last; current = current->next) {
        if (current == node) {
          is_node_in_list = 1;
          break;
        }
      }
      if (!is_node_in_list) {
        ERROR("node is not in list");
      } else {
        mz_LinkedListNode *next = node->next;
        mz_LinkedListNode *prev = node->prev;
        prev->next = next;
        next->prev = prev;
        list->count -= 1;
        result = node->value;
        free(node);
      }
    }
  }
  return result;
}

void *mz_linkedlist_pop(mz_LinkedList *list) {
  mz_LinkedListNode *node = list->last;
  return node != NULL ? mz_linkedlist_remove(list, node) : NULL;
}

void *mz_linkedlist_shift(mz_LinkedList *list) {
  mz_LinkedListNode *node = list->first;
  return node != NULL ? mz_linkedlist_remove(list, node) : NULL;
}

void mz_linkedlist_unshift(mz_LinkedList *list, void *value) {
  if (!list) {
    ERROR("list is null");
  } else {
    if (list->count == 0) {
      mz_linkedlist_push(list, value);
    } else {
      mz_LinkedListNode *node = calloc(1, sizeof(mz_LinkedListNode));
      if (!node) {
        ERROR("could not allocate node");
      } else {
        node->value = value;
        node->next = list->first;
        list->first->prev = node;
        list->first = node;
        list->count += 1;
      }
    }
  }
}

