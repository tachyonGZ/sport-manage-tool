#pragma once

#include "pch.h"
#include "list.h"

typedef struct {
	LIST *pList;
	LIST_NODE *pNode;
} LIST_ITERATOR;


LIST_ITERATOR *make_list_iterator(LIST *pList, LIST_NODE *pNode);
void free_list_iterator(LIST_ITERATOR *pIter);

bool find_list_iterator(LIST *pList, const uint32_t iKey, LIST_ITERATOR **ppIter);
bool insert_list_iterator(LIST_ITERATOR *pIter, const uint32_t iKey, const void *pValue, const size_t valueSize);

const uint32_t get_list_iterator_key(const LIST_ITERATOR *pIter);
const void *get_list_iterator_value(const LIST_ITERATOR *pIter);

bool set_list_iterator_value(LIST_ITERATOR *pIter, const void *pValue, const size_t valueSize);
bool erase_list_iterator(LIST_ITERATOR *pIter);

bool is_list_iterator_equal(const LIST_ITERATOR *pL, const LIST_ITERATOR *pR);

LIST_ITERATOR *get_list_begin(LIST *pList);
LIST_ITERATOR *get_list_last(LIST *pList);
bool forward_list_iterator(LIST_ITERATOR *pIter);