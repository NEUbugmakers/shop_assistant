#pragma once
#include<stdlib.h>
#include<string.h>
typedef  int Rank;
typedef struct {//B_vector的基本数据
	size_t _esize;
	size_t _size;
	B_listNode* header ;
	B_listNode* trailer;
} B_list;
typedef struct {//B_vector的基本数据
	B_listNode* pred;
	B_listNode* succ;
	char* _elem;
} B_listNode;
