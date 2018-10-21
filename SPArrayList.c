/*
 * arrayList.c
 *
 */

#include "SPArrayList.h"
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

SPArrayList	* spArrayListCreate(int maxSize){
	if (maxSize <= 0){
		return NULL;
	}
	SPArrayList *list = (SPArrayList*) malloc( sizeof(SPArrayList));
	if (!list){
		return NULL;
	}
	list->moves =(Move**) malloc(maxSize * sizeof(Move*));

	if (!list -> moves){
		free(list);
		return NULL;
	}
	list->maxSize = maxSize;
	list->actualSize = 0;

	return list;
}

SPArrayList* spArrayListCopy(SPArrayList* src){
	if (!src){
	//	errorHandling("malloc");
		return NULL;
	}
	SPArrayList* lstCopy = spArrayListCreate(src -> maxSize);
	if (!lstCopy)
	//	errorHandling("malloc");
		return NULL;

	//lstCopy->moves = src->moves;
	int size = src->actualSize;
	for (int i = 0; i < size; i++){
		Move* moveCopy = (Move*) malloc(sizeof(Move));
		if (moveCopy == NULL){
			//errorHandling("malloc");
			return NULL;
		}
		moveCopy->from = src->moves[i]->from;
		moveCopy->to = src->moves[i]->to;
		moveCopy->moved_piece = src->moves[i]->moved_piece;
		moveCopy->taken_piece= src->moves[i]->taken_piece;
		lstCopy->moves[i] = moveCopy;
	}
	lstCopy->actualSize = src->actualSize;
	lstCopy->maxSize = src->maxSize;

	return lstCopy;
}


void spArrayListDestroy(SPArrayList* src){
	if (!src)
		return;
	int size = src->actualSize;

	for (int i = 0; i<size; i++){
		free(src->moves[i]);

	}
	if (src->moves) {
		free(src->moves);
	}
	free(src);
	return;
}

SP_ARRAY_LIST_MESSAGE spArrayListClear(SPArrayList* src){

	if (!src)
		return SP_ARRAY_LIST_INVALID_ARGUMENT;
	src -> actualSize = 0;

	return SP_ARRAY_LIST_SUCCESS;
}

SP_ARRAY_LIST_MESSAGE spArrayListAddAt(SPArrayList* src, Move * move, int index){

	if (!src || index < 0 || index > src -> actualSize)
		return SP_ARRAY_LIST_INVALID_ARGUMENT;

	if (src -> actualSize == src -> maxSize)
		return SP_ARRAY_LIST_FULL;

	for (int i = src -> actualSize; i > index; i--){
		src -> moves[i] = src -> moves[i-1];
	}
	src -> moves[index] = move;
	src -> actualSize++;

	return SP_ARRAY_LIST_SUCCESS;
}

SP_ARRAY_LIST_MESSAGE spArrayListAddFirst(SPArrayList* src, Move *move){

	if (!src){
		return SP_ARRAY_LIST_INVALID_ARGUMENT;
	}
	if (src->actualSize == src->maxSize){
		return SP_ARRAY_LIST_FULL;
	}
	for (int i = src->actualSize; i>0; i--){
		src->moves[i] = src->moves[i-1];
	}

	src->moves[0] = move;
	src->actualSize++;

	return SP_ARRAY_LIST_SUCCESS;
}

SP_ARRAY_LIST_MESSAGE spArrayListAddLast(SPArrayList* src, Move *move){

	if (!src){
		return SP_ARRAY_LIST_INVALID_ARGUMENT;
	}
	if (src->actualSize == src->maxSize){
		return SP_ARRAY_LIST_FULL;
	}
	src->moves[src->actualSize] = move;
	src->actualSize++;

	return SP_ARRAY_LIST_SUCCESS;
}

SP_ARRAY_LIST_MESSAGE spArrayListRemoveAt(SPArrayList* src, int index){

	if (!src || index < 0 || index >= src -> actualSize)
		return SP_ARRAY_LIST_INVALID_ARGUMENT;

	if (src -> actualSize == 0)
		return SP_ARRAY_LIST_EMPTY;

	free(src->moves[index]);

	for (int i = index; i < src -> actualSize - 1; i++)
	{
		src -> moves[i] = src -> moves[i+1];
	}
	src -> actualSize --;

	return SP_ARRAY_LIST_SUCCESS;
}
SP_ARRAY_LIST_MESSAGE spArrayListRemoveFirst(SPArrayList *src){

	if (!src){
		return SP_ARRAY_LIST_INVALID_ARGUMENT;
	}
	if (src -> actualSize == 0){
		return SP_ARRAY_LIST_EMPTY;
	}
	free(src->moves[0]);

	for (int i = 0; i < src->actualSize-1; i++){
		src->moves[i] = src->moves[i+1];
	}
	src->actualSize--;

	return SP_ARRAY_LIST_SUCCESS;
}

SP_ARRAY_LIST_MESSAGE spArrayListRemoveLast(SPArrayList* src){
	if (!src){
		return SP_ARRAY_LIST_INVALID_ARGUMENT;
	}
	if (src->actualSize == 0){
		return SP_ARRAY_LIST_EMPTY;
	}
	free(src->moves[src->actualSize-1]);

	src->actualSize --;

	return SP_ARRAY_LIST_SUCCESS;
}

Move * spArrayListGetAt(SPArrayList *src, int index){

	if (!src || index < 0 || index >= src -> actualSize)
		return NULL;
	return src->moves[index];
}

Move * spArrayListGetFirst(SPArrayList *src){

	return spArrayListGetAt(src, 0);
}

Move * spArrayListGetLast(SPArrayList *src){

	if (!src)
		return NULL;

	return spArrayListGetAt(src, src -> actualSize-1);
}

SPArrayList * spArrayListConcat(SPArrayList* lst1, SPArrayList* lst2){

	if (!lst1 || !lst2)
		return NULL;

	if (lst1->actualSize == 0){
		return lst2;
	}
	if (lst2->actualSize == 0){
		return lst1;
	}
	int main_size = lst1->maxSize + lst2->maxSize;
	SPArrayList* main_list = spArrayListCreate(main_size);
	if (!main_list){
		return NULL;
	}

	for (int i = 0; i < lst1->actualSize; i++){
		spArrayListAddLast(main_list, spArrayListGetAt(lst1,i));
	}
	for (int i = 0; i < lst2->actualSize; i++){
		spArrayListAddLast(main_list, spArrayListGetAt(lst2,i));
	}

	return main_list;
}

int spArrayListMaxCapacity(SPArrayList* src){

	if (!src)
		return -1;

	return src -> maxSize;
}

int spArrayListSize(SPArrayList* src){

	if (!src)
		return -1;

	return src->actualSize;
}

bool spArrayListIsFull(SPArrayList* src){

	if (!src || src -> actualSize != src -> maxSize)
		return false;

	return true;
}

bool spArrayListIsEmpty(SPArrayList* src){

	if (!src || src->actualSize != 0)
		return false;

	return true;
}
//
//spCopyMove(Move* move,SPChessBoard* chessBoard,Square src, Square dst){
//	Move* moveCopy = spCreateMove(chessBoard,src,dst);
//
//}
