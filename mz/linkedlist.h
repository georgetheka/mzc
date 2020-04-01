#ifndef __mz_linkedlist__
#define __mz_linkedlist__

#include <stdlib.h>

typedef struct mz_LinkedListNode {
  struct mz_LinkedListNode *next;
  struct mz_LinkedListNode *prev;
  void *value;
} mz_LinkedListNode;

typedef struct mz_LinkedList {
  int count;
  mz_LinkedListNode *first;
  mz_LinkedListNode *last;
} mz_LinkedList;

mz_LinkedList *mz_linkedlist_new();

void mz_linkedlist_free(mz_LinkedList *list);

void mz_linkedlist_push(mz_LinkedList *list, void *value);

void *mz_linkedlist_remove(mz_LinkedList *list, mz_LinkedListNode *node);

void *mz_linkedlist_pop(mz_LinkedList *list);

void mz_linkedlist_unshift(mz_LinkedList *list, void *value);

void *mz_linkedlist_shift(mz_LinkedList *list);

#define mz_mLinkedList_count(A) ((A)->count)
#define mz_m_linkedlist_first(A) ((A)->first != NULL ? (A)->first->value : NULL)
#define mz_m_linkedlist_last(A) ((A)->last != NULL ? (A)->last->value : NULL)

#define mz_m_linkedlist_foreach(L, S, M, V) mz_LinkedListNode *_node = NULL;\
        mz_LinkedListNode *V = NULL;\
        for(V = _node = L->S; _node != NULL; V = _node = _node->M)

#endif
